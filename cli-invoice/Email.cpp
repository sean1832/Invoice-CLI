#include "Email.h"
#include "Console.h"
#include <sstream>
#include <curl/curl.h>

#include "Common.h"
#include "Helper.h"

using namespace std;

// sender
EmailSender::EmailSender(string& email, string& password) : email_(std::move(email)), password_(std::move(password))
{
}
string EmailSender::get_email() const { return email_; }
string EmailSender::get_password() const { return password_; }

void EmailSender::set_email(const string& email) { email_ = email; }
void EmailSender::set_password(const string& password) { password_ = password; }

EmailSender EmailSender::from_json(const nlohmann::json& j)
{
	if (j.empty())
	{
		throw invalid_argument("[EmailSender::from_json] invalid argument: json is empty");
	}
	if (!j.contains("email") || !j.contains("password"))
	{
		throw invalid_argument("[EmailSender::from_json] invalid argument: json does not contain field email or password");
	}
	if (!j["email"].is_string() || !j["password"].is_string())
	{
		throw invalid_argument("[EmailSender::from_json] invalid argument: email or password is not a string");
	}

	EmailSender sender;
	sender.set_email(j["email"]);
	sender.set_password(j["password"]);
	return sender;
}

// recipient
EmailRecipient::EmailRecipient(std::string email, std::string subject, std::string body) :
email_(std::move(email)),
subject_(std::move(subject)),
body_(std::move(body))
{
}

EmailRecipient::EmailRecipient(std::string email, std::string subject, std::string body,std::string attachment) :
email_(std::move(email)),
subject_(std::move(subject)),
body_(std::move(body)),
attachment_(std::move(attachment))
{
}

std::string EmailRecipient::get_email() const { return email_; }

std::string EmailRecipient::get_subject() const { return subject_; }

std::string EmailRecipient::get_body() const { return body_; }

std::string EmailRecipient::get_attachment() const { return attachment_; }

void EmailRecipient::set_email(const std::string& email) { email_ = email; }

void EmailRecipient::set_subject(const std::string& subject) { subject_ = subject; }

void EmailRecipient::set_body(const std::string& body) { body_ = body; }

void EmailRecipient::set_attachment(const std::string& attachment)
{
	if (Helper::file_exist(attachment))
	{
		attachment_ = attachment;
	}
	else
	{
		throw invalid_argument("[EmailRecipient::set_attachment] invalid argument: attachment file " + attachment + " does not exist");
	}
}

EmailRecipient EmailRecipient::from_json(const nlohmann::json& j)
{
	if (j.empty())
	{
		return {}; // empty recipient
	}
	EmailRecipient recipient;
	if (j.contains("recipient_email") && j["recipient_email"].is_string())
		recipient.set_email(j["recipient_email"]);

	if (j.contains("subject") && j["subject"].is_string())
		recipient.set_subject(j["subject"]);

	if (j.contains("body") && j["body"].is_string())
		recipient.set_body(j["body"]);

	if (j.contains("payload") && j["payload"].is_string())
		recipient.set_attachment(j["payload"]);

	return recipient;
}

string EmailRecipient::to_string() const
{
	ostringstream oss;
	const string indent = "    "; // 4 spaces
	oss <<  endl;
	oss << indent << "email: " << email_ << endl;
	oss << indent << "subject: " << subject_ << endl;
	oss << indent << "body: " << body_ << endl;
	oss << indent << "attachment: " << attachment_ << endl;
	return oss.str();
}

// email

struct upload_status { int lines_read; };

string payload_text; // this will hold the constructed payload

size_t payload_source_read(void* ptr, size_t size, size_t nmemb, void* userp)
{
	struct upload_status* upload_ctx = static_cast<struct upload_status*>(userp);
	const char* data;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	// Make sure we do not read beyond the payload_text string
	if (upload_ctx->lines_read < payload_text.length()) {
		// Calculate the amount of bytes left to read
		const size_t len = payload_text.length() - upload_ctx->lines_read;
		// Determine how much we can copy now (minimum of len and size*nmemb)
		const size_t to_copy = (len < size * nmemb) ? len : size * nmemb;

		// Copy the data and advance the read position
		memcpy(ptr, payload_text.c_str() + upload_ctx->lines_read, to_copy);
		upload_ctx->lines_read += to_copy;

		return to_copy;
	}

	return 0;
}

string payload_text_generator(const string& sender, const string& recipient, const string& subject, const string& body)
{
	ostringstream oss;
	oss << "Date: " << Common::get_current_datetime("%a, %d %b %Y %H:%M:%S %z") << "\r\n";
	oss << "To: " << recipient << "\r\n";
	oss << "From: " << sender << "\r\n";
	oss << "Subject: " << subject << "\r\n";
	oss << "\r\n";
	oss << body << "\r\n";
	return oss.str();
}

std::map<std::string, std::string> mime_types = {
	{".txt", "text/plain"},
	{".pdf", "application/pdf"},
	{".png", "image/png"},
	// ... add more as needed
};

std::string get_mime_type(const std::string& filename) {
	const std::string::size_type idx = filename.rfind('.');

	if (idx != std::string::npos) {
		const std::string extension = filename.substr(idx);
		const auto iter = mime_types.find(extension);
		if (iter != mime_types.end()) {
			return iter->second;
		}
	}

	// return default MIME type if we can't determine it
	return "application/octet-stream";
}

void Email::send(const EmailSender& sender, const EmailRecipient& recipient)
{
	CURLcode res = CURLE_OK;

	upload_status upload_ctx;
	upload_ctx.lines_read = 0;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		const string username = sender.get_email();
		const string password = sender.get_password();
		const string to = recipient.get_email();
		const string subject = recipient.get_subject();
		const string body = recipient.get_body();
		const string attachment_path_str = recipient.get_attachment();

		payload_text = payload_text_generator(username, to, subject, body);
		// show payload
		cout << "-------------------------------------------------------\n";
		cout << "payload: " << endl;
		cout << payload_text << endl;
		cout << "-------------------------------------------------------\n";


		curl_mime* mime = curl_mime_init(curl);

		// add body
		curl_mimepart* part = curl_mime_addpart(mime);
		curl_mime_data(part, body.c_str(), CURL_ZERO_TERMINATED);

		// mime (attachment)
		if (!attachment_path_str.empty())
		{
			const char* attachment_path = attachment_path_str.c_str();
			curl_mimepart* attach_part = curl_mime_addpart(mime);
			curl_mime_filedata(attach_part, attachment_path);
			curl_mime_type(attach_part, get_mime_type(attachment_path).c_str());
			curl_mime_filename(attach_part, Helper::get_filename_from_path(attachment_path, true).c_str());
			curl_mime_encoder(attach_part, "base64");
		}

		curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

		curl_slist* recipients = nullptr;
		recipients = curl_slist_append(recipients, to.c_str());

		curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, ("Subject: " + subject).c_str());
		headers = curl_slist_append(headers, ("To: " + to).c_str()); // Added this line to explicitly set the 'To' field in the email header.

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, username.c_str());
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source_read);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=PLAIN");

		// Send the message
		cout << "Sending email to " << to << "..." << endl;
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			// Clean up
			curl_slist_free_all(recipients);
			curl_slist_free_all(headers);
			curl_mime_free(mime);
			curl_easy_cleanup(curl);
			// Handle error
			throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
		}
		
		// Clean up
		curl_slist_free_all(recipients);
		curl_slist_free_all(headers);
		curl_mime_free(mime);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}

