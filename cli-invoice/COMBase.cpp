#include "COMBase.h"

VARIANT COMBase::invoke(const wchar_t* name, VARIANT* params, int param_count, WORD w_flags)
{
	DISPID disp_id;
	// Get the dispatch ID for the name.
	hr_ = p_object_->GetIDsOfNames(IID_NULL, const_cast<wchar_t**>(&name), 1, LOCALE_USER_DEFAULT, &disp_id);
	if (FAILED(hr_))
	{
		// Handle errors and throw exception if the ID retrieval fails.
		std::wcerr << L"Failed to get ID for name: " << name << L". HRESULT: " << hr_ << std::endl;
		throw std::exception("[COMBase::invoke] Failed to get ID for name");
	}

	DISPPARAMS dp = {nullptr, nullptr, static_cast<UINT>(param_count), static_cast<UINT>(param_count) };
	dp.rgvarg = params;

	VARIANT result;
	VariantInit(&result);
	// Invoke the method or access the property on the COM object.
	hr_ = p_object_->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, w_flags, &dp, &result, nullptr, nullptr);
	if (FAILED(hr_))
	{
		// Handle errors and throw exception if the invocation fails.
		std::wcerr << L"Failed to invoke method or access property: " << name << L". HRESULT: " << hr_ << std::endl;
		throw std::exception("[COMBase::invoke] Failed to invoke method or access property");
	}
	// Return the result of the invocation.
	// This is a variant type, so the caller must handle the result appropriately.
	return result;
}

COMBase::COMBase(const std::wstring& prog_id)
{
	// initialize COM library
	CoInitialize(nullptr);
	// create COM instance
	p_object_ = create_com_instance(prog_id);
}


IDispatch* COMBase::create_com_instance(const std::wstring& prog_id)
{
	IDispatch* p_dispatch = nullptr;
	CLSID cls_id;
	// Get the CLSID for the Prog ID.
	hr_ = CLSIDFromProgID(prog_id.c_str(), &cls_id);
	if (SUCCEEDED(hr_))
	{
		// Create the COM instance.
		hr_ = CoCreateInstance(cls_id, nullptr, CLSCTX_LOCAL_SERVER, IID_IDispatch, reinterpret_cast<void**>(&p_dispatch));
		if (FAILED(hr_))
		{
			std::wcerr << L"Failed to create COM instance for Prog ID: " << prog_id << L". HRESULT: " << hr_ << std::endl;
			throw std::exception("[COMBase::create_com_instance] Failed to create COM instance");
		}
	}
	else
	{
		std::wcerr << L"Failed to get CLSID for Prog ID: " << prog_id << L". HRESULT: " << hr_ << std::endl;
		throw std::exception("[COMBase::create_com_instance] Failed to get CLSID for Prog ID");
	}
	return p_dispatch;
}