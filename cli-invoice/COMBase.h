#pragma once
#include "Consol.h"

class COMBase
{
protected:
	IDispatch* p_object_; // Pointer to the IDispatch interface of the COM object.
	HRESULT hr_; // Holds the result of COM operations.

	/**
	 * Invokes a method or accesses a property on the COM object.
	 * @param name Name of the method or property.
	 * @param params Parameters to be passed to the method or property.
	 * @param param_count Count of the parameters.
	 * @param w_flags Flags describing the context of the Invoke call.
	 * @return The result of the invocation.
	 */
	VARIANT invoke(const wchar_t* name, VARIANT* params, int param_count, WORD w_flags);

public:
	/**
	 * Constructor that initializes the COM object.
	 * @param prog_id The Prog ID of the COM object to be created.
	 */
	COMBase(const std::wstring& prog_id);

	/**
	 * Destructor that releases the COM object and un-initializes the COM library.
	 */
	virtual ~COMBase()
	{
		// release COM object if it is not null
		if (p_object_) p_object_->Release();
		// un-initialize COM library
		CoUninitialize();
	}

	/**
	 * Creates a COM instance from a given Prog ID.
	 * @param prog_id The Prog ID of the COM object to be created.
	 * @return Pointer to the IDispatch interface of the created COM object.
	 */
	IDispatch* create_com_instance(const std::wstring& prog_id);

};

