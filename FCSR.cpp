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
//              [] Leer archivo por bytes
//              [] Clasificar cada byte
//              [] Crear un diccionario para contar incidencias de cada byte
//              [] Recorrer el archivo clasificando y actualizando diccionario
//              [] Ordenar diccionario de mayor a menor
//              [] Crear arbol binario a partir de diccionario
//              [] Recorrer arbol resultado
//              [] Extraer las claves de indice para cada byte
// Proceso de compresión
//              [] Leer el archivo byte a byte
//              [] Exportar en txt el indice de compresión	
//              [] Exportar el archivo de salida con bytes sustituidos
// Proceso de descompresión
//              [] Leer el indice de compresion
//              [] Leer el comprimido por fragmentos
//              [] Exportar el archivo original al sustituir fragmentos
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////