
#include "Public.h"

/*
1.	glfw��Ҫ��ʼ��

2.	gladҲ��Ҫ��ʼ���� glfw���ṩ�˻�ȡ�����ĵĵ�ַ���õ�ַ���Գ�ʼ��glad, glad��ʼ�������ʹ���ڲ��ӿ�

3.  glfwGetPrimaryMonitor ����ȫ��ģʽ���

4.	VBO���㻺�����Vertex Buffer Objects�� ���ڴ洢�������㣬ʹ�øû���������һ���Խ����������cpu���͵�gpu
	glGenBuffers(); ���ɶ��㻺�����

5.  OpenGL �кܶ��ֻ���������ͣ� ���ж��㻺�����������GL_ARRAY_BUFFER�� ��VBO�󶨵�GL_ARRAY_BUFFERĿ���� glBindBuffer();

6.	OpenGL��ɫ���� ������������Ҫʹ�ö�����ɫ����Ƭ����ɫ���� ���������������У�������ɫ�����Դ洢������Ϣ�� Ƭ����ɫ���洢��ɫ��Ϣ
	��ɫ��ʹ��GLSL���Ա�д������C���ԣ� ʹ�ò��裺
	������ɫ�� glCreateShader(���ͣ�GL_VERTEX_SHADER ������ɫ���� GL_FRAGMENT_SHADERƬ����ɫ��)
	������ɫ��Դ�뵽��ɫ���� glShaderSource
	������ɫ�� glCompileShader
	�жϱ����Ƿ�ɹ� glGetShaderiv, �����ɹ����ȡʧ����Ϣ getShaderInfo

7. ��ɫ�����򣺴�������ɫ������Ҫ������ɫ������ glShaderProgram
	glCreateProgram //������ɫ������
	glAttchShader	//����ɫ�����ŵ���ɫ��������
	glLinkProgram	//������ɫ������
	glGetProgramiv	//��ȡ����״̬
	glGetProgramInfo	//��ȡ���Ӵ�����Ϣ

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
	glfwInit(); //glfw ��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(640, 480, "window", /*glfwGetPrimaryMonitor()*/nullptr, nullptr);

	if (window == nullptr) {
		goto END;
	}

	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 640, 480, 15);ȫ��ģʽ

	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glfwSetWindowContentScaleCallback(window, windowContentScaleCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������ɫ���� ���ڴ洢������Ϣ
	GLuint vertexShader; //������ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����������ɫ��
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);//����ɫ��Դ�븽�ŵ���ɫ��������
	glCompileShader(vertexShader);//������ɫ��

	//�ж���ɫ�������Ƿ�ɹ�
	GLint success;
	GLchar logInfo[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, logInfo);
		cout << "compile shader failed: " << logInfo << endl;
	}

	//Ƭ����ɫ���� ���ڴ洢��ɫ��Ϣ
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

	GLuint VBO, VAO;//���嶥�㻺�����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //���ɶ��㻺�����
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//�����㻺�����󶨵� GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���������ݸ��Ƶ�������ڴ���
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