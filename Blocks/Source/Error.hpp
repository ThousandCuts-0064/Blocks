#pragma once

#include <exception>
#include <string>

#include "ErrorType.hpp"

class Error : std::exception
{
private:
	ErrorType const _errorType;
	std::string _strMessage;

public:
	Error(ErrorType const& errorType) :
		_errorType(errorType),
		exception(__func__)
	{}

	Error(ErrorType const& errorType, char const* const message) :
		_errorType(errorType),
		exception(message)
	{}

	Error(ErrorType const& errorType, std::string const message) :
		_errorType(errorType),
		_strMessage(message),
		exception(_strMessage.c_str())
	{}

	inline const ErrorType GetErrorType() const { return _errorType; }
	inline const char* GetMessage() const { return what(); }
};