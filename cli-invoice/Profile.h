#pragma once
#include <string>

#include "Email.h"
#include "Entry.h"

class Profile
{
public:
	// constructor
	Profile() = default;
	Profile(
		int id,
		std::string name,
		std::string address,
		std::string phone,
		Entry default_entry
	);

	Profile(
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
	);

	// setter (optional)
	void set_payment(
		std::string payment_methods,
		std::string account_name,
		std::string bsb,
		std::string account_number
	);
	void set_abn(std::string abn);
	void set_email(std::string email);
	void set_default_entry(Entry default_entry);
	void set_email_recipient(EmailRecipient email_recipient);

	// getter
	int get_id() const;
	std::string get_name() const;
	std::string get_abn() const;
	std::string get_email() const;
	std::string get_phone() const;
	std::string get_address() const;
	std::string get_payment_methods() const;
	std::string get_account_name() const;
	std::string get_bsb() const;
	std::string get_account_number() const;
	Entry get_default_entry() const;
	EmailRecipient get_email_recipient() const;

	// methods
	std::string to_string() const;

	// static
	static Profile* search_by_id(const std::vector<Profile>& profiles, int id);
	static Profile from_json(const nlohmann::json& j);

private:
	int id_;
	std::string name_;
	std::string abn_;
	std::string email_;
	std::string phone_;
	std::string address_;
	std::string payment_methods_;
	std::string account_name_;
	std::string bsb_;
	std::string account_number_;
	Entry default_entry_;
	EmailRecipient email_recipient_;
};

