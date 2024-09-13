#include "View.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <Light.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sgraph/GLScenegraphRenderer.h"
#include "sgraph/GLScenegraphLighter.h"
#include "VertexAttrib.h"
#include <glm/gtx/string_cast.hpp>
#include "TextureImage.h"
View::View() {

}

View::~View(){

}

bool pressed = false; // left button pressed originally set to false
bool firstCallbackAfterPress = false; // has the first callback happened after press
float pressedX;
float pressedY;
float pressedRotX;
float pressedRotY;
float rotX = 0;
float rotY = 0;
float KONSTANT = 0.01;

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        pressed = true;
        firstCallbackAfterPress = false;
        cout << "PRESSED" << endl;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        pressed = false;
        cout << "NOT PRESSED" << endl;
    }
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    // if this is the first call after pressed, then memorize the  xy 
    // location of the nouse and the current xy rotations
    if( pressed == true && firstCallbackAfterPress == false ){
        firstCallbackAfterPress = true;
        pressedX = xpos;
        pressedY = ypos;
        pressedRotX = rotX;
        pressedRotY = rotY;
    }

    if (pressed) {
        std::cout << "CALCULATING" << std::endl;
        float dx = xpos - pressedX;
        float deltaRotY = KONSTANT * dx;
        rotY =  (pressedRotY + deltaRotY);


        float dy = ypos - pressedY;
        float deltaRotX = KONSTANT * dy;
        rotX =  (pressedRotX + deltaRotX);

        //
        // based on changes in the xy location of mouse, figure out which axis we are rotating on
        //float rotdy = xpos - prevX;
        //float rotdx = ypos - prevY;

        //if (rotdx > 0) {
        //    rotX = rotX - 0.007;
        //}
        //else {
        //    rotX = rotX + 0.007;
        //}

        //if (rotdy > 0) {
        //    rotY = rotY - 0.007;
        //}
        //else {
        //    rotY = rotY + 0.007;
        //}
        //prevX = xpos;
        //prevY = ypos;
    }
}

void View::init(Callbacks *callbacks,Model& model) 
{
    //cout << "GOT TO VIEWINIT()" << endl;
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 800, "Daas Potapov Assignment 5", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
     glfwSetWindowUserPointer(window, (void *)callbacks);

    
     glfwSetCursorPosCallback(window, cursor_callback);
     glfwSetMouseButtonCallback(window, mouse_callback);

    //using C++ functions as callbacks to a C-style library
    glfwSetKeyCallback(window, 
    [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->onkey(key,scancode,action,mods);
    });

    glfwSetWindowSizeCallback(window, 
    [](GLFWwindow* window, int width,int height)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->reshape(width,height);
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    cout << "GOT HERE1" << endl;

    // create the shader program
    program.createProgram(string("shaders/phong-multiple.vert"),
                        string("shaders/phong-multiple.frag"));
    // assuming it got created, get all the shader variables that it uses
    // so we can initialize them at some point
    // enable the shader program

    cout << "GOT HERE2" << endl;

    program.enable();
    shaderLocations = program.getAllShaderVariables();


    initObjects(model);
    
	int window_width,window_height;
    glfwGetFramebufferSize(window,&window_width,&window_height);

    //prepare the projection matrix for perspective projection
	projection = glm::perspective(glm::radians(60.0f),(float)window_width/window_height,0.1f,10000.0f);
    glViewport(0, 0, window_width,window_height);

    frames = 0;
    time = glfwGetTime();
    cameraMode = GLOBAL;

    renderer = new sgraph::GLScenegraphRenderer(modelview,objects,shaderLocations);
    

    cameraRotation = glm::mat4(1.0f);
}


void View::initShaderVariables(vector<util::Light>& lights) {
  //get input variables that need to be given to the shader program
  for (int i = 0; i < lights.size(); i++)
    {
      LightLocation ll;
      stringstream name;

      name << "light[" << i << "]";
      ll.ambient = shaderLocations.getLocation(name.str() + "" +".ambient");
      ll.diffuse = shaderLocations.getLocation(name.str() + ".diffuse");
      ll.specular = shaderLocations.getLocation(name.str() + ".specular");
      ll.position = shaderLocations.getLocation(name.str() + ".position");
      ll.spotDirection = shaderLocations.getLocation(name.str() + ".spotDirection");
      ll.spotCutoff = shaderLocations.getLocation(name.str() + ".spotCutoff");
      lightLocations.push_back(ll);
    }
}

void View::initObjects(Model& model) {
    sgraph::IScenegraph * scenegraph = model.getScenegraph();
    map<string,util::PolygonMesh<VertexAttrib> > meshes = scenegraph->getMeshes();

    //cout << "GOT TO INITOBJECTS()" << endl;
    map<string,string> shaderVarsToVertexAttribs;

    shaderVarsToVertexAttribs["vPosition"] = "position";
    shaderVarsToVertexAttribs["vNormal"] = "normal";
    shaderVarsToVertexAttribs["vTexCoord"] = "texcoord";

    for (typename map<string,util::PolygonMesh<VertexAttrib> >::iterator it=meshes.begin();
           it!=meshes.end();
           it++) {
        util::ObjectInstance * obj = new util::ObjectInstance(it->first);
        obj->initPolygonMesh(shaderLocations,shaderVarsToVertexAttribs,it->second);
        objects[it->first] = obj;
    }

    vector<string> meshNames = model.getMeshNames();

    for (string name:meshNames) {
        util::ObjectInstance *obj =  new util::ObjectInstance("triangles");
        obj->initPolygonMesh<VertexAttrib>(
            program,                    // the shader program
            shaderLocations,            // the shader locations
            shaderVarsToVertexAttribs,  // the shader variable -> attrib map
            model.getMesh(name));                      // the actual mesh object
        objects[name] = obj;
    }

    glEnable(GL_TEXTURE_2D);
    vector<string> textureNames = model.getTextureNames();
    for (string name:textureNames) {
        util::TextureImage *textureObject = model.getTextureObject(name);
        unsigned int textureId;
        glGenTextures(1,&textureId);
        glBindTexture(GL_TEXTURE_2D,textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureObject->getWidth(),textureObject->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,textureObject->getImage());
        glGenerateMipmap(GL_TEXTURE_2D);
        textureIds[name] = textureId; 
        cout << "Name = " << name << " Id = " << textureId << endl;
    }
    
}



void View::display(sgraph::IScenegraph *scenegraph, Model& model, float time, float rX, float rY) {
    //out << "GOT TO DISPLAY()" << endl;
    program.enable();
    //glClearColor(0.0,0.0,0.0,1); 
    double r = 153.0/256.0; 
    double g = 240.0/256.0;
    double b = 255.0/256.0;
    glClearColor(r,g,b,1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

   
    int window_width,window_height;
    glfwGetFramebufferSize(window,&window_width,&window_height);
 	projection = glm::perspective(glm::radians(60.0f),(float)window_width/window_height,0.1f,10000.0f);
    glViewport(0, 0, window_width,window_height);
    //send projection matrix to GPU    
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    while (!modelview.empty()) {
        modelview.pop();
    }

   modelview.push(glm::mat4(1.0));
    
    if (cameraMode == GLOBAL) {

    /*
    modelview.top() = modelview.top()
                    * glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-500.0f),glm::vec3(0.0f,1.0f,0.0f))
                    * glm::inverse(model.getTransform("camera 1")); */

    cout << "pressedX = " << pressedX << ", pressedY = " << pressedY << endl;
    cout << "rotX = " << rotX << ", rotY = " << rotY << endl;

    glm::mat4 r1 = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 r2 = glm::rotate(glm::mat4(1.0f), rotY, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 t2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -50, -200));
    modelview.top() = modelview.top()
                    * glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-500.0f),glm::vec3(0.0f,1.0f,0.0f))
                    * t2 * r2 * r1;

    }
    else if (cameraMode == CHOP) {
        glm::vec3 initialCameraPosition = glm::vec3(0.0f, 300.0f, 2.0f);

        modelview.top() = modelview.top()
                    * glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-500.0f),glm::vec3(0.0f,1.0f,0.0f))
                    * glm::inverse(model.getTransform("camera 2"));
    }
    else if (cameraMode == FPS) {
        modelview.top() = modelview.top() 
                    * glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-500.0f),glm::vec3(0.0f,1.0f,0.0f))
                    * glm::inverse(model.getTransform("camera 3"));
    }
    
    stack<glm::mat4> lighter_modelview;
    lighter_modelview.push(modelview.top());

    // The SceneGraphLighter fills up the lights and lightCoordinateSystem vectors
    // which is then used later in this method.
    vector<util::Light> lights;
    vector<string> lightCoordinateSystems;
    sgraph::SGNodeVisitor *lighter = new sgraph::GLScenegraphLighter(lighter_modelview,lights,lightCoordinateSystems);
    scenegraph->getRoot()->accept(lighter);
    
    initShaderVariables(lights);
    
    for (int i = 0; i < lights.size(); i++) {
        glm::vec4 pos = lights[i].getPosition();
        glUniform4fv(lightLocations[i].position, 1, glm::value_ptr(pos));
    }

    //pass light color properties to shader
    glUniform1i(shaderLocations.getLocation("numLights"),lights.size());
    
    //pass light colors to the shader
    //lights[i].getSpotCutoff() returns the cos of the cutoff angle passed in as calculated in the ScenegraphImporter
    for (int i = 0; i < lights.size(); i++) {
        glUniform3fv(lightLocations[i].ambient, 1, glm::value_ptr(lights[i].getAmbient()));
        glUniform3fv(lightLocations[i].diffuse, 1, glm::value_ptr(lights[i].getDiffuse()));
        glUniform3fv(lightLocations[i].specular, 1,glm::value_ptr(lights[i].getSpecular()));
        glUniform4fv(lightLocations[i].spotDirection, 1,glm::value_ptr(lights[i].getSpotDirection()));
        glUniform1f(lightLocations[i].spotCutoff,lights[i].getSpotCutoff());
    }

    // enables texturing
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shaderLocations.getLocation("image"), 0);

    // TODO *** FIX MIPMAPPING
    if (1 == 1) {
        // Enabling mip mapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);	    
    }

    // Render the Scene and Material
    renderLeavesAndMaterial(scenegraph,model);
    
    modelview.pop();

    glFlush();
    program.disable();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    frames++;
    double currenttime = glfwGetTime();
    if ((currenttime-time)>1.0) {
        //printf("Framerate: %2.0f\r",frames/(currenttime-time));
        frames = 0;
        time = currenttime;
    }
    lightLocations.clear();
}

bool View::shouldWindowClose() {
    return glfwWindowShouldClose(window);
}

void View::closeWindow() {
    for (map<string,util::ObjectInstance *>::iterator it=objects.begin();
           it!=objects.end();
           it++) {
          it->second->cleanup();
          delete it->second;
    } 
    glfwDestroyWindow(window);

    glfwTerminate();
}
 void View::getCursorPosition(double *xpos, double *ypos)
 {
    glfwGetCursorPos(window, xpos, ypos);
 }

 void View::updateCameraType(int val) {
    if (val == 1) {
        cameraMode = GLOBAL;
    }
    else if (val == 2) {
        cameraMode = CHOP;
    }
    else {
        cameraMode = FPS;
    }
 }

 void View::renderLeavesAndMaterial(sgraph::IScenegraph *scenegraph, Model& model) {
    //cout << "GOT TO RENDERLEAVESANDMATERIAL()" << endl;
    sgraph::GLScenegraphRenderer *temp = (sgraph::GLScenegraphRenderer *)renderer;
    temp->setTextureIds(textureIds);
    scenegraph->getRoot()->accept(renderer); 

/*
    for (string name:model.getMeshNames()) {
        modelview.push(modelview.top());  // save the current modelview
        //glm::mat4 transform = model.getAnimationTransform(name) * model.getTransform(name);
        glm::mat4 transform = model.getTransform(name);
        util::Material mat = model.getMaterial(name);
        modelview.top() = modelview.top() * transform;
        
        glm::mat4 normalmatrix = glm::inverse(glm::transpose((modelview.top())));

        glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, false, glm::value_ptr(modelview.top()));
        glUniformMatrix4fv(shaderLocations.getLocation("normalmatrix"), 1, false,glm::value_ptr(normalmatrix));

        glUniform3fv(shaderLocations.getLocation("material.ambient"), 1, glm::value_ptr(mat.getAmbient()));
        glUniform3fv(shaderLocations.getLocation("material.diffuse"), 1, glm::value_ptr(mat.getDiffuse()));
        glUniform3fv(shaderLocations.getLocation("material.specular"), 1,glm::value_ptr(mat.getSpecular()));
        glUniform1f(shaderLocations.getLocation("material.shininess"), mat.getShininess());     
        objects[name]->draw();
        modelview.pop();
    }
*/
 }
