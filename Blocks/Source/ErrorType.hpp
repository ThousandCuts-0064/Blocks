#pragma once

enum ErrorType
{
	Unknown,
	GlfwInitFail,
	GlfwCreateWindowFail,
	GlewInitFail,
	OpenFileFail,
	CompileShaderFail,
};