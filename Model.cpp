#include "Model.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const float CHOPPER_DELTA = 0.2f;

Model::Model() {
    cam3Position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    cam3Rotation = glm::mat4(1.0f);
}

Model::~Model() {
    if (scenegraph) {
        delete scenegraph;
    }
}

void Model::addMesh(const string& name,util::PolygonMesh<VertexAttrib>& mesh,util::Material& mat,glm::mat4& transformation) {
    meshes[name] = mesh;
    materials[name] = mat;
    transformations[name] = transformation;
    animation_transformations[name] = glm::mat4(1.0f);
}

const util::PolygonMesh<VertexAttrib>& Model::getMesh(const string& name) {
    return meshes[name];
}

sgraph::IScenegraph *Model::getScenegraph() {
    return this->scenegraph;
}

void Model::setScenegraph(sgraph::IScenegraph *scenegraph) {
    this->scenegraph = scenegraph;
}

const glm::mat4 Model::getAnimationTransform(const string& name) {
    return glm::mat4(animation_transformations[name]);
}

vector<string> Model::getTextureNames() {
    return scenegraph->getTextureNames();
}

util::TextureImage* Model::getTextureObject(const string& name) {
    return scenegraph->getTextureObject(name);
}

const util::Material& Model::getMaterial(const string& name) {
    return materials[name];
}

const glm::mat4 Model::getTransform(const string& name) {
    return glm::mat4(transformations[name]);
}

vector<string> Model::getMeshNames() {
    vector<string> names;

    for (auto const& mapentry:meshes) {
        names.push_back(mapentry.first);
    }
    return names;
}

void Model::animateCam2() {
    transformations["camera 2"] =
        glm::rotate(glm::mat4(1.0), glm::radians(CHOPPER_DELTA),
                    glm::vec3(0.0f, 1.0f, 0.0f)) 
        * transformations["camera 2"];

}

void Model::changeCam3RotYAngle(float delta) {
    glm::mat4 oldRotation = cam3Rotation;
    cam3Rotation = cam3Rotation * glm::rotate(glm::mat4(1.0f), glm::radians(delta), glm::vec3(0.0f, 1.0f, 0.0f));
    transformations["camera 3"] =
        glm::translate(glm::mat4(1.0f), glm::vec3(cam3Position.x, cam3Position.y, cam3Position.z))
        * cam3Rotation * glm::inverse(oldRotation)
        * glm::translate(glm::mat4(1.0f), glm::vec3(-cam3Position.x, -cam3Position.y, -cam3Position.z))
        * transformations["camera 3"];
}   


void Model::changeCam3RotXAngle(float delta) {
   glm::mat4 oldRotation = cam3Rotation;
    cam3Rotation = cam3Rotation * glm::rotate(glm::mat4(1.0f), glm::radians(delta), glm::vec3(1.0f, 0.0f, 0.0f));
    transformations["camera 3"] =
        glm::translate(glm::mat4(1.0f), glm::vec3(cam3Position.x, cam3Position.y, cam3Position.z))
        * cam3Rotation * glm::inverse(oldRotation)
        * glm::translate(glm::mat4(1.0f), glm::vec3(-cam3Position.x, -cam3Position.y, -cam3Position.z))
        * transformations["camera 3"];
}

 void Model::changeCam3Pos(float val) {
    glm::mat4 newTransform = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, val));
    transformations["camera 3"] = transformations["camera 3"] * newTransform;
    cam3Position = transformations["camera 3"] * glm::vec4(0.0, 0.0, 0.0, 1.0);
 }

void Model::resetCam3() {
    transformations["camera 3"] = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
    * glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 10.0f, 6.0f));
    cam3Position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    cam3Rotation = glm::mat4(1.0f);
}

void Model::changeXAxisRotation(float deltaR) {
    xAxisAngle = xAxisAngle + deltaR;
}

void Model::changeYAxisRotation(float deltaR) {
    yAxisAngle = yAxisAngle + deltaR;
}

float Model::getXAxisRotation() {
    return xAxisAngle;
}

float Model::getYAxisRotation() {
    return yAxisAngle;
}