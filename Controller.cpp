#include "Controller.h"
#include "sgraph/IScenegraph.h"
#include "sgraph/Scenegraph.h"
#include "sgraph/GroupNode.h"
#include "sgraph/LeafNode.h"
#include "sgraph/ScaleTransform.h"
#include "ObjImporter.h"
using namespace sgraph;
#include <iostream>
using namespace std;

#include "sgraph/ScenegraphExporter.h"
#include "sgraph/ScenegraphImporter.h"

#include "sgraph/GLScenegraphPrinter.h"

const float ROT_DELTA = 0.5f;
const float POS_DELTA = 0.1f;

Controller::Controller(Model& m,View& v, std::string input_file) {
    rotXDelta = 0.0f;
    rotYDelta = 0.0f;
    posDelta = 0.0f;
    model = m;
    view = v;
    time = 0.0f;
    createModels();
    initScenegraph(input_file);
}

void Controller::initScenegraph(std::string input_file) {

     
    
    //read in the file of commands
    ifstream inFile(input_file);
    //ifstream inFile("scenegraphmodels/earth.txt");
    sgraph::ScenegraphImporter importer;
    

    IScenegraph *scenegraph = importer.parse(inFile);
    //scenegraph->setMeshes(meshes);
    model.setScenegraph(scenegraph);
    cout <<"Scenegraph made" << endl;   

}

Controller::~Controller()
{
    
}

void Controller::createModels() {
    
    util::PolygonMesh<VertexAttrib> mesh;
    glm::mat4 transform;
    util::Material mat;

    /*
    ifstream in;
    in.open("models/sphere.obj");
    mesh = util::ObjImporter<VertexAttrib>::importFile(in, false);
    mat.setAmbient(0.3f, 0.3f, 0.3f);
    mat.setDiffuse(0.7f, 0.7f, 0.7f);
    mat.setSpecular(0.7f, 0.7f, 0.7f);
    mat.setShininess(100);

    transform = glm::translate(glm::mat4(1.0),glm::vec3(-70.0f,0.0f,0.0f)) 
                * glm::scale(glm::mat4(1.0),glm::vec3(50.0f,50.0f,50.0f));
    model.addMesh("sphere1",mesh,mat,transform);
    transform = glm::translate(glm::mat4(1.0),glm::vec3(70.0f,0.0f,0.0f)) 
                * glm::scale(glm::mat4(1.0),glm::vec3(50.0f,50.0f,50.0f));
    model.addMesh("sphere2",mesh,mat,transform);

    in.close();
    */

    // CAMERA 1
    mesh = getCameraMesh();
    mat.setAmbient(0.066, 0.505, 0.145);  // only this one is used currently to determine color
    mat.setDiffuse(1, 1, 0);
    mat.setSpecular(1, 1, 0);
    transform = transform = glm::inverse(glm::lookAt(glm::vec3(0.0f, 300.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
     * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 6.0f));
    model.addMesh("camera 1",mesh,mat,transform);

    // CAMERA 2
    mat.setAmbient(0.066, 0.505, 0.145);  // only this one is used currently to determine color
    mat.setDiffuse(1, 1, 0);
    mat.setSpecular(1, 1, 0);
    glm::vec4 maxB = mesh.getMaximumBounds();
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    transform = glm::rotate(glm::mat4(1.0), glm::radians(90.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 10.0f, 6.0f)) *
                glm::rotate(glm::mat4(1.0), glm::radians(180.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::translate(glm::mat4(1.0), glm::vec3(9.0f, 0.0f, 45.0f));
    transform = transform * translation;
    model.addMesh("camera 2",mesh,mat,transform);

    // CAMERA 3
    mat.setAmbient(0.066, 0.505, 0.145);  // only this one is used currently to determine color
    mat.setDiffuse(1, 1, 0);
    mat.setSpecular(1, 1, 0);
    transform = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
    * glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 10.0f, 6.0f));
    model.addMesh("camera 3",mesh,mat,transform);
}

void Controller::run()
{
    sgraph::IScenegraph * scenegraph = model.getScenegraph();
    view.init(this,model);

    // print textual representation of scenegraph before displaying with the view
    //SGNodeVisitor* textPrinter = new sgraph::ScenegraphTextPrinter(std::cout);
    //scenegraph->getRoot()->accept(textPrinter);

    while (!view.shouldWindowClose()) {
        float rX = model.getXAxisRotation();
        float rY = model.getYAxisRotation();
        model.changeCam3RotYAngle(rotYDelta);
        model.changeCam3RotXAngle(rotXDelta);
        model.changeCam3Pos(posDelta);
        time = time + 0.01;
        model.animateCam2();
        view.display(scenegraph, model, time, rX, rY);
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}
void Controller::onkey(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R) {
         model.resetCam3();
     }
     else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
        view.updateCameraType(1);
    }
     else if (key == GLFW_KEY_2 && action == GLFW_PRESS){
        view.updateCameraType(2);
    }
     else if (key == GLFW_KEY_3 && action == GLFW_PRESS){
        view.updateCameraType(3);
    }
    else if ((key == GLFW_KEY_KP_ADD) || key == GLFW_KEY_EQUAL){
        if (action == GLFW_PRESS)
            posDelta = -POS_DELTA;
        else if (action == GLFW_RELEASE)
            posDelta = 0.0f;
        
    }
    else if ((key == GLFW_KEY_MINUS)){
        if (action == GLFW_PRESS)
            posDelta = POS_DELTA;
        else if (action == GLFW_RELEASE)
            posDelta = 0.0f;
        
    }
    else if ((key == GLFW_KEY_LEFT)){
        if (action == GLFW_PRESS)
            rotYDelta = ROT_DELTA;
        else if (action == GLFW_RELEASE)
            rotYDelta = 0.0f;
        
    }
    else if ((key == GLFW_KEY_RIGHT)){
        if (action == GLFW_PRESS)
            rotYDelta = -ROT_DELTA;
        else if (action == GLFW_RELEASE)
            rotYDelta = 0.0f;
        
    }
    else if ((key == GLFW_KEY_UP)){
        if (action == GLFW_PRESS)
            rotXDelta = -ROT_DELTA;
        else if (action == GLFW_RELEASE)
            rotXDelta = 0.0f;
        
    }
    else if ((key == GLFW_KEY_DOWN)){
        if (action == GLFW_PRESS)
            rotXDelta = ROT_DELTA;
        else if (action == GLFW_RELEASE)
            rotXDelta = 0.0f;
        
    }
}

void Controller::onmouse(int button, int action, int mods)
{
}

void Controller::oncursor(double xpos, double ypos)
{
}

void Controller::reshape(int width, int height) 
{
    //std::cout <<"Window reshaped to width=" << width << " and height=" << height << std::endl;
    glViewport(0, 0, width, height);
}

void Controller::dispose()
{
    view.closeWindow();
}

void Controller::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// creates mesh for a camera frustum
util::PolygonMesh<VertexAttrib> Controller::getCameraMesh() {
    vector<glm::vec4> positions;

    /*
    glm::vec4 vec = glm::vec4(1.0, 1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(1.0, -1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(1.0, -1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-1.0, -1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-1.0, -1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-1.0, 1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-1.0, 1.0, 0.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(1.0, 1.0, 0.0, 1.0);
    positions.push_back(vec);
    */

    // making first bigger square
    glm::vec4 vec = glm::vec4(0.5, 0.5, -0.5, 1.0); // line 1
    positions.push_back(vec);
    vec = glm::vec4(-0.5, 0.5, -0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.5, 0.5, -0.5, 1.0); // line 2
    positions.push_back(vec);
    vec = glm::vec4(-0.5, -0.5, -0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.5, -0.5, -0.5, 1.0); // line 3
    positions.push_back(vec);
    vec = glm::vec4(0.5, -0.5, -0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(0.5, -0.5, -0.5, 1.0); // line 4
    positions.push_back(vec);
    vec = glm::vec4(0.5, 0.5, -0.5, 1.0);
    positions.push_back(vec);
    
    // making second smaller square
    vec = glm::vec4(0.3, 0.3, 0.5, 1.0); // line 1
    positions.push_back(vec);
    vec = glm::vec4(-0.3, 0.3, 0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.3, 0.3, 0.5, 1.0); // line 2
    positions.push_back(vec);
    vec = glm::vec4(-0.3, -0.3, 0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.3, -0.3, 0.5, 1.0); // line 3
    positions.push_back(vec);
    vec = glm::vec4(0.3, -0.3, 0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(0.3, -0.3, 0.5, 1.0); // line 4
    positions.push_back(vec);
    vec = glm::vec4(0.3, 0.3, 0.5, 1.0);
    positions.push_back(vec);
    
    // making diagonal lines
    vec = glm::vec4(0.5, 0.5, -0.5, 1.0); // line 1
    positions.push_back(vec);
    vec = glm::vec4(0.3, 0.3, 0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.5, 0.5, -0.5, 1.0); // line 2
    positions.push_back(vec);
     vec = glm::vec4(-0.3, 0.3, 0.5, 1.0); 
    positions.push_back(vec);
    vec = glm::vec4(-0.5, -0.5, -0.5, 1.0); // line 3
    positions.push_back(vec);
    vec = glm::vec4(-0.3, -0.3, 0.5, 1.0); 
    positions.push_back(vec);
    vec = glm::vec4(0.5, -0.5, -0.5, 1.0); // line 4
    positions.push_back(vec);
    vec = glm::vec4(0.3, -0.3, 0.5, 1.0); 
    positions.push_back(vec);

    // frustum
    vec = glm::vec4(0.0, 0.0, 1.0, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(0.0, 0.0, -0.5, 1.0);
    positions.push_back(vec);
    vec = glm::vec4(-0.5, 0.5, -0.5, 1.0);
    positions.push_back(vec);
    

    return createMeshFromPositions(positions);
}

// creates mesh from positions vector given
util::PolygonMesh<VertexAttrib> Controller::createMeshFromPositions(vector<glm::vec4> positions) {
    // get vertex data by passing in positions vector
    vector<VertexAttrib> vertexData = createVertexData(positions);
    
    // generate indices vector 
    vector<unsigned int> indices;
    for (int i=0;i<positions.size();i++)
    {
        indices.push_back(i);
    }

    // create mesh
    util::PolygonMesh<VertexAttrib> mesh;

    // give mesh vertex data
    mesh.setVertexData(vertexData);

    // give mesh index data that forms polygons
    mesh.setPrimitives(indices);

    mesh.setPrimitiveType(GL_LINES); 
    mesh.setPrimitiveSize(2);  // 2 vertices for each GL_LINE_STRIP

    return mesh;
}

vector<VertexAttrib> Controller::createVertexData(vector<glm::vec4> positions){
    vector<VertexAttrib> vertexData;
    for (unsigned int i=0;i<positions.size();i++) {
        VertexAttrib v;
        vector<float> data;

        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        data.push_back(positions[i].w);
        v.setData("position",data);
        vertexData.push_back(v);
    }
    return vertexData;
}