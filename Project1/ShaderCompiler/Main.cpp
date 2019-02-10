
#include <glad/glad.h>//在运行时获取opengl实现函数的地址并将其保存在一个函数指针中供以后使用
#include <GLFW/glfw3.h>//GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口。它允许用户创建OpenGL上下文，定义窗口参数以及处理用户输入。

#include <iostream>
#include "shader.h"
using namespace std;

const unsigned	int SCR_WIDTH = 800;
const unsigned	int SCR_HEIGHT = 600;

const char * VERTEX_SHADER_PATH = "Shaders/first.vs";
const char * FRAMGENT_SHADER_PATH = "Shaders/first.fs";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()//实例化窗口
{
	glfwInit();//初始化glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Core-profile
																  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	Shader ourShader("Shaders/vertexAttr.vs", "Shaders/vertexAttr.fs");

	float vertices[] = { 
		//位置				//颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f//顶部
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//generate vbo
	glGenBuffers(1, &VBO);
	//set buffer type GL_ARRAY_BUFFER for vertex buffer;//from now on, any call on GL_ARRAY_BUFFER is applied to VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertice datas to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Link Vertex Attributes to OpenGL for decoding Vertex Datas;
	//VertexAttribPointer Apply On Datas from the VBO binded to GL_ARRAY_BUFFER;
	//更新uniform颜色
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	ourShader.use();

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		//输入
		processInput(window);

		//渲染指令

		//清空屏幕颜色
		//清空屏幕指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//指定清空颜色缓冲位(还有DEPTH深度，stencil漏字板)

		//Active Program;
		ourShader.use();

		//更新uniform颜色
		float timeValue = glfwGetTime();
		float offset = sin(timeValue) / 2.0f;
		ourShader.setFloat("offset", offset);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 

		//交换缓冲
		glfwSwapBuffers(window);
		//检查并调用事件
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

int main0()//实例化窗口
{
	glfwInit();//初始化glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Core-profile
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	Shader ourShader(VERTEX_SHADER_PATH, FRAMGENT_SHADER_PATH);

	//float vertices[] = { 
	//	//位置				//颜色
	//	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//右下
	//	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//左下
	//	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f//顶部
	//};
	float vertices[] = {
		//位置				//颜色
		0.5f, -0.5f, 0.0f,//右下
		-0.5f, -0.5f, 0.0f,//左下
		0.0f, 0.5f, 0.0f//顶部
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//generate vbo
	glGenBuffers(1, &VBO);
	//set buffer type GL_ARRAY_BUFFER for vertex buffer;//from now on, any call on GL_ARRAY_BUFFER is applied to VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertice datas to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Link Vertex Attributes to OpenGL for decoding Vertex Datas;
	//VertexAttribPointer Apply On Datas from the VBO binded to GL_ARRAY_BUFFER;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		//输入
		processInput(window);

		//渲染指令

		//清空屏幕颜色
		//清空屏幕指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//指定清空颜色缓冲位(还有DEPTH深度，stencil漏字板)

		//Active Program;
		ourShader.use();

		//更新uniform颜色
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		ourShader.set4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 
		
		//交换缓冲
		glfwSwapBuffers(window);
		//检查并调用事件
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

//视口大小//对视网膜Retina屏，窗口会大一些
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}