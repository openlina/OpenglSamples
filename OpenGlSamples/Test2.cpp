
#include "Public.h"

/*
1.	glfw需要初始化

2.	glad也需要初始化， glfw中提供了获取上下文的地址，该地址可以初始化glad, glad初始化后才能使用内部接口

3.  glfwGetPrimaryMonitor 用于全屏模式监控

4.	VBO顶点缓冲对象，Vertex Buffer Objects， 用于存储大量顶点，使用该缓冲对象可以一次性将大量顶点从cpu发送到gpu
	glGenBuffers(); 生成顶点缓冲对象

5.  OpenGL 有很多种缓冲对象类型， 其中顶点缓冲对象类型是GL_ARRAY_BUFFER， 将VBO绑定到GL_ARRAY_BUFFER目标中 glBindBuffer();

6.	OpenGL着色器： 程序中最少需要使用顶点着色器和片段着色器， 绘制三角形例子中，顶点着色器可以存储顶点信息， 片段着色器存储颜色信息
	着色器使用GLSL语言编写，类似C语言， 使用步骤：
	创建着色器 glCreateShader(类型：GL_VERTEX_SHADER 顶点着色器， GL_FRAGMENT_SHADER片段着色器)
	附着着色器源码到着色器上 glShaderSource
	编译着色器 glCompileShader
	判断编译是否成功 glGetShaderiv, 若不成功则获取失败信息 getShaderInfo

7. 着色器程序：创建好着色器后还需要创建着色器程序 glShaderProgram
	glCreateProgram //创建着色器程序
	glAttchShader	//将着色器附着到着色器程序上
	glLinkProgram	//链接着色器程序
	glGetProgramiv	//获取链接状态
	glGetProgramInfo	//获取链接错误信息

*/

const GLchar *VertexShaderSource = "#version 330 core \n"
"layout(location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}";

const GLchar *FragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	cout << "frame size chaned: w=" << width << ", h=" << height << endl;
	glViewport(0, 0, width, height);
}

void windowContentScaleCallback(GLFWwindow*, float xscale, float yscal) {
	cout << "frame size chaned: wxscale=" << xscale << ", yscal=" << yscal << endl;
}


int main()
{
	glfwInit(); //glfw 初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(640, 480, "window", /*glfwGetPrimaryMonitor()*/nullptr, nullptr);

	if (window == nullptr) {
		goto END;
	}

	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 640, 480, 15);全屏模式

	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetWindowContentScaleCallback(window, windowContentScaleCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//顶点着色器， 用于存储顶点信息
	GLuint vertexShader; //顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);//将着色器源码附着到着色器对象上
	glCompileShader(vertexShader);//编译着色器

	//判断着色器编译是否成功
	GLint success;
	GLchar logInfo[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, logInfo);
		cout << "compile shader failed: " << logInfo << endl;
	}

	//片段着色器， 用于存储颜色信息
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	GLint success_fragment;
	GLchar logInfo_fragment[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragment);
	if (!success_fragment) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, logInfo_fragment);
		cout << "compile shader failed: " << logInfo_fragment << endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, logInfo);
		cout << "link shader program failed: " << logInfo << endl;
	}

	GLuint VBO, VAO;//定义顶点缓冲对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //生成顶点缓冲对象
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//将顶点缓冲对象绑定到 GL_ARRAY_BUFFER目标中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

END:
	glfwTerminate();
}