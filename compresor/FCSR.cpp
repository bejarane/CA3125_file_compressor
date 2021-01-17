///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : FCSR.cpp
//  Description :
//      Archivo principal. Contiene el llamado a funciones de com-
//      presion y descompresion.
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

int main(int argc, char** argv){//se espera sólo el argumento de ruta
    if (argc>1){//se verifica que se incluya la ruta
        std::string ruta = argv[1];
        //objeto para albergar metodos y propiedades
        compresor lectura;
        //aviso general
        std::cout << "Inicio" << std::endl;
        //inicializa archivo
        lectura.open(ruta);
        //cuenta incidencia de bytes
        lectura.conteo();
        //genera arbol de huffman
        lectura.Crearbol();
        //indexa el arbol de huffman
        lectura.indexado();
        //lee y reemplaza lo bytes el archivo original
        lectura.comprimir();
        //imprime la hoja de datos estadisticos del arbol
        lectura.print_stats();
        //exporta la tabla de indices
        lectura.exportar();

        std::cout << "Completado" << std::endl;
        return 1;
    }else{
        //en caso de no usar argumento
        std::cout << "Debe incluir ruta de archivo a comprimir" << std::endl;
        return 0;
    }
}