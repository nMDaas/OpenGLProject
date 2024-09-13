#ifndef _GLSCENEGRAPHPRINTER_H_
#define _GLSCENEGRAPHPRINTER_H_

#include "SGNodeVisitor.h"
#include "GroupNode.h"
#include "LeafNode.h"
#include "TransformNode.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "TranslateTransform.h"
#include <ShaderProgram.h>
#include <ShaderLocationsVault.h>
#include "ObjectInstance.h"
#include <stack>
#include <iostream>
#include <LightLocation.h>
#include <Light.h>
using namespace std;

namespace sgraph {
    /**
     * This visitor implements drawing the scene graph using OpenGL
     * 
     */
    class GLSceneGraphPrinter: public SGNodeVisitor {
        public:
        /**
         * @brief Construct a new GLScenegraphPrinter object
         */
        GLSceneGraphPrinter() {
           
        }

        /**
         * @brief Recur to the children for printing
         * 
         * @param groupNode 
         */
        void visitGroupNode(GroupNode *groupNode) {
            string prefix = "";
            for (int i = 0; i < indent; i++) {
                prefix = prefix + "\t";
            }
            prefix = prefix + "- ";
            cout << prefix << (string) groupNode->getName() << endl;
            indent = indent + 1; 
            for (int i=0;i<groupNode->getChildren().size();i=i+1) {
                groupNode->getChildren()[i]->accept(this);
            }
            indent = indent - 1;
        }

        /**
         * @brief Draw the instance for the leaf, after passing the 
         * modelview and color to the shader
         * 
         * @param leafNode 
         */
        void visitLeafNode(LeafNode *leafNode) {
            string prefix = "";
            for (int i = 0; i < indent; i++) {
                prefix = prefix + "\t";
            }
            prefix = prefix + "- ";
            cout << prefix << (string) leafNode->getName() << endl;
        }

        /**
         * @brief Multiply the transform to the modelview and recur to child
         * 
         * @param transformNode 
         */
        void visitTransformNode(TransformNode * transformNode) {
            string prefix = "";
            for (int i = 0; i < indent; i++) {
                prefix = prefix + "\t";
            }
            prefix = prefix + "- ";

            cout << prefix << (string) transformNode->getName() << endl;
            indent = indent + 1;
            if (transformNode->getChildren().size()>0) {
                transformNode->getChildren()[0]->accept(this);
            }
            indent = indent - 1;
        }

        /**
         * @brief For this visitor, only the transformation matrix is required.
         * Thus there is nothing special to be done for each type of transformation.
         * We delegate to visitTransformNode above
         * 
         * @param scaleNode 
         */
        void visitScaleTransform(ScaleTransform *scaleNode) {
            visitTransformNode(scaleNode);
        }

        /**
         * @brief For this visitor, only the transformation matrix is required.
         * Thus there is nothing special to be done for each type of transformation.
         * We delegate to visitTransformNode above
         * 
         * @param translateNode 
         */
        void visitTranslateTransform(TranslateTransform *translateNode) {
            visitTransformNode(translateNode);
        }

        void visitRotateTransform(RotateTransform *rotateNode) {
            visitTransformNode(rotateNode);
        }

        private:
        int indent = 0;
   };
}

#endif