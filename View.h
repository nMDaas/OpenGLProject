#ifndef __VIEW_H__
#define __VIEW_H__

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Light.h>
#include <cstdio>
#include <ShaderProgram.h>
#include "sgraph/SGNodeVisitor.h"
#include "ObjectInstance.h"
#include "PolygonMesh.h"
#include "LightLocation.h"
#include "VertexAttrib.h"
#include "Callbacks.h"
#include "sgraph/IScenegraph.h"
#include "Model.h"

#include <stack>
using namespace std;


class View
{

    class LightLocation {
    public:
        int ambient,diffuse,specular,position,spotDirection;
        float spotCutoff;
        LightLocation()
        {
        ambient = diffuse = specular = position = spotDirection = spotCutoff = -1;
        }

    };

typedef enum {GLOBAL,FPS,CHOP} TypeOfCamera;

public:

    View();
    ~View();
    void init(Callbacks* callbacks,Model& model);
    void display(sgraph::IScenegraph *scenegraph, Model& model, float time, float rX, float rY);
    bool shouldWindowClose();
    void closeWindow();
    void resetTrackball();

    void updateCameraRotation(float xpos, float ypos, bool buttonPressed);
    void updateCameraType(int val);
    void getCursorPosition(double *xpos, double *ypos);

private: 

    GLFWwindow* window;
    util::ShaderProgram program;
    util::ShaderLocationsVault shaderLocations;
    map<string,util::ObjectInstance *> objects;
    glm::mat4 projection;
    stack<glm::mat4> modelview;
    sgraph::SGNodeVisitor *renderer;
    int frames;
    double time;
    void initObjects(Model& model);
    void initLights(Model& model);
    void initShaderVariables(vector<util::Light>& lights);

    void renderLeavesAndMaterial(sgraph::IScenegraph *scenegraph, Model& model);

    glm::mat4 cameraRotation;
    bool XYinit = false;
    float prevX = 0.0f;
    float prevY = 0.0f;
    float rotdx = 0.0f;
    float rotdy = 0.0f;
    TypeOfCamera cameraMode;

    //the shader locations for all lights, for convenience
    vector<LightLocation> lightLocations;
    map<string,unsigned int> textureIds;
};

#endif
