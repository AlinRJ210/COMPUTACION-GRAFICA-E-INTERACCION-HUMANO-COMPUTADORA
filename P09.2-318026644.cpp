/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;

// variables dragon 
float movDragon;
float movDragonset;
float rotala1;
float rotala1Offset;

bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;

float toffsetnumerocambiau = 0.0;
//float angulovaria = 0.0f;

//varibles de rotacion para el dragon
int direccion = 0; // 0: X positivo, 1: X negativo 
const float rotacion_angulo_dragon = 180.0f; // Ángulo en grados

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
std::vector<Texture> texturas1(43);
int textura_actual1 = 0;
std::vector<Texture> texturas2(43);
int textura_actual2 = 0;
std::vector<Texture> texturas3(43);
int textura_actual3 = 0;
std::vector<Texture> texturas4(43);
int textura_actual4 = 0;
std::vector<Texture> texturas5(43);
int textura_actual5 = 0;
std::vector<Texture> texturas6(43);
int textura_actual6 = 0;
std::vector<Texture> texturas7(43);
int textura_actual7 = 0;
std::vector<Texture> texturas8(43);
int textura_actual8 = 0;
std::vector<Texture> texturas9(43);
int textura_actual9 = 0;
std::vector<Texture> texturas10(43);
int textura_actual10 = 0;
std::vector<Texture> texturas11(43);
int textura_actual11 = 0;
std::vector<Texture> texturas12(43);
int textura_actual12 = 0;
std::vector<Texture> texturas13(43);
int textura_actual13 = 0;
std::vector<Texture> texturas14(43);
int textura_actual14 = 0;
std::vector<Texture> texturas15(43);
int textura_actual15 = 0;
std::vector<Texture> texturas16(43);
int textura_actual16 = 0;
std::vector<Texture> texturas17(43);
int textura_actual17 = 0;
std::vector<Texture> texturas18(43);
int textura_actual18 = 0;
std::vector<Texture> texturas19(43);
int textura_actual19 = 0;
std::vector<Texture> texturas20(43);
int textura_actual20 = 0;
std::vector<Texture> texturas21(43);
int textura_actual21 = 0;
std::vector<Texture> texturas22(43);
int textura_actual22 = 0;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
//Texture textures[23];



Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Skybox skybox;
Model Ala_1;
Model Ala_2;
Model arco_letrero;

//materiales
Material Material_brillante;
Material Material_opaco;

//TEXTURAS
GLfloat contador_tiempo1 = 0.0f;
GLfloat contador_tiempo2 = 0.0f;
GLfloat contador_tiempo3 = 0.0f;
GLfloat contador_tiempo4 = 0.0f;
GLfloat contador_tiempo5 = 0.0f;
GLfloat contador_tiempo6 = 0.0f;
GLfloat contador_tiempo7 = 0.0f;
GLfloat contador_tiempo8 = 0.0f;
GLfloat contador_tiempo9 = 0.0f;
GLfloat contador_tiempo10 = 0.0f;
GLfloat contador_tiempo11 = 0.0f;
GLfloat contador_tiempo12 = 0.0f;
GLfloat contador_tiempo13 = 0.0f;
GLfloat contador_tiempo14 = 0.0f;
GLfloat contador_tiempo15 = 0.0f;
GLfloat contador_tiempo16 = 0.0f;
GLfloat contador_tiempo17 = 0.0f;
GLfloat contador_tiempo18 = 0.0f;
GLfloat contador_tiempo19 = 0.0f;
GLfloat contador_tiempo20 = 0.0f;
GLfloat contador_tiempo21 = 0.0f;
GLfloat contador_tiempo22 = 0.0f;


bool texturas_run=true;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//ESFFERA
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA1/numero" + std::to_string(i + 1) + ".png";
		texturas1[i] = Texture(path.c_str());
		texturas1[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA2/numero" + std::to_string(i + 1) + ".png";
		texturas2[i] = Texture(path.c_str());
		texturas2[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA3/numero" + std::to_string(i + 1) + ".png";
		texturas3[i] = Texture(path.c_str());
		texturas3[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA4/numero" + std::to_string(i + 1) + ".png";
		texturas4[i] = Texture(path.c_str());
		texturas4[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA5/numero" + std::to_string(i + 1) + ".png";
		texturas5[i] = Texture(path.c_str());
		texturas5[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA6/numero" + std::to_string(i + 1) + ".png";
		texturas6[i] = Texture(path.c_str());
		texturas6[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA7/numero" + std::to_string(i + 1) + ".png";
		texturas7[i] = Texture(path.c_str());
		texturas7[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA8/numero" + std::to_string(i + 1) + ".png";
		texturas8[i] = Texture(path.c_str());
		texturas8[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA9/numero" + std::to_string(i + 1) + ".png";
		texturas9[i] = Texture(path.c_str());
		texturas9[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA10/numero" + std::to_string(i + 1) + ".png";
		texturas10[i] = Texture(path.c_str());
		texturas10[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA11/numero" + std::to_string(i + 1) + ".png";
		texturas11[i] = Texture(path.c_str());
		texturas11[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA12/numero" + std::to_string(i + 1) + ".png";
		texturas12[i] = Texture(path.c_str());
		texturas12[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA13/numero" + std::to_string(i + 1) + ".png";
		texturas13[i] = Texture(path.c_str());
		texturas13[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA14/numero" + std::to_string(i + 1) + ".png";
		texturas14[i] = Texture(path.c_str());
		texturas14[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA15/numero" + std::to_string(i + 1) + ".png";
		texturas15[i] = Texture(path.c_str());
		texturas15[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA16/numero" + std::to_string(i + 1) + ".png";
		texturas16[i] = Texture(path.c_str());
		texturas16[i].LoadTextureA();
	}

	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA17/numero" + std::to_string(i + 1) + ".png";
		texturas17[i] = Texture(path.c_str());
		texturas17[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA18/numero" + std::to_string(i + 1) + ".png";
		texturas18[i] = Texture(path.c_str());
		texturas18[i].LoadTextureA();
	}
	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA19/numero" + std::to_string(i + 1) + ".png";
		texturas19[i] = Texture(path.c_str());
		texturas19[i].LoadTextureA();
	}

	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA20/numero" + std::to_string(i + 1) + ".png";
		texturas20[i] = Texture(path.c_str());
		texturas20[i].LoadTextureA();
	}

	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA21/numero" + std::to_string(i + 1) + ".png";
		texturas21[i] = Texture(path.c_str());
		texturas21[i].LoadTextureA();
	}

	for (int i = 0; i < 43; ++i) {
		std::string path = "Textures/CARPETA22/numero" + std::to_string(i + 1) + ".png";
		texturas22[i] = Texture(path.c_str());
		texturas22[i].LoadTextureA();
	}

	

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/Dragon_separado.obj");
	Ala_1 = Model();
	Ala_1.LoadModel("Models/Ala_1.obj");
	Ala_2 = Model();
	Ala_2.LoadModel("Models/Ala_2.obj");
	arco_letrero = Model();
	arco_letrero.LoadModel("Models/arco_completo.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	

	//Dragon
	movDragon = 0.0f;
	movDragonset = 0.5f;
	rotala1 = 0.0f;
	rotala1Offset = 0.50; //me permite mover mas rapido o lento el modelo 

	//rotala1 += 0.5f * deltaTime;

	avanza = true;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		
	
		//MOVIMIENTO DRAGON 
	
		// modificacion 
		if (mainWindow.getarrancar() == true)
		{
			if (avanza)
			{
				switch (direccion)
				{
				case 0: // X positivo
					if (movDragon < 400.0f)
					{
						movDragon += movDragonset * deltaTime;
						rotala1 += rotala1Offset * deltaTime;
						
					}
					else
					{
						direccion = 1; // Cambiar a Y positivo
						movDragon = 200.0f; // Asegurar que no sobrepase el límite
					}
					break;

				case 1: // X negativo
					if (movDragon > -400.0f)
					{
						movDragon -= movDragonset * deltaTime;
						rotala1 -= rotala1Offset * deltaTime;
					}
					else
					{
						direccion = 0; // Cambiar a X negativo
						movDragon = -200.0f; // Asegurar que no sobrepase el límite
					}
					break;
				
				}
			}
		}

		//TEXTURAS
		/*
		contador_tiempo += deltaTime;
		
		
			if (contador_tiempo >= 20.0f) {
			
				texturas_run = !texturas_run;
				contador_tiempo = 0.0f;
			}*/
		contador_tiempo1 += deltaTime;

		if (contador_tiempo1 >= 25.0f) {
			textura_actual1 = (textura_actual1 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo1 = 0.0f;
		}
		contador_tiempo2 += deltaTime;

		if (contador_tiempo2 >= 25.0f) {
			textura_actual2 = (textura_actual2 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo2 = 0.0f;
		}
		contador_tiempo3 += deltaTime;

		if (contador_tiempo3 >= 25.0f) {
			textura_actual3 = (textura_actual3 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo3 = 0.0f;
		}
		contador_tiempo4 += deltaTime;

		if (contador_tiempo4 >= 25.0f) {
			textura_actual4 = (textura_actual4 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo4 = 0.0f;
		}
		contador_tiempo5 += deltaTime;

		if (contador_tiempo5 >= 25.0f) {
			textura_actual5 = (textura_actual5 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo5 = 0.0f;
		}
		contador_tiempo6 += deltaTime;

		if (contador_tiempo6 >= 25.0f) {
			textura_actual6 = (textura_actual6 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo6 = 0.0f;
		}
		contador_tiempo7 += deltaTime;

		if (contador_tiempo7 >= 25.0f) {
			textura_actual7 = (textura_actual7 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo7 = 0.0f;
		}
		contador_tiempo8 += deltaTime;

		if (contador_tiempo8 >= 25.0f) {
			textura_actual8 = (textura_actual8 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo8 = 0.0f;
		}
		contador_tiempo9 += deltaTime;

		if (contador_tiempo9 >= 25.0f) {
			textura_actual9 = (textura_actual9 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo9 = 0.0f;
		}
		contador_tiempo10 += deltaTime;

		if (contador_tiempo10 >= 25.0f) {
			textura_actual10 = (textura_actual10 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo10 = 0.0f;
		}
		contador_tiempo11 += deltaTime;

		if (contador_tiempo11 >= 25.0f) {
			textura_actual11 = (textura_actual11 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo11 = 0.0f;
		}
		contador_tiempo12 += deltaTime;

		if (contador_tiempo12 >= 25.0f) {
			textura_actual12 = (textura_actual12 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo12 = 0.0f;
		}
		contador_tiempo13 += deltaTime;

		if (contador_tiempo13 >= 25.0f) {
			textura_actual13 = (textura_actual13 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo13 = 0.0f;
		}
		contador_tiempo14 += deltaTime;

		if (contador_tiempo14 >= 25.0f) {
			textura_actual14 = (textura_actual14 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo14 = 0.0f;
		}
		contador_tiempo15 += deltaTime;

		if (contador_tiempo15 >= 25.0f) {
			textura_actual15 = (textura_actual15 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo15 = 0.0f;
		}
		contador_tiempo16 += deltaTime;
		if (contador_tiempo16 >= 25.0f) {
			textura_actual16 = (textura_actual16 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo16 = 0.0f;
		}
		
		contador_tiempo17 += deltaTime;
		if (contador_tiempo17 >= 25.0f) {
			textura_actual17 = (textura_actual17 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo17 = 0.0f;
		}
		contador_tiempo18 += deltaTime;
		if (contador_tiempo18 >= 25.0f) {
			textura_actual18 = (textura_actual18 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo18 = 0.0f;
		}
		contador_tiempo19 += deltaTime;
		if (contador_tiempo19 >= 25.0f) {
			textura_actual19 = (textura_actual19 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo19 = 0.0f;
		}
		contador_tiempo20 += deltaTime;
		if (contador_tiempo20 >= 25.0f) {
			textura_actual20 = (textura_actual20 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo20 = 0.0f;
		}
		contador_tiempo21 += deltaTime;
		if (contador_tiempo21 >= 25.0f) {
			textura_actual21 = (textura_actual21 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo21 = 0.0f;
		}
		contador_tiempo22 += deltaTime;
		if (contador_tiempo22 >= 25.0f) {
			textura_actual22 = (textura_actual22 + 1) % 43;  // Avanza al siguiente índice y reinicia si llega a 23
			contador_tiempo22 = 0.0f;
		}
			
		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// DRAGON 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f-movDragon/6, 5.0f+(3*sin(glm::radians(movDragon*8))), 6.0)); //al aumentar acorto el periodo con angulo varia * n 
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Dragon_M.RenderModel();

		//Instancia del helicoptero
		// Aplicar transformaciones de modelo
		model = glm::mat4(1.0);

		
		switch (direccion)
		{
		case 0: // X positivo
			model = glm::translate(model, glm::vec3(0.0f - movDragon / 6, 50.0f + (3 * sin(glm::radians(movDragon * 8))), 6.0f));
			model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
			break;

		case 1: // X negativo 
			model = glm::translate(model, glm::vec3(0.0f - movDragon / 6, 50.0f + (3 * sin(glm::radians(movDragon * 8))), 6.0f));
			model = glm::rotate(model, rotacion_angulo_dragon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
			break;
		}

		
		modelaux = model;

		
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		// Enviar la matriz de modelo al shader
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();

		


		//Ala 1

		model = modelaux;

		//CODIGO AUXILIAR
		// Variables
		float time = glfwGetTime(); // Tiempo actual
		float frequency = 0.50f; // Frecuencia de aleteo, puedes ajustarla
		float maxAngle = 25.0f; // Ángulo máximo de rotación en grados

		// Calcula el ángulo de rotación basado en una función seno
		float angle = maxAngle * sin(frequency * time); // Oscila entre -45 y 45 grados

		// Configura la transformación del modelo
		//model = glm::mat4(1.0f); // Resetea la matriz a identidad
		model = glm::translate(model, glm::vec3(0.0f - rotala1 / 6, 0.0f + (3 * sin(glm::radians(rotala1 * 8))), 0.0));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		// Configura el color y las matrices uniformes
		color = glm::vec3(0.5f, 0.5f, 0.5f); // Color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Renderiza el ala
		Ala_1.RenderModel();

		/*
		//ALA 1
		model = glm::translate(model, glm::vec3(0.0f - rotala1 / 6, 5.0f + (3 * sin(glm::radians(rotala1 * 8))), 6.0)); //al aumentar acorto el periodo con angulo varia * n 
		//model = glm::rotate(model, rotala1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_1.RenderModel();*/

		//Ala 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f - rotala1 / 6, 0.0f + (3 * sin(glm::radians(rotala1 * 8))), 0.0f)); //al aumentar acorto el periodo con angulo varia * n 
		//model = glm::translate(model, glm::vec3(0.30f, 5.0f, 6.05f));
		//	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotala1 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		color = glm::vec3(2.0f, 2.0f, 2.0f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_2.RenderModel();

		
		//ARCO PARA ANIMACION

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f,- 5.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotala1 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 10.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco_letrero.RenderModel();

	//PROGRAMA PARA IR ALTERNANDO
	// 1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas1[textura_actual1].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas2[textura_actual2].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 3
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas3[textura_actual3].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 4
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas4[textura_actual4].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 5
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas5[textura_actual5].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 6
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas6[textura_actual6].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// //7
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas7[textura_actual7].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// //8
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas8[textura_actual8].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 9
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas9[textura_actual9].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 10
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas10[textura_actual10].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// 11
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas11[textura_actual11].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
	// //12
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(18.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas12[textura_actual12].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//13
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas13[textura_actual13].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//14
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(24.0f, 36.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas14[textura_actual14].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//15
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		texturas15[textura_actual15].UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//16

		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas16[textura_actual16].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//17
		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas17[textura_actual17].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		
		//18
		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas18[textura_actual18].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		
		//19

		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas19[textura_actual19].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		
		//20
		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas20[textura_actual20].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//21

		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas21[textura_actual21].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//22
		//1
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 33.0f, 10.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Usar la textura actual
		texturas22[textura_actual22].UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		
		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
