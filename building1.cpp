//! [code]

#include "Box.h"
#define GLM_FORCE_SWIZZLE
#include "BoxSet.h"
#include <fstream>
using namespace std;
#include "Material.h"

int main(int argc,char *argv[]) {
    BoxSet bset;
    ofstream outStream("output.txt");
    util::Material mat;
    mat.setAmbient(1,1,1);
    mat.setDiffuse(1,1,1);
    mat.setSpecular(1,1,1);
    mat.setShininess(10);
    //start with one box
    bset.add(Box(0,0,0,100,100,100));
    
    
    //create a set of 4 tall windows on the 4 sides of the building 
    //4 x 4 = 16
    for (int i = 0; i <= 3; i ++) {
    bset.difference(Box(15+20*(i), 0,10,10,15,85));
    bset.difference(Box(15+20*(i), 90, 10,10,15,85));
    bset.difference(Box(0, 15+20*(i), 10,10,15,85));
    bset.difference(Box(90, 15+20*(i), 10,10,15,85));
    }

    //write it out to the above file. Using the one-argument version of 
    //this function means it will export the scene graph with each box
    //of a different random color. When we incorporate lighting into the 
    //scene you should regenerate your model using a specific material,
    //by using the two-argument version of this function
    bset.toScenegraph(outStream);

}

//! [code]