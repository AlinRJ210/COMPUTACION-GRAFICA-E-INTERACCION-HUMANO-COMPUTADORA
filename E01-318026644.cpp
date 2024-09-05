#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

// Fragment Shader
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);	 			\n\
}";

void CrearTriangulo()
{
	GLfloat vertices[] = {
		//C 
		//Primer triangulo, primer lateral
		-1.0f, 0.0f, 0.0f,
		-.85f, 1.f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		-.85f, 0.0f, 0.0f,
		-.85f, 1.0f, 0.0f,

		//Parte superior C
		- .51f, 1.0f, 0.0f,
		-.85f, .85f, 0.0f,
		-.85f, 1.0f, 0.0f,

		-.51f, 1.0f, 0.0f,
		-.85f, .85f, 0.0f,
		-.51f,.85f,0.0f,

		//Parte inferior C
		-.85f, 0.0f, 0.0f,
		-.51f,0.0f,0.0f,
		-.51f, .15f, 0.0f,

		-.85f, 0.0f, 0.0f,
		-.51f,.15f,0.0f,
		-.85f, .15f, 0.0f,

		//A
		//Rombo izquierdo
		-.50f, -.25f, 0.0f,
		-.40f,.60f,0.0f,
		-.35f,-.25f, 0.0f,

		-.25f, .60f, 0.0f,
		-.40f,.60f,0.0f,
		-.35f,-.25f, 0.0f,

		//Rombo derecho

		-.25f, .60f, 0.0f,
		0.0f,-.25f,0.0f,
		-.15f,-.25f, 0.0f,

		-.25f, .60f, 0.0f,
		-.10f,.60f,0.0f,
		0.0f,-.25f, 0.0f,

		//Trapecio superior

		-.40f,.60f,0.0f,
		-.38f,.75,0.0f,//
		-.10f,.60f,0.0f,

		-.115f,.75f,0.0f, //
		-.38f,.75,0.0f,
		-.10f,.60f,0.0f,

		


		//Trapecio parte inferior

		-.32f,0.22f,0.0f,
		-.29,.37f,0.0f,
		-.19,.22,0.0f,

		-.21f,.37f,0.0f,
		-.29,.37f,0.0f,
		-.19,.22,0.0f,


		//R

		//Primer lateral
		0.05f, .5f, 0.0f,
		.20f,-.50f,0.0f,
		0.05f,-.50f, 0.0f,

		.05f, .50f, 0.0f,
		.20f,-.50f,0.0f,
		.20f,.50f, 0.0f,

		//Rectangulo superior

		.20f,.50f, 0.0f,
		.20f,.35f, 0.0f,
		.5f,.50f, 0.0f,

		.20f,.35f, 0.0f,
		.5f,.35f, 0.0f,
		.5f,.50f, 0.0f,


		//Rectangulo inferior

		.20f,.20f, 0.0f,
		.20f,.05f, 0.0f,
		.5f,.05f, 0.0f,

		.20f,.20f, 0.0f,
		.5f,.05f, 0.0f,
		.5f,.20f, 0.0f,

		//Rectangulo interno

		0.5f,.20f,0.0f,
		0.5f,.35f,0.0f,
		0.35f,.35f,0.0f,

		0.5f,.20f,0.0f,
		0.35f,.20f,0.0f,
		0.35f,.35f,0.0f,

		//Rombo inferior

		.35f,.05f, 0.0f,
		.5f,-.50f, 0.0f,
		.35f,-.50f, 0.0f,

		.35f,-.50f, 0.0f,
		.35f,.05f, 0.0f,
		.20f,.05f, 0.0f,

		//J
		//Rectangulo inferior
		.55f,-.75f,0.0f,
		.85f,-.75f,0.0,
		.85f,-.60f,0.0,

		.55f,-.75f,0.0f,
		.85f,-.60f,0.0,
		.55f,-.60f,0.0,

		//Rectangulo superior
		.55f,.25f,0.0f,
		.55f,.10f,0.0,
		.98f,.25f,0.0,

		.98f, .10f, 0.0f,
		.55f, .10f, 0.0,
		.98f, .25f, 0.0,

		//Rectangulo medio

		.85f,-.60f,0.0f,
		.70f,-.60f,0.0f,
		.85f,.10f,0.0f,

		.70f,-.60f, 0.0f,
		.70f, .10f, 0.0f,
		.85f,.10f, 0.0f










		

		

	};
	glGenVertexArrays(1, &VAO); // generar 1 VAO
	glBindVertexArray(VAO); // asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // pasar los datos al VBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al compilar el shader %d: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al linkear: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al validar: %s \n", eLog);
		return;
	}
}

int main()
{
	// Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	// Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Crear ventana
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);
	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);

	CrearTriangulo();
	CompileShaders();

	float colores[3][4] = {
		{1.0f, 0.0f, 0.0f, 1.0f}, // Rojo
		{0.0f, 1.0f, 0.0f, 1.0f}, // Verde
		{0.0f, 0.0f, 1.0f, 1.0f}  // Azul
	};

	int color = 0;  //nos permite seleccionar el color 
	double tiempo = glfwGetTime(); //nos permite almacenar el tiempo
	double intervalo = 2.0; // Cambia cada segundo

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Cambiar el color de fondo cada segundo
		double cambio = glfwGetTime();
		if (cambio - tiempo >= intervalo)
		{
			color = (color + 1) % 3;
			tiempo = cambio;
		}

		glClearColor(colores[color][0], colores[color][1], colores[color][2], colores[color][3]);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 95);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwTerminate();
	return 0;
}
