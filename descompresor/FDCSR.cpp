///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileDeCompreSsoR
//  File        : FDCSR.cpp
//  Description :
//      Archivo principal. Contiene el llamado a funciones de descompresion. Preferible usar comandos de línea.
//
//  Authors     : E. Rodriguez
//
//  To-do       :
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////

#include "./source/decompresor.h"

int main(int argc, char** argv){
    if (argc>3){
        std::string rutaC = argv[1];
        std::string rutaT = argv[2];
        std::string rutaS = argv[3];
        decompresor lectura(rutaC,rutaT,rutaS);

        std::cout << "Inicio" << std::endl;
        //std::cout << rutaT << std::endl;

        lectura.leertabla();
        lectura.descomprimir();


        return 1;
    }else{
        std::cout << "Debe incluir ruta de archivo a descomprimir" << std::endl;

        return 0;
    }
}