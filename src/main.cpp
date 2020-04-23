//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "WaterComputeShaderProgram.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#ifndef NDEBUG
#include <unistd.h>
#endif
#include <random>



using namespace glm;
static const GLsizei WIDTH = 640, HEIGHT = 480; //размеры окна
static const GLint WATER_SIZE = 64;
unsigned int normalMap, texCoords;
unsigned int Hcurr, Hnext, Hprev;
unsigned int textureID;
unsigned int framebuffer;
unsigned int indexbuffer;


unsigned int quadVAO, quadVBO;
unsigned int VAO, VBO;

GLuint pboIdv;
GLuint pboIdn;
GLuint pboIdt;

int * data;

mat4 model = mat4(1.0f);
mat4 view = mat4(1.0);
mat4 projection = mat4(1.0);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void calculateHeights (WaterComputeShaderProgram& shader, int width, int height);
GLuint createTexture (int width, int height);
void	createIndexBuffer (int width, int height);
GLuint createPBO (int size);
void renderToVBO (const ShaderProgram& shaderProgram, int width, int height);
void initFrameBuffer(int width, int height);
void drawWater (const ShaderProgram& waterShader, int width, int height);

Camera camera(glm::vec3(0.0f, 0.5f, 1.5f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool allow_drop = true;

int initGL()
{
	int res = 0;
	#ifndef NDEBUG
	chdir("..");
	#endif
	//грузим функции opengl через glad
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

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); 
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
    glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;

	
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	std::unordered_map<GLenum, std::string> compute;
	shaders[GL_VERTEX_SHADER]   = "shaders/pool.vs";
	shaders[GL_FRAGMENT_SHADER] = "shaders/pool.fs";
	ShaderProgram waterProgram(shaders); GL_CHECK_ERRORS;
	//shaders[GL_VERTEX_SHADER] = "shaders/r2vb.vs";
	//shaders[GL_FRAGMENT_SHADER] = "shaders/r2vb.fs";
	//ShaderProgram r2vbProgram(shaders); GL_CHECK_ERRORS;
	compute[GL_COMPUTE_SHADER] = "shaders/calculatingHeights.comp"; GL_CHECK_ERRORS;
	WaterComputeShaderProgram calculatingProgram(compute, WATER_SIZE, WATER_SIZE, 16, 16);
	
	compute[GL_COMPUTE_SHADER] = "shaders/dropShader.comp"; GL_CHECK_ERRORS;
	//VBO = calculatingProgram.returnVertices();
	/*
	shaders[GL_FRAGMENT_SHADER] = "shaders/heights.fs";
	ShaderProgram heightProgram(shaders); GL_CHECK_ERRORS;
	shaders[GL_FRAGMENT_SHADER] = "shaders/normals.fs";
	ShaderProgram normalProgram(shaders); GL_CHECK_ERRORS;*/

  glfwSwapInterval(1); // force 60 frames per second
  
  //Создаем и загружаем геометрию поверхности
  //
  GLuint g_vertexArrayObject;
  unsigned int EBO;

	float vertices[] = {
    // positions          
     0.5f,  0.5f, 0.0f,   
     0.5f, -0.5f, 0.0f,   
    -0.5f, -0.5f, 0.0f,   
    -0.5f,  0.5f, 0.0f 
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
	unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
	VBO = createPBO(sizeof(Vertex) * WATER_SIZE * WATER_SIZE);
    GLuint vertexLocation = 0; // simple layout, assume have only positions at location = 0
	
	glGenVertexArrays(1, &VAO);   GL_CHECK_ERRORS;

	// Water verticies
	/*
    
	                                                     GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER,	VBO);                                           GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), (GLfloat*)vertices, GL_STATIC_DRAW); 		   GL_CHECK_ERRORS;
	
    glGenVertexArrays(1, &g_vertexArrayObject);                                                    GL_CHECK_ERRORS;
    glBindVertexArray(g_vertexArrayObject);     
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); GL_CHECK_ERRORS;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (GLfloat*)indices, GL_STATIC_DRAW); GL_CHECK_ERRORS;
	
	*/
	

	 // screen quad VAO
    glGenVertexArrays(1, &quadVAO); GL_CHECK_ERRORS;
    glGenBuffers(1, &quadVBO); GL_CHECK_ERRORS;
    glBindVertexArray(quadVAO); GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0); GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(1); GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); GL_CHECK_ERRORS;
	glBindVertexArray(0);
	                                                   
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); GL_CHECK_ERRORS;
	glEnable(GL_PROGRAM_POINT_SIZE);  
	
	//initFrameBuffer(WATER_SIZE, WATER_SIZE);
	createIndexBuffer(WATER_SIZE, WATER_SIZE);
  
	model = rotate(model, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	view = translate(view, vec3(0,0,-2));
	projection = perspective(radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);
	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		processInput(window);
		glfwPollEvents();

		//очищаем экран каждый кадр
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
	
    // очистка и заполнение экрана цветом

		//renderToVBO(r2vbProgram, WATER_SIZE, WATER_SIZE);
		calculateHeights(calculatingProgram, WATER_SIZE, WATER_SIZE);
    //
    	glViewport  (0, 0, WIDTH, HEIGHT);
    	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    	glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		drawWater(waterProgram,  WATER_SIZE, WATER_SIZE);

		glfwSwapBuffers(window); 
	}

	//очищаем vbo и vao перед закрытием программы
  //
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &indexbuffer);
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
    	if (firstMouse)    	{
        	lastX = xpos;
        	lastY = ypos;
        	firstMouse = false;
    	}

    	float xoffset = xpos - lastX;
    	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    	lastX = xpos;
    	lastY = ypos;

    	camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else 
	{
		firstMouse = true;
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

GLuint createTexture (int width, int height)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint createPBO (int size)
{
	GLuint result;
	glGenBuffers(1, &result);
  	glBindBuffer(GL_PIXEL_PACK_BUFFER, result);
  	glBufferData(GL_PIXEL_PACK_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	return result;
}

void initFrameBuffer(int width, int height)
{
	GLuint PBOsize = width * height * 4 * sizeof(float);

	pboIdv = createPBO(PBOsize);
	pboIdn = createPBO(PBOsize);
	pboIdt = createPBO(PBOsize);
	// Making framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture

    Hnext = createTexture(width, height);
	Hprev = createTexture(width, height);
	Hcurr = createTexture(width, height);
	textureID = createTexture(width, height);
	normalMap = createTexture(width, height);
	texCoords = createTexture(width, height);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_TEXTURE_2D, textureID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                            GL_TEXTURE_2D, normalMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                            GL_TEXTURE_2D, texCoords, 0);

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	createIndexBuffer (int width, int height)
{
	data = (int *) malloc ( (width - 2)*(height - 2)*6*sizeof (int) );
	int	  k    = 0;
	
	for ( int i = 1; i < width - 2; i++ )
		for ( int j = 1; j < height - 2; j++ )
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
	//free ( data );
}

void renderToVBO (const ShaderProgram& shaderProgram, int width, int height)
{
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	shaderProgram.StartUseShader(); GL_CHECK_ERRORS;
	glViewport  (0, 0, width, height);
	//mat4 scal(1.0);
	//scal = scale(scal, vec3((float)height/(float)HEIGHT,(float)width/(float)WIDTH,  1.0));
	//shaderProgram.SetUniform("texScale", scal);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); GL_CHECK_ERRORS;
	glDrawBuffers(2, buffers); GL_CHECK_ERRORS;
	glBindVertexArray(quadVAO); GL_CHECK_ERRORS;
    glDrawArrays(GL_TRIANGLES, 0, 6); GL_CHECK_ERRORS;
	glBindVertexArray(0); GL_CHECK_ERRORS;
	shaderProgram.StopUseShader(); GL_CHECK_ERRORS;
	
	// Read verticies
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIdv); GL_CHECK_ERRORS;
	glReadBuffer(GL_COLOR_ATTACHMENT0); GL_CHECK_ERRORS;
	glReadPixels( 0, 0, width, height, GL_RGBA, GL_FLOAT, NULL ); GL_CHECK_ERRORS;
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); GL_CHECK_ERRORS;
	//Read normals
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIdn); GL_CHECK_ERRORS;
	glReadBuffer(GL_COLOR_ATTACHMENT1); GL_CHECK_ERRORS;
	glReadPixels( 0, 0, width, height, GL_RGBA, GL_FLOAT, NULL ); GL_CHECK_ERRORS;
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	//Read texCoords
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIdt);  GL_CHECK_ERRORS;
	glReadBuffer(GL_COLOR_ATTACHMENT2); GL_CHECK_ERRORS;
	glReadPixels( 0, 0, width, height, GL_RGBA, GL_FLOAT, NULL ); GL_CHECK_ERRORS;
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); GL_CHECK_ERRORS;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_CHECK_ERRORS;

	/*glBindBuffer(GL_COPY_WRITE_BUFFER, VBO);
	glBindBuffer(GL_COPY_READ_BUFFER, pboIdv);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 4 * sizeof(float) * width * height);  GL_CHECK_ERRORS;

	glBindBuffer(GL_COPY_READ_BUFFER, pboIdn);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 4 * sizeof(float) * width * height, 4 * sizeof(float) * width * height);

	glBindBuffer(GL_COPY_READ_BUFFER, pboIdt);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 2 * 4 * sizeof(float) * width * height, 4 * sizeof(float) * width * height);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); GL_CHECK_ERRORS;*/

	glBindVertexArray(VAO);  GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, pboIdv);  GL_CHECK_ERRORS;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);  GL_CHECK_ERRORS;
	GLintptr vertex_texcoord_offset = /*2 * 4 * width * height*/0 * sizeof(float);
	GLintptr vertex_normal_offset = /*4 * width * height*/0 * sizeof(float);
	GLintptr vertex_position_offset = 0 * sizeof(float);
	glEnableVertexAttribArray(0);  GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)vertex_position_offset);  GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, pboIdn); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(1); GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)vertex_normal_offset); GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, pboIdt); GL_CHECK_ERRORS;
	glEnableVertexAttribArray(2); GL_CHECK_ERRORS;
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)vertex_texcoord_offset); GL_CHECK_ERRORS;

	glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERRORS;
	glBindVertexArray(0); GL_CHECK_ERRORS;
}

void calculateHeights (WaterComputeShaderProgram& shader, int width, int height)
{
	shader.StartUseShader(); GL_CHECK_ERRORS;
	shader.SetUniform("width", width); GL_CHECK_ERRORS;
	shader.SetUniform("height", height); GL_CHECK_ERRORS;
	
	//shader.SetUniform("probability", rand()); GL_CHECK_ERRORS;
	shader.startComputing(); GL_CHECK_ERRORS;
	shader.StopUseShader(); GL_CHECK_ERRORS;
	
	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO); GL_CHECK_ERRORS;

	void* mapped_data = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vertex) * width * height, GL_MAP_READ_BIT); GL_CHECK_ERRORS;

	Vertex* vertices = (Vertex *) mapped_data;

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			std::cout << "(" << vertices[i + j*width].position.x << ", " << vertices[i + j*width].position.y << ", " << vertices[i + j*width].position.z << ")\n";
			std::cout << "(" << vertices[i + j*width].normal.x << ", " << vertices[i + j*width].normal.y << ", " << vertices[i + j*width].normal.z << ")\n";
		}
	glMemoryBarrier   ( GL_SHADER_STORAGE_BARRIER_BIT );
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); GL_CHECK_ERRORS;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); GL_CHECK_ERRORS;*/

	//glBindBuffer(GL_COPY_WRITE_BUFFER, VBO); GL_CHECK_ERRORS;
	//glBindBuffer(GL_COPY_READ_BUFFER, SSBO); GL_CHECK_ERRORS;
	//glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(Vertex) * width * height);  GL_CHECK_ERRORS; 
	
	
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
	waterShader.StartUseShader(); GL_CHECK_ERRORS;
	
	view = camera.GetViewMatrix();
	projection = perspective(radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);
	waterShader.SetUniform("view", view); GL_CHECK_ERRORS;
	waterShader.SetUniform("projection", projection); GL_CHECK_ERRORS;
	waterShader.SetUniform("model", model); GL_CHECK_ERRORS;
	waterShader.SetUniform("lightPos", vec3(2.2, 2.0, 0.0)); GL_CHECK_ERRORS;
	waterShader.SetUniform("lightColor", vec3(1.0)); GL_CHECK_ERRORS;
	waterShader.SetUniform("cameraPos", camera.Position); GL_CHECK_ERRORS;
	glBindVertexArray(VAO); GL_CHECK_ERRORS;
	glDrawElements(GL_TRIANGLES, (width-3)*(height-3)*6, GL_UNSIGNED_INT, NULL); GL_CHECK_ERRORS;
	//glDrawArrays(GL_POINTS, 0 , (width) * (height));
	glBindVertexArray(0); GL_CHECK_ERRORS;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	waterShader.StopUseShader();
} 


void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::unordered_map<GLenum, std::string> compute;
		compute[GL_COMPUTE_SHADER] = "shaders/dropShader.comp";
		ShaderProgram dropShader(compute);
		dropShader.StartUseShader();
		ivec2 rand_vec = ivec2(rand() % WATER_SIZE, rand() % WATER_SIZE);
		dropShader.SetUniform("rand_vec", rand_vec); GL_CHECK_ERRORS;
		dropShader.SetUniform("width", WATER_SIZE); GL_CHECK_ERRORS;
		dropShader.SetUniform("strength", (float)(rand() % 150)/(float)10000); GL_CHECK_ERRORS;
		glDispatchCompute(1, 1, 1);
		glMemoryBarrier   ( GL_SHADER_STORAGE_BARRIER_BIT );
		dropShader.StopUseShader();
	}
}