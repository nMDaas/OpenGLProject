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
    bset.add(Box(0,0,0,100,200,100));

    // creates 3x3 square windows on the shorter faces of the building
    // 3 x 3 x 2 = 18    
    for (int i = 0; i <= 2; i ++) {
    bset.difference(Box(15+30*(i), 0,60,15,15,15));
    bset.difference(Box(15+30*(i), 185,60,15,15,15));
    bset.difference(Box(15+30*(i), 0,35,15,15,15));
    bset.difference(Box(15+30*(i), 185,35,15,15,15));
    bset.difference(Box(15+30*(i), 0,15,15,15,15));
    bset.difference(Box(15+30*(i), 185,15,15,15,15));
    }

    // creates 1x6 square windows on the longer faces of the building
    // 1 x 6 x 2 = 12
    // creates 1 x 6 tall rectangular windows on the longer faces of the building
    // 1 x 6 x 2 = 12
    for (int i = 0; i <= 5; i ++) {
    bset.difference(Box(0, 15+30*(i), 70,15,15,15));
    bset.difference(Box(90, 15+30*(i), 70,15,15,15));
    bset.difference(Box(0, 15+30*(i), 10,15,15,50));
    bset.difference(Box(90, 15+30*(i), 10,15,15,50));
    }

    // two wide and thin windows on shorter faces of building
    bset.difference(Box(15,0,85,75,15,5));
    bset.difference(Box(15,185,85,75,15,5));

    //write it out to the above file. Using the one-argument version of 
    //this function means it will export the scene graph with each box
    //of a different random color. When we incorporate lighting into the 
    //scene you should regenerate your model using a specific material,
    //by using the two-argument version of this function
    bset.toScenegraph(outStream);

}

//! [code]