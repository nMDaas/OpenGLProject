#ifndef __MODEL_H__
#define __MODEL_H__

#include "PolygonMesh.h"
#include "VertexAttrib.h"
#include "sgraph/IScenegraph.h"
#include <map>
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Model 
{
public:

    Model();
    ~Model();
    const util::PolygonMesh<VertexAttrib>& getMesh(const string& name);
    sgraph::IScenegraph *getScenegraph();
    void setScenegraph(sgraph::IScenegraph *scenegraph);
    void addMesh(const string& name,util::PolygonMesh<VertexAttrib>& mesh,util::Material& mat,glm::mat4& transformation);
    vector<string> getMeshNames();
    const glm::mat4 getAnimationTransform(const string& name);
    const util::Material& getMaterial(const string& name);
    const glm::mat4 getTransform(const string& name);
    void animateCam2();

    // to make camera 3 turn left and right
    void changeCam3RotYAngle(float delta);
    float getCam3RotYAngle();

    // to make camera 3 turn up and down
    void changeCam3RotXAngle(float delta);
    float getCam3RotXAngle();

    void changeCam3Pos(float val);

    void resetCam3();

    vector<string> getTextureNames();
    util::TextureImage* getTextureObject(const string& name);

    void animateCam2(float time);

    float getXAxisRotation();
    float getYAxisRotation();
    void changeXAxisRotation(float deltaR);
    void changeYAxisRotation(float deltaR);

private:
    map<string,util::PolygonMesh<VertexAttrib> > meshes;
    map<string,util::Material> materials;
    map<string,glm::mat4> transformations;
    map<string,glm::mat4> animation_transformations;
    sgraph::IScenegraph *scenegraph;

    glm::vec4 cam3Position;
    glm::mat4 cam3Rotation;

    float cam3RotYAngle; // to make camera 3 turn left and right
    float cam3RotXAngle; // to make camera 3 turn up and down
    float cam3RotZAngle; 

    float cam3X;
    float cam3Y;
    float cam3Z;

    float xAxisAngle;
    float yAxisAngle;
};
#endif