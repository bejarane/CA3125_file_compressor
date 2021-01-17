#include <bits/stdc++.h>

///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : estadistica.h
//  Description :
//      Lector de archivos, calculador de frecuencias y generador
//      arbol huffman.
//
//  Authors     : E. Rodriguez
//
//  To-do       :
//              [X] Leer archivo por bytes
//              [X] Clasificar cada byte
//              [X] Crear un diccionario para contar incidencias de cada byte
//              [X] Recorrer el archivo clasificando y actualizando diccionario
//              [X] Ordenar diccionario de mayor a menor
//              [X] Crear arbol binario a partir de diccionario
//              [X] Recorrer arbol resultado
//              [X] Extraer las claves de indice para cada byte
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////

/**
 * Hoja alberga los datos de cada nodo que tiene el arbol de huffman
 */
struct hoja {
    char valor = 0; //se lee byte por byte
    unsigned long int frecuencia = 0;
    hoja *derecha;
    hoja *izquierda;

    //constructor nulo
    hoja(char _valor,unsigned long int _frecuencia){
        valor = _valor;
        frecuencia = _frecuencia;
        derecha = nullptr;
        izquierda = nullptr;
    }

    //constructor puntero
    hoja (char _valor,unsigned long int _frecuencia, hoja* _derecha, hoja* _izquierda){
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

struct comparadorInv{
    bool operator()(hoja* a, hoja* b){
        return a->frecuencia < b->frecuencia;
    }
};

struct arbol {
    hoja *raiz;
    std::vector<int> niveles;
    std::unordered_map<char,unsigned int>index; //la entrada es un byte y el arbol de 
    //maximo tamaño requiere 9 niveles, es decir, 2 bytes de ruta

    arbol(){
        niveles.push_back(0);
    }

    void asignar(){
        if(raiz->derecha == nullptr && raiz->izquierda==nullptr){
            //inserta una ruta
            index[raiz->valor] = 0;
            return;
        }
        niveles[0]=0;
        asignar(raiz->derecha,1,0b10);//la ruta incluye un 1 de más para usar
        //luego en el proceso de compresión 
        asignar(raiz->izquierda,1,0b11);
    }

    void asignar(hoja* nodo, int nivel,unsigned int ruta){
        if (niveles[0]<nivel){
            niveles[0]++; //inicia un nuevo nivel
            niveles.push_back(0);
        }
        niveles[nivel]++; //aumenta contador de nodos
        nivel++;
        
        //guarda una ruta cuando encuentra un nodo sin descendencia
        if(nodo->derecha == nullptr && nodo->izquierda==nullptr){
            //inserta una ruta
            //std::cout<< "ruta: " << std::bitset<9>(ruta)<<std::endl;
            index[nodo->valor] = ruta;
            return;
        }
        asignar(nodo->derecha,nivel,ruta<<1);
        asignar(nodo->izquierda,nivel,(ruta<<1)+1);
    }
};

struct escritor{
    std::ofstream output;
    char buffer=0;
    int bits=0;
    unsigned long int escritos=0;

    escritor(std::string ruta){
        ruta = ruta + ".huff";
        output.open(ruta, std::ios::out | std::ios::binary);
    }

    void guardar(){
        char byte[] = {buffer};
        output.write(byte,1);
        buffer = bits = 0;
        escritos++;
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
            //std::cout<<"missing bits:"<<bits<<std::endl;
            buffer = buffer<<(8-bits);
            guardar();
        }
        output.close();

        std::cout<<"Bytes escritos: "<<escritos<<std::endl;
    }
};

/**
 * Estadistica funciona alrededor de un archivo cualquiera.
 * Su funcion es leer el archivo y categorizar bytes contando su frecuencia
 * Su salida es un vector de frecuencias
 */

struct compresor {
    std::string ruta;
    std::unordered_map<char,unsigned int> registro;
    std::ifstream File;
    std::priority_queue <hoja*,std::vector<hoja*>,comparador> frecuencia;
    std::priority_queue <hoja*,std::vector<hoja*>,comparadorInv> tabla_frecuencia;
    arbol Arbol;
    unsigned long int fileBytes = 0;

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
        
        while(File.read(buffer, 1)){
            registro[buffer[0]]++;
            lectura ++;
        }
        //alcanza EOF
        std::cout << "Bytes leídos: "<< lectura << std::endl;

        fileBytes = lectura;
        File.clear();File.close();

        std::cout << "Tamaño índice: " << registro.size() << std::endl;

        //traversing unordered map https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/
        for (auto nodo : registro){
            hoja* buff_hoja;
            buff_hoja = new hoja(nodo.first,nodo.second);
            frecuencia.push(buff_hoja);
            tabla_frecuencia.push(buff_hoja); //guarda copia
        }
        //el resultado ya está ordenado
    }

    void Crearbol(){
        //std::cout << "frecuencias: "<< frecuencia.size()<<std::endl;

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
        //std::cout << "frecuencias: "<< frecuencia.size()<<std::endl;
        //std::cout << "valor: "<< frecuencia.top()->valor<<std::endl;
        
        Arbol.raiz = frecuencia.top();frecuencia.pop();
    }

    void indexado(){
        Arbol.asignar();
        std::cout<< "Indexu" << Arbol.index.size()<<std::endl;;
        for (auto _libro : Arbol.index){
            std::cout << "Valor: "<< (_libro.first) << " Ruta: " << std::bitset<24>(_libro.second) << std::endl;
        }
    }

    void comprimir(){
        open(ruta);
        escritor salida(ruta);

        char buffer[2];
        long long int lectura = 0;
        while(File.read(buffer, 1)){
            //std::cout << "Leido " << std::bitset<32>(buffer[0]) << std::endl;
            //std::cout << "Comprime a: " << std::bitset<24>(Arbol.index[buffer[0]])<<std::endl;
            int byte = Arbol.index[buffer[0]];
            if (byte == 0){
                std::cout<< "Error"<< std::bitset<16>(buffer[0])<<std::endl;;
            }
            int bit = 0;
            for(bit=0;(byte>>bit)>0;bit++);
            //std::cout<<"bits"<<bit<<std::endl;
            bit-=2;
            while(bit>=0){
                //std::cout<<"bits"<<std::bitset<9>((byte>>bit)&1)<<std::endl;
                salida.escribir((byte>>(bit))&0b1);
                bit--;
            }
            //std::cout << "registro" << registro[buffer[0]] << std::endl;
            lectura ++;
        }
        std::cout<< "Indexu" << Arbol.index.size()<<std::endl;;
        //std::cout << "Leidos: " << lectura << " bytes\n";
        salida.cerrar();
        File.clear();File.close();
    }

    void print_stats(){
        std::ofstream stats(ruta+".stats");
        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";
        stats<<"------Estadísticas de Arbol de Huffman------\n";
        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";
        stats<<'\n';
        stats<<'\n';
        stats<<">>Altura del árbol: "<<Arbol.niveles[0]<<" niveles";
        stats<<'\n';

        int anchura=0;
        for (int nivel : Arbol.niveles){
            if (nivel != Arbol.niveles[0]){
                anchura = nivel > anchura?nivel:anchura;
            }
        }
        stats<<">>Anchura de árbol (max): "<<anchura<<" nodos";
        stats<<'\n';

        stats<<">>Conteo de nodos por nivel\n";
        stats<<"Nivel 0 --> 1 nodos\n";

        for (unsigned int j=1; j < Arbol.niveles.size(); j++){
            stats<<"Nivel "<<j<<" --> "<<Arbol.niveles[j]<<" nodos\n";
        }
        stats<<'\n';

        stats<<">>Tabla de frecuencias\n";
        stats<<"(Valor en hexadecimal)\n";

        while(tabla_frecuencia.size()>0){
            hoja* buffer = tabla_frecuencia.top(); tabla_frecuencia.pop();
            stats<<std::setw(10);
            stats<<std::hex<<buffer->valor<<std::dec;
            //stats<<std::setw(8);
            stats<<"\t"<<buffer->frecuencia<<'\n';
            //no hay que eliminar el apuntado de los punteros por si se requieren luego.
        }
        stats<<'\n';

        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";
        stats<<"--------------Fin del reporte---------------\n";
        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";

        stats.clear();stats.close();
    }

    void exportar(){
        std::ofstream tabla(ruta+".table");
        //int size = 2; //dos bytes para ruta
        //tabla << size <<'\n';
        for (int j = 7; j>=0;j--){
            char s = (fileBytes>>(8*j))&0xff;
            tabla << s;
        }

        for(auto _libro : Arbol.index){//recordar que el index está al revés
            char a = _libro.first;
            char b = (_libro.second>>8)&0xff;
            char c = (_libro.second)&0xff;
            tabla << a << b << c;
        }
        tabla.clear(); tabla.close();
    }
};