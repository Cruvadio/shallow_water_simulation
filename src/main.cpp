//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "WaterComputeShaderProgram.h"
#include "Skybox.h"
#include "Pool.h"
#include "Sphere.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#ifndef NDEBUG
#include <unistd.h>
#endif
#include <random>


const float sphereRadius = 0.4;
const vec3 sphereCenter = vec3(-0.6, -0.62, -0.6);
ShaderProgram drops;


using namespace glm;
static const GLsizei WIDTH = 1024, HEIGHT = 735; //размеры окна
static const GLint WATER_SIZE = 160;
static const GLint NUM_DROPS = 10;
unsigned int indexbuffer;

vec3 lightColor = vec3(1.0);
vec3 lightPos = vec3(2.0, 2.2, 1.0);

unsigned int VAO;

int * data;

mat4 model = mat4(1.0f);
mat4 view = mat4(1.0);
mat4 projection = mat4(1.0);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
//void processInput(GLFWwindow *window);
void calculateHeights (WaterComputeShaderProgram& shader, int width, int height);
GLuint createTexture (int width, int height);
void	createIndexBuffer (int width, int height);
GLuint createPBO (int size);
void renderToVBO (const ShaderProgram& shaderProgram, int width, int height);
void initFrameBuffer(int width, int height);
void drawWater (const ShaderProgram& waterShader, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void makeDrop();
void makeDrop(vec2 clicked_vec, int width, int height);

Camera camera(glm::vec3(0.0f, 0.5f, 1.5f));

Camera camera1 (glm::vec3(-2, 1.5, 3), vec3(0, 1, 0), -56, -30);
Camera camera2 (glm::vec3(1.5, 0.4, -0.05), vec3(0, 1, 0), -182, -6);
Camera camera3 (glm::vec3(-0.7, 2.7, 1.4), vec3(0, 1, 0), -60, -64);


float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool allow_simulation = true;
bool wireframe = false;

int initGL()
{
	int res = 0;
	#ifndef NDEBUG
	chdir("..");
	#endif
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

  GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSetKeyCallback(window,key_callback);
    glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;

	

	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	std::unordered_map<GLenum, std::string> shaders;
	std::unordered_map<GLenum, std::string> compute;
	shaders[GL_VERTEX_SHADER]   = "shaders/pool.vs";
	shaders[GL_FRAGMENT_SHADER] = "shaders/pool.fs";
	ShaderProgram waterProgram(shaders); GL_CHECK_ERRORS;

	waterProgram.StartUseShader();
	waterProgram.SetUniform("skybox", 0);GL_CHECK_ERRORS;
	waterProgram.SetUniform("wallTexture", 1);GL_CHECK_ERRORS;
	waterProgram.SetUniform("normalMap", 2);GL_CHECK_ERRORS;
	waterProgram.StopUseShader();

	shaders[GL_VERTEX_SHADER]   = "shaders/skybox.vs";
	shaders[GL_FRAGMENT_SHADER] = "shaders/skybox.fs";
	ShaderProgram skyboxShader(shaders); GL_CHECK_ERRORS;

	shaders[GL_VERTEX_SHADER]   = "shaders/box.vs";
	shaders[GL_FRAGMENT_SHADER] = "shaders/box.fs";
	ShaderProgram poolShader(shaders); GL_CHECK_ERRORS;

	shaders[GL_VERTEX_SHADER]   = "shaders/sphere.vs";
	shaders[GL_FRAGMENT_SHADER] = "shaders/sphere.fs";
	ShaderProgram sphereShader(shaders); GL_CHECK_ERRORS;

	compute[GL_COMPUTE_SHADER] = "shaders/calculatingHeights.comp"; 
	WaterComputeShaderProgram calculatingProgram(compute, WATER_SIZE, WATER_SIZE, 16, 16);
	
	compute[GL_COMPUTE_SHADER] = "shaders/dropShader.comp"; 
	ShaderProgram dropShader(compute);
	drops = dropShader;

	glGenVertexArrays(1, &VAO);
	std::vector<std::string> textures{
		"textures/posx.jpg",
		"textures/negx.jpg",
		"textures/posy.jpg",
		"textures/negy.jpg",
		"textures/posz.jpg",
		"textures/negz.jpg",
	};

	camera = camera1;

	mat4 mat = glm::scale(mat4(1.0), vec3(1., 7./12., 1.));
	mat = translate(mat, vec3(0, -0.75, 0));

	Pool pool(mat, poolShader, "textures/Stones/Pebbles_017_baseColor.jpg", "textures/Stones/Pebbles_017_normal.jpg");
	Skybox skybox(textures, skyboxShader);
	Sphere sphere("models/sphere.obj", sphereShader, sphereRadius, sphereCenter);

  	glfwSwapInterval(1); 
	createIndexBuffer(WATER_SIZE, WATER_SIZE);
	mat4 rot = rotate(model, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	model = rot * model;
	view = translate(view, vec3(0,0,-2));
	projection = perspective(radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);

	for(int i = 0; i < NUM_DROPS; i++)
		makeDrop();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

		//processInput(window);
		glfwPollEvents();


		if (allow_simulation)
			calculateHeights(calculatingProgram, WATER_SIZE, WATER_SIZE);

    	glViewport  (0, 0, WIDTH, HEIGHT);
    	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    	glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		view = camera.GetViewMatrix();
		projection = perspective(radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);
		

		sphere.drawSphere(projection, view, lightPos, camera.Position, WATER_SIZE, WATER_SIZE);
		pool.drawPool(projection, view, lightPos, camera.Position,sphereCenter, sphereRadius, WATER_SIZE, WATER_SIZE);
		skybox.bindTexture();
		pool.bindTexture(GL_TEXTURE1);
		drawWater(waterProgram,  WATER_SIZE, WATER_SIZE);
		skybox.drawSkybox(projection, view);

		glfwSwapBuffers(window); 
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &indexbuffer);
	calculatingProgram.cleanUp();
	pool.cleanUp();
	skybox.cleanUp();
	sphere.cleanUp();

	glfwTerminate();
	return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
    	if (firstMouse)    	{
        	lastX = xpos;
        	lastY = ypos;
        	firstMouse = false;
    	}

    	float xoffset = xpos - lastX;
    	float yoffset = lastY - ypos; 
    	lastX = xpos;
    	lastY = ypos;

    	camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else 
	{
		firstMouse = true;
	}*/

	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   // camera.ProcessMouseScroll(yoffset);
}


void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		makeDrop();
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		wireframe = !wireframe;
	}
	else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		allow_simulation = !allow_simulation;
	}
	else if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
	{
		allow_simulation = true;
		for (int i = 0; i < NUM_DROPS; i++)
			makeDrop();
	}
	else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	else if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		camera = camera1;
	}
	else if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		camera = camera2;
	}
	else if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		camera = camera3;
	}

	
}

void	createIndexBuffer (int width, int height)
{
	data = (int *) malloc ( (width - 1)*(height - 1)*6*sizeof (int) );
	int	  k    = 0;
	
	for ( int i = 1; i < width - 1; i++ )
		for ( int j = 1; j < height - 1; j++ )
		{
			data [k]   = i + width*j;				// first triangle (i,j)-(i+1,j) - (i,j+1)
			data [k+1] = i + width*j + 1;
			data [k+2] = i + width*j + width;
			
			data [k+3] = i + width*j + 1;			// second triangle (i+1,j)-(i+1,j+1) - (i,j+1)
			data [k+4] = i + width*j + width + 1;
			data [k+5] = i + width *j + width;
			k+= 6;
		}
		
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (width - 2)*(height - 2)*6*sizeof (int), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	free ( data );
}


void calculateHeights (WaterComputeShaderProgram& shader, int width, int height)
{
	shader.StartUseShader(); GL_CHECK_ERRORS;
	shader.SetUniform("width", width); GL_CHECK_ERRORS;
	shader.SetUniform("height", height); GL_CHECK_ERRORS;
	
	shader.startComputing(); GL_CHECK_ERRORS;
	shader.StopUseShader(); GL_CHECK_ERRORS;

	
	glBindVertexArray(VAO);  GL_CHECK_ERRORS;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);  GL_CHECK_ERRORS;
	
	GLintptr vertex_texcoord_offset = 0 * sizeof(GLfloat);
	GLintptr vertex_normal_offset =  0 * sizeof(GLfloat);
	GLintptr vertex_position_offset = 0;

	shader.bindPositions();
	glEnableVertexAttribArray(0);  GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)vertex_position_offset);  GL_CHECK_ERRORS;

	shader.bindNormals();
    glEnableVertexAttribArray(1); GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)vertex_normal_offset); GL_CHECK_ERRORS;

	shader.bindTextureCoords();
	glEnableVertexAttribArray(2); GL_CHECK_ERRORS;
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)vertex_texcoord_offset); GL_CHECK_ERRORS;

	glBindVertexArray(0); GL_CHECK_ERRORS;
	glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERRORS;
	
}

void drawWater (const ShaderProgram& waterShader, int width, int height)
{
	if (wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	waterShader.StartUseShader(); GL_CHECK_ERRORS;
	waterShader.SetUniform("view", view); GL_CHECK_ERRORS;
	waterShader.SetUniform("projection", projection); GL_CHECK_ERRORS;
	waterShader.SetUniform("model", model); GL_CHECK_ERRORS;
	waterShader.SetUniform("lightPos", lightPos); GL_CHECK_ERRORS;
	waterShader.SetUniform("lightColor", lightColor); GL_CHECK_ERRORS;
	waterShader.SetUniform("cameraPos", camera.Position); GL_CHECK_ERRORS;
	waterShader.SetUniform("sphereRadius", sphereRadius); GL_CHECK_ERRORS;
	waterShader.SetUniform("sphereCenter", sphereCenter); GL_CHECK_ERRORS;

	glBindVertexArray(VAO); GL_CHECK_ERRORS;
	glDrawElements(GL_TRIANGLES, (width-2)*(height-2)*6, GL_UNSIGNED_INT, NULL); GL_CHECK_ERRORS;

	glBindVertexArray(0); GL_CHECK_ERRORS;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	waterShader.StopUseShader();
} 

void makeDrop()
{
	
	drops.StartUseShader();
	ivec2 rand_vec = ivec2( 1 + rand() % WATER_SIZE, 1 + rand() % WATER_SIZE);
	drops.SetUniform("rand_vec", rand_vec); GL_CHECK_ERRORS;
	drops.SetUniform("width", WATER_SIZE); GL_CHECK_ERRORS;
	drops.SetUniform("strength", (float)(rand() % 150)/(float)10000); GL_CHECK_ERRORS;
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier   ( GL_SHADER_STORAGE_BARRIER_BIT );
	drops.StopUseShader();
}



