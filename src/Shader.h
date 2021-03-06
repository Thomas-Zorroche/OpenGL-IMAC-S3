#pragma once
#include <glad/glad.h>

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FramgentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	// caching for uniforms
	std::unordered_map <std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	inline unsigned int getID() const { return m_RendererID; }

	// Set uniforms
	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};