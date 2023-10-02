#include "Profile.h"

#include <sstream>
#include <utility>
#include "Entry.h"
// constructor

Profile::Profile(int id, std::string name, std::string address, std::string phone, Entry default_entry)
	: id_(id), name_(std::move(name)), phone_(std::move(phone)), address_(std::move(address)), default_entry_(std::move(default_entry))
{
}

Profile::Profile(
	int id,
	std::string name,
	std::string address,
	std::string phone,
	std::string email,
	std::string abn,
	std::string payment_method,
	std::string account_name,
	std::string bsb,
	std::string account_number,
	Entry default_entry,
	EmailRecipient email_recipient
) : id_(id), name_(std::move(name)), abn_(std::move(abn)), email_(std::move(email)),
    phone_(std::move(phone)), address_(std::move(address)), payment_methods_(std::move(payment_method)),
    account_name_(std::move(account_name)), bsb_(std::move(bsb)), account_number_(std::move(account_number)),
	default_entry_(std::move(default_entry)), email_recipient_(std::move(email_recipient))
{
}

// setter (optional)

void Profile::set_payment(std::string payment_methods, std::string account_name, std::string bsb, std::string account_number)
{
	payment_methods_ = std::move(payment_methods);
	account_name_ = std::move(account_name);
	bsb_ = std::move(bsb);
	account_number_ = std::move(account_number);
}

void Profile::set_abn(std::string abn)
{
	abn_ = std::move(abn);
}

void Profile::set_email(std::string email)
{
	email_ = std::move(email);
}

void Profile::set_default_entry(Entry default_entry)
{
	default_entry_ = std::move(default_entry);
}

void Profile::set_email_recipient(EmailRecipient email_recipient)
{
	email_recipient_ = std::move(email_recipient);
}


// getter
int Profile::get_id() const { return id_; }
std::string Profile::get_name() const { return name_; }
std::string Profile::get_abn() const { return abn_; }
std::string Profile::get_email() const { return email_; }
std::string Profile::get_phone() const { return phone_; }
std::string Profile::get_address() const { return address_; }
std::string Profile::get_payment_methods() const { return payment_methods_; }
std::string Profile::get_account_name() const { return account_name_; }
std::string Profile::get_bsb() const { return bsb_; }
std::string Profile::get_account_number() const { return account_number_; }
Entry Profile::get_default_entry() const { return default_entry_; }
EmailRecipient Profile::get_email_recipient() const { return email_recipient_; }

// methods
Profile* Profile::search_by_id(const std::vector<Profile>& profiles, int id)
{
	for (const auto& profile : profiles) 
	{
		if (profile.get_id() == id) 
		{
			return const_cast<Profile*>(&profile); // Return a non-const pointer to the found profile
		}
	}
	return nullptr; // Return nullptr if no profile with the given id is found
}

Profile Profile::from_json(const nlohmann::json& j)
{
	Profile profile;
	profile.id_ = j.value("id", 0);
	profile.name_ = j.value("name", "");
	profile.abn_ = j.value("abn", "");
	profile.email_ = j.value("email", "");
	profile.phone_ = j.value("phone", "");
	profile.address_ = j.value("address", "");
	profile.payment_methods_ = j.value("payment_methods", "");
	profile.account_name_ = j.value("account_name", "");
	profile.bsb_ = j.value("bsb", "");
	profile.account_number_ = j.value("account_number", "");
	profile.default_entry_ = Entry::from_json(j.value("default_param", nlohmann::json()));
	profile.email_recipient_ = EmailRecipient::from_json(j.value("recipient_detail", nlohmann::json()));
	return profile;
}

std::string Profile::to_string() const
{
	std::ostringstream oss;
	oss << "ID: " << id_ << "\n"
		<< "Name: " << name_ << "\n"
		<< "ABN: " << abn_ << "\n"
		<< "Email: " << email_ << "\n"
		<< "Phone: " << phone_ << "\n"
		<< "Address: " << address_ << "\n"
		<< "Payment Methods: " << payment_methods_ << "\n"
		<< "Account Name: " << account_name_ << "\n"
		<< "BSB: " << bsb_ << "\n"
		<< "Account Number: " << account_number_ << "\n"
		<< "Default Entry: " << default_entry_.to_string() << "\n"
		<< "Email Recipient: " << email_recipient_.to_string();
	return oss.str();
}

