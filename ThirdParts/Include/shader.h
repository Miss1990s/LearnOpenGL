#pragma once

#include <glad/glad.h>

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	//构造器，顶点着色器路径，片元着色器路径
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	//uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	void set4f(const std::string & name, float v1, float v2, float v3, float v4) const;
};
