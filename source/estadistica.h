#include <bits/stdc++.h>

#define SIZE_BLOCK 1


///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : estadistica.h
//  Description :
//      Lector de archivos y calculador de frecuencias.
//
//  Authors     : E. Rodriguez
//
//  To-do       :
//              [X] Leer archivo por bytes
//              [X] Clasificar cada byte
//              [X] Crear un diccionario para contar incidencias de cada byte
//              [X] Recorrer el archivo clasificando y actualizando diccionario
//              [] Ordenar diccionario de mayor a menor
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////

/**
 * Hoja alberga los datos de cada nodo que tiene el arbol de huffman
 */
struct hoja {
    unsigned int valor = 0;
    unsigned long int frecuencia = 0;
    hoja *derecha;
    hoja *izquierda;

    //constructor nulo
    hoja(unsigned int _valor,unsigned long int _frecuencia){
        valor = _valor;
        frecuencia = _frecuencia;
        derecha = nullptr;
        izquierda = nullptr;
    }

    //constructor puntero
    hoja (unsigned int _valor,unsigned long int _frecuencia, hoja* _derecha, hoja* _izquierda){
        valor = _valor;
        frecuencia = _frecuencia;
        derecha = _derecha;
        izquierda = _izquierda;
    }
};

struct comparador{
    bool operator()(hoja* a, hoja* b){
        return a->frecuencia > b->frecuencia;
    }
};

struct arbol {
    hoja *raiz;
    int altura,anchura;
    std::vector<int> niveles;
    std::unordered_map<unsigned long int,unsigned long int>index;

    arbol(){
        niveles.push_back(0);
    }

    void asignar(){
        if(raiz->derecha == nullptr && raiz->izquierda==nullptr){
            //inserta una ruta
            index[raiz->valor] = 0;
            return;
        }
        
        asignar(raiz->derecha,1,0b10);//la ruta incluye un 1 de más para usar
        //luego en el proceso de compresión 
        asignar(raiz->izquierda,1,0b11);
    }

    void asignar(hoja* nodo, int nivel,unsigned long int ruta){
        if (niveles[0]<nivel){
            niveles[0]++; //inicia un nuevo nivel
            niveles.push_back(0);
        }
        niveles[nivel]++; //aumenta contador de nodos
        
        //guarda una ruta cuando encuentra un nodo sin descendencia
        if(nodo->derecha == nullptr && nodo->izquierda==nullptr){
            //inserta una ruta
            //std::cout<< "ruta: " << std::bitset<16>(ruta)<<std::endl;
            index[nodo->valor] = ruta;
            return;
        }
        asignar(nodo->derecha,nivel,ruta<<1);
        asignar(nodo->izquierda,nivel,(ruta<<1)+1);
    }
};

struct escritor{
    std::ofstream output;
    int buffer=0;
    int bits=0;

    escritor(std::string ruta){
        ruta = ruta + ".huff";
        output.open(ruta, std::ios::out | std::ios::binary);
    }

    void guardar(){
        char byte[] = {buffer};
            output.write(byte,SIZE_BLOCK);
            buffer = bits = 0;
    }

    void escribir(bool bit){
        if (bits ==8){
            guardar();
        }
        bits++;
        buffer = (buffer<<1)|bit;
    }

    void cerrar(){
        if (bits>0){
            guardar();
        }
        output.close();
    }
};

/**
 * Estadistica funciona alrededor de un archivo cualquiera.
 * Su funcion es leer el archivo y categorizar bytes contando su frecuencia
 * Su salida es un vector de frecuencias
 */

struct estadistica {
    std::string ruta;
    std::unordered_map<unsigned int,unsigned long int> registro;
    std::ifstream File;
    std::priority_queue <hoja*,std::vector<hoja*>,comparador> frecuencia;
    std::priority_queue <hoja*,std::vector<hoja*>,comparador> tabla_frecuencia;
    arbol Arbol;

    void open(std::string _ruta){
        ruta = _ruta;
        File.open(ruta, std::ios::in | std::ios::binary);
    }

    void conteo (){ //https://www.cplusplus.com/reference/fstream/ifstream/open/
        char buffer[2];
        long long int lectura = 0;
        
        /**
         * para escribir https://courses.cs.vt.edu/~cs2604/fall00/binio.html
        */
       //std::ofstream output;
       //output.open("out_test.jpg", std::ios::out | std::ios::binary); 
        
        while(File.read(buffer, SIZE_BLOCK)){
            //std::cout << "Leido " << std::bitset<32>(buffer[0]) << std::endl;
            registro[buffer[0]]++;
            //std::cout << "registro" << registro[buffer[0]] << std::endl;
            //output.write(buffer,SIZE_BLOCK);
            lectura ++;
        }
        //alcanza EOF
        std::cout << "Bytes leídos: "<< lectura << std::endl;
        File.clear();
        File.close();
        //output.close();

        std::cout << "Map size: " << registro.size() << std::endl;


        //traversing unordered map https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/
        for (auto nodo : registro){
            hoja* buff_hoja;
            //std::cout << "conteo" << nodo.second << std::endl;
            buff_hoja = new hoja(nodo.first,nodo.second);
            frecuencia.push(buff_hoja);
            tabla_frecuencia.push(buff_hoja); //guarda copia
            //std::cout << "conteo" << buff_hoja->frecuencia << std::endl;
        }

        //el resultado ya está ordenado
    }

    void Crearbol(){
        std::cout << "frecuencias: "<< frecuencia.size()<<std::endl;

        while(frecuencia.size() > 1){
            //std::cout << "Popper" << frecuencia.size() << std::endl;
            hoja *buffer0 = frecuencia.top(); frecuencia.pop();
            hoja *buffer1 = frecuencia.top(); frecuencia.pop();
            int suma = buffer0->frecuencia + buffer1->frecuencia;
            //std::cout << "frecuenciaA" << buffer0->frecuencia;
            //std::cout << "frecuenciaB" << buffer1->frecuencia << std::endl;
            //std::cout << "suma" << suma << std::endl;
            hoja *buffer2 = new hoja (0,suma,buffer0,buffer1);
            frecuencia.push(buffer2);
        }
        std::cout << "frecuencias: "<< frecuencia.size()<<std::endl;
        std::cout << "valor: "<< frecuencia.top()->valor<<std::endl;
        
        Arbol.raiz = frecuencia.top();frecuencia.pop();
    }

    void indexado(){
        Arbol.asignar();

        for (auto _libro : Arbol.index){
            std::cout << "Valor: "<< std::bitset<24>(_libro.first) << " Ruta: " << std::bitset<24>(_libro.second) << std::endl;
        }
    }

    void comprimir(){
        open(ruta);
        escritor salida(ruta);

        char buffer[2];
        long long int lectura = 0;
        while(File.read(buffer, SIZE_BLOCK)){
            //std::cout << "Leido " << std::bitset<32>(buffer[0]) << std::endl;
            //std::cout << "Comprime a: " << std::bitset<24>(Arbol.index[buffer[0]])<<std::endl;
            int byte = Arbol.index[buffer[0]];
            while(byte){
                salida.escribir(byte&1);
                byte=byte>>1;
            }
            //std::cout << "registro" << registro[buffer[0]] << std::endl;
            lectura ++;
        }
        std::cout << "Leidos: " << lectura << " bytes\n";
        salida.cerrar();
        File.clear();File.close();
    }
};