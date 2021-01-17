///////// 2021. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileDeCompreSsoR
//  File        : FDCSR.cpp
//  Description :
//      Archivo principal. Contiene el llamado a funciones de descompresion.
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

        lectura.leertabla();//genera arbol de huffman a partir de tabla
        lectura.descomprimir();//descomprime el archivo

        std::cout << "Completado" << std::endl;

        return 1;
    }else{
        std::cout << "Debe incluir ruta de archivo a descomprimir" << std::endl;
        return 0;
    }
}