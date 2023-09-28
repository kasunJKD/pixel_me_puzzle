#include "gl_render.h"

// #############################################################################
//                           OpenGL Structs
// #############################################################################
struct GLContext
{
  GLuint programID;
  GLuint textureID;
  GLuint transformSBOID;
  GLuint screenSizeID;
  GLuint orthoProjectionID;
};

// #############################################################################
//                           OpenGL Globals
// #############################################################################
static GLContext glContext;


static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                         GLsizei length, const GLchar* message, const void* user)
{
  if(severity == GL_DEBUG_SEVERITY_LOW || 
     severity == GL_DEBUG_SEVERITY_MEDIUM ||
     severity == GL_DEBUG_SEVERITY_HIGH)
  {
    SM_ASSERT(false, "OpenGL Error: %s", message);
  }
  else
  {
    SM_TRACE((char*)message);
  }
}

GLuint gl_create_shader(int shaderType, char* shaderPath, Allocator* transientStorage)
{
  int fileSize = 0;
  char* vertShader = read_file(shaderPath, &fileSize, transientStorage);
  if(!vertShader)
  {
    SM_ASSERT(false, "Failed to load shader: %s",shaderPath);
    return 0;
  }

  GLuint shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &vertShader, 0);
  glCompileShader(shaderID);

  // Test if Shader compiled successfully 
  {
    int success;
    char shaderLog[2048] = {};

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shaderID, 2048, 0, shaderLog);
      SM_ASSERT(false, "Failed to compile %s Shader, Error: %s", shaderPath, shaderLog);
      return 0;
    }
  }

  return shaderID;
}

bool gl_init(Allocator* transientStorage)
{
  load_gl_functions();

  glDebugMessageCallback(&gl_debug_callback, nullptr);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glEnable(GL_DEBUG_OUTPUT);

  GLuint vertShaderID = gl_create_shader(GL_VERTEX_SHADER, 
                                         "shaders/quad.vert", transientStorage);
  GLuint fragShaderID = gl_create_shader(GL_FRAGMENT_SHADER, 
                                         "shaders/quad.frag", transientStorage);
  if(!vertShaderID || !fragShaderID)
  {
    SM_ASSERT(false, "Failed to create Shaders");
    return false;
  }

  glContext.programID = glCreateProgram();
  glAttachShader(glContext.programID, vertShaderID);
  glAttachShader(glContext.programID, fragShaderID);
  glLinkProgram(glContext.programID);

  glDetachShader(glContext.programID, vertShaderID);
  glDetachShader(glContext.programID, fragShaderID);
  glDeleteShader(vertShaderID);
  glDeleteShader(fragShaderID);

  // This has to be done, otherwise OpenGL will not draw anything
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glUseProgram(glContext.programID);

  return true;
}

void gl_render()
{
  glClearColor(119.0f/255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
  glClearDepth(0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, input.screenSizeX , input.screenSizeY);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}