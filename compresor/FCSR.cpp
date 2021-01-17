///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : FCSR.cpp
//  Description :
//      Archivo principal. Contiene el llamado a funciones de com-
//      presion y descompresion. Preferible usar comandos de línea.
//
//  Authors     : E. Rodriguez
//
//  To-do       :
//      Proceso de arbol de huffman
//              [X] Libreria de compresor
// Proceso de compresión
//              [X] Leer el archivo byte a byte
//              [X] Exportar en txt el indice de compresión	
//              [X] Exportar el archivo de salida con bytes sustituidos
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////
#include "./source/compresor.h"

int main(int argc, char** argv){
    if (argc>1){
        std::string ruta = argv[1];
        compresor lectura;

        std::cout << "Inicio" << std::endl;


        lectura.open(ruta);
        lectura.conteo();
        lectura.Crearbol();
        lectura.indexado();
        lectura.comprimir();
        lectura.print_stats();
        lectura.exportar();


        return 1;
    }else{
        std::cout << "Debe incluir ruta de archivo a comprimir" << std::endl;

        return 0;
    }
}