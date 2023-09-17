#include <exception>
#include <string.h>
#include "ErrorType.hpp"

class Error : std::exception
{
private:
	ErrorType const _errorType;

public:
	Error(ErrorType const& errorType) :
		_errorType(errorType),
		exception("Error")
	{}

	Error(ErrorType const& errorType, char const* const message) :
		_errorType(errorType),
		exception(message)
	{}

	inline const ErrorType GetErrorType() const { return _errorType; }
	inline const char* GetMessage() const { return what(); }
};