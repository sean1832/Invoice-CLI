#pragma once
#include <string>
#include <nlohmann/json.hpp>

class EmailSender
{
public:
	EmailSender() = default;
	EmailSender(std::string& email, std::string& password);

	// getter
	std::string get_email() const;
	std::string get_password() const;

	// setter
	void set_email(const std::string& email);
	void set_password(const std::string& password);

	// methods
	static EmailSender from_json(const nlohmann::json& j);
private:
	std::string email_;
	std::string password_;
};

class EmailRecipient
{
public:
	EmailRecipient() = default;
	EmailRecipient(std::string email, std::string subject, std::string body);
	EmailRecipient(std::string email, std::string subject, std::string body, std::string attachment);

	// getter
	std::string get_email() const;
	std::string get_subject() const;
	std::string get_body() const;
	std::string get_attachment() const;

	// setter
	void set_email(const std::string& email);
	void set_subject(const std::string& subject);
	void set_body(const std::string& body);
	void set_attachment(const std::string& attachment);

	// methods
	std::string to_string() const;

	static EmailRecipient from_json(const nlohmann::json& j);

private:
	std::string email_;
	std::string subject_;
	std::string body_;
	std::string attachment_;
};


class Email
{
public:
	// methods
	static void send(const EmailSender& sender, const EmailRecipient& recipient);
private:
};

