//! [code]

#include <glad/glad.h>
#include "View.h"
#include "Model.h"
#include "Controller.h"

int main(int argc,char *argv[]) {

    if (argc != 2)
        printf("Usage: ./Multicam.exe <input_file>\n");

    std::string input_file = argv[1];
    //std::string input_file = "";

    Model model;
    View view;
    Controller controller(model,view, input_file);
    controller.run();


}

//! [code]
