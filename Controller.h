#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "View.h"
#include "Model.h"
#include "Callbacks.h"

class Controller: public Callbacks
{
public:
    Controller(Model& m,View& v, std::string input_file);
    ~Controller();
    void run();

    virtual void reshape(int width, int height);
    virtual void dispose();
    virtual void onkey(int key, int scancode, int action, int mods);
    virtual void error_callback(int error, const char* description);
    virtual void onmouse(int button, int action, int mods);
    virtual void oncursor(double xpos, double ypos);
private:
    void createModels();
    void initScenegraph(std::string input_file);

    View view;
    Model model;

    bool mousePressed = false;
    float time;
    util::PolygonMesh<VertexAttrib> getCameraMesh();
    util::PolygonMesh<VertexAttrib> createMeshFromPositions(vector<glm::vec4> positions);
    vector<VertexAttrib> createVertexData(vector<glm::vec4> positions);
    util::PolygonMesh<VertexAttrib> cameraMesh;

    float rotXDelta, rotYDelta, posDelta;
};

#endif