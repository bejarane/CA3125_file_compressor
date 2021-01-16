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
//              [] Libreria de estadistica
//              [] Crear arbol binario a partir de diccionario
//              [] Recorrer arbol resultado
//              [] Extraer las claves de indice para cada byte
// Proceso de compresión
//              [] Leer el archivo byte a byte
//              [] Exportar en txt el indice de compresión	
//              [] Exportar el archivo de salida con bytes sustituidos
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////
#include "./source/estadistica.h"

int main(int argc, char** argv){
    if (argc>1){
        std::string ruta = argv[1];
        estadistica lectura;

        std::cout << "Inicio" << std::endl;


        lectura.open(ruta);
        lectura.conteo();
        std::cout<<"Creararbol"<<std::endl;
        lectura.Crearbol();
        lectura.indexado();
        lectura.comprimir();


        return 1;
    }else{
        std::cout << "Debe incluir ruta de archivo a comprimir" << std::endl;

        return 0;
    }
}