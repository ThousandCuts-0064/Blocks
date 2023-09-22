#pragma once

enum ErrorType
{
	Unknown,
	NotSupported,

	GlfwInitFail,
	GlfwCreateWindowFail,
	GlewInitFail,
	OpenFileFail,
	CompileShaderFail,
};