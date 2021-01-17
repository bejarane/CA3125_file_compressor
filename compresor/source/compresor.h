#include <bits/stdc++.h>

///////// 2021. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : compresor.h
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

///TEMPLATE///
/**
 * TITULO
 * DECRIPCION
 * @param X (TYPE) DESCRIPCION
 * @param X (TYPE) DESCRIPCION
 * @param X (TYPE) DESCRIPCION
 * @return DECRIPCION
 */

#define DEBUG


/**
 * Un objeto de tipo hoja alberga los datos correspondientes a cada
 * nodo del arbol de huffman. Tambien contiene los metodos necesarios
 * para administrarlo.
 */
struct hoja {
    char valor = 0; //se lee byte por byte
    unsigned long int frecuencia = 0;
    hoja *derecha;
    hoja *izquierda;

    /**
     * HOJA
     * Primer constructor del objeto hoja, requiere dos parametros
     * @param _valor (char) Valor de la hoja
     * @param _frecuencia (unsigned long int) Cantidad de incidencias del valor
     * @return no retorno
     */
    hoja(char _valor,unsigned long int _frecuencia){
        valor = _valor;
        frecuencia = _frecuencia;
        derecha = nullptr;
        izquierda = nullptr;
    }

    /**
     * HOJA
     * Segundo constructor del objeto hoja, ingresa todas las propiedades de una hoja
     * @param _valor (char) Valor de la hoja
     * @param _frecuencia (unsigned long int) Cantidad de incidencias del valor
     * @param _derecha (hoja*) Puntero a la siguiente hoja a la derecha
     * @param _izquierda (hoja*) Puntero a la siguiente hoja a la derecha
     * @return no retorno
     */
    hoja (char _valor,unsigned long int _frecuencia, hoja* _derecha, hoja* _izquierda){
        valor = _valor;
        frecuencia = _frecuencia;
        derecha = _derecha;
        izquierda = _izquierda;
    }
};

/**
 * Objeto requerido para comparar hojas dentro del priority queue
*/
struct comparador{
    /**
     * OPERATOR
     * Compara dos hojas
     * @param a (hoja*) Primer hoja a comparar
     * @param b (hoja*) Segunda hoja a comparar
     * @return (bool) Resultado de la comparación
     */
    bool operator()(hoja* a, hoja* b){
        return a->frecuencia > b->frecuencia;
    }
};

/**
 * Objeto requerido para comparar hojas dentro del priority queue, orden inverso
*/
struct comparadorInv{
    /**
     * OPERATOR
     * Compara dos hojas
     * @param a (hoja*) Primer hoja a comparar
     * @param b (hoja*) Segunda hoja a comparar
     * @return (bool) Resultado de la comparación
     */
    bool operator()(hoja* a, hoja* b){
        return a->frecuencia < b->frecuencia;
    }
};

/**
 * Objeto que contiene los métodos y las propiedas necesarias para crear y
 * manipular el Arbol de Hoffman
*/
struct arbol {
    hoja *raiz; //puntero al inicio del arbol
    std::vector<int> niveles; //arreglo para contar nodos por nivel
    std::unordered_map<char,unsigned long int>index; //la entrada es un byte y el arbol de 
    //maximo tamaño esperado es 64 bits
    unsigned long int maxruta=0; //almacena la ruta con má bits (para posterior guardado)

    /**
     * ARBOL
     * Contructor del objeto. Inicializa los niveles
     * @return Sin retorno
     */
    arbol(){
        niveles.push_back(0);
    }

    /**
     * CUENTABYTES
     * Cuenta la cantidad de bytes necesario para representar la ruta de mayor tamaño.
     * @param ruta (unsigned long int) Binario de la ruta
     * @return (char) Cantidad de bytes necesarios para representar. El maximo del programa es de 8 bytes, el char soporta hasta 256.
     */
    char cuentaBytes(unsigned long int ruta){
        char contador;
        for (contador = 0; ruta; contador++){
            ruta = ruta>>8;
        }
        return contador; 
    }

    /**
     * ASIGNAR
     * Inicia el proceso de asignacion, trata de controlar arboles vacios
     * @return No tiene retorno
     */
    void asignar(){
        if(raiz->derecha == nullptr && raiz->izquierda==nullptr){//si arbol vacio
            //inserta una ruta
            index[raiz->valor] = 0;
            return;
        }
        niveles[0]=0;//inicializa los niveles
        //0s para la derecha y 1s para la izquierda
        asignar(raiz->derecha,1,0b10);//la ruta incluye un 1 de más para usar
        //luego en el proceso de compresión 
        asignar(raiz->izquierda,1,0b11);
    }

    /**
     * ASIGNAR
     * Recorre recursivamente el arbol de huffman y asigna rutas
     * @param nodo (hoja*) Puntero de la posición actual 
     * @param nivel (int) nivel actual del nodo consultado
     * @param ruta (unsigned long int) ruta heredada por el nodo
     * @return DECRIPCION
     */
    void asignar(hoja* nodo, int nivel,unsigned long int ruta){
        if (niveles[0]<nivel){//si el nivel es nuevo
            niveles[0]++; //inicia un nuevo nivel
            niveles.push_back(0);
        }
        niveles[nivel]++; //aumenta contador de nodos
        nivel++;//avanza al siguiente nivel
        
        if(nodo->derecha == nullptr && nodo->izquierda==nullptr){//guarda una ruta cuando encuentra un nodo sin descendencia
            maxruta = ruta>maxruta?ruta:maxruta;//guarda ruta maxima
            index[nodo->valor] = ruta;//actualiza ruta en indice
            return;
        }
        asignar(nodo->derecha,nivel,ruta<<1);//continua a la derecha
        asignar(nodo->izquierda,nivel,(ruta<<1)+1);//continua a la izquierda
    }
};

/**
 * Objeto que se encarga de administrar y procesar la escritura del archivo comprimido.
 * Almacena en buffer los bits para agruparlos en bytes y escribirlos como tales.
 * Al cerrar el archivos se asegura de guardar bytes completos
*/
struct escritor{
    std::ofstream output;//objeto del archivo
    char buffer=0;//byte a ecribir
    int bits=0;//bits acumulados
    unsigned long int escritos=0;//total de bytes escritos

    /**
     * ESCRITOR
     * Inicializa el objeto estableciendo la ruta de acceso y abriendo el archivo para lectura
     * @param ruta (std::string) Ruta de acceso al archivo
     * @return Sin retorno
     */
    escritor(std::string ruta){
        ruta = ruta + ".huff";
        output.open(ruta, std::ios::out | std::ios::binary);
    }

    /**
     * GUARDAR
     * Graba un byte en el archivo, reinicia el contador de bits y avanza un byte en el conteo de escritos 
     * @return Sin retorno
     */
    void guardar(){
        char byte[] = {buffer};
        output.write(byte,1);
        buffer = bits = 0;
        escritos++;
    }

    /**
     * ESCRIBIR
     * Evalua si se han completado los bits para escribir.
     * Guarda el byte si esta completo.
     * Almacena en buffer si está incompleto.
     * @param bit (bool) Bit que va a ser escrito
     * @return Sin retorno
     */
    void escribir(bool bit){
        if (bits ==8){
            guardar();
        }
        bits++;
        buffer = (buffer<<1)|bit;
    }

    /**
     * CERRAR
     * Finaliza el proceso de escritura.
     * En caso de tener datos en el buffer, se asignan ceros posteriores para completar el byte
     * @return Sin retorno
     */
    void cerrar(){
        if (bits>0){
            buffer = buffer<<(8-bits);
            guardar();
        }
        output.close();
        #ifdef DEBUG
        std::cout<<"Escritos: "<<escritos << " B"<<std::endl;
        #endif
    }
};

/**
 * Estadistica funciona alrededor de un archivo cualquiera.
 * Su funcion es leer el archivo y categorizar bytes contando su frecuencia
 * Su salida es un vector de frecuencias
 */

struct compresor {
    std::unordered_map<char,unsigned int> registro;//primer contador de bytes
    std::string ruta; //ruta al archivo original
    std::ifstream File; //objeto del archivo
    std::priority_queue <hoja*,std::vector<hoja*>,comparador> frecuencia;//cola para generar huffman
    std::priority_queue <hoja*,std::vector<hoja*>,comparadorInv> tabla_frecuencia;//cola para tabla de frecuencias en reporte
    arbol Arbol;//Arbol de almacenamiento
    unsigned long int fileBytes=0;//longitud total del archivo

    /**
     * OPEN
     * Abri el archivo original para lectura
     * @param _ruta (std::string) ruta al archivo original
     * @return Sin retorno
     */
    void open(std::string _ruta){
        ruta = _ruta;
        File.open(ruta, std::ios::in | std::ios::binary);
    }

    /**
     * CONTEO
     * Cuenta las incidencias de cada byte en el archivo original
     * @return Sin retorno
     */
    void conteo (){
        char buffer[2];//buffer de lectura, 1 byte extra
        long long int lectura = 0;

        while(File.read(buffer, 1)){
            registro[buffer[0]]++;//registra la incidencia
            lectura ++;
        }
        //alcanza EOF
        #ifdef DEBUG
        std::cout << "Leídos: "<< lectura << " B" << std::endl;
        #endif

        fileBytes = lectura; //almacena el conteo
        File.clear();File.close();

        #ifdef DEBUG
        std::cout << "Tamaño del índice: " << registro.size() << " Nodos" << std::endl;
        #endif

        for (auto nodo : registro){//ordena las frecuencias
            hoja* buff_hoja;
            buff_hoja = new hoja(nodo.first,nodo.second);
            frecuencia.push(buff_hoja);
            tabla_frecuencia.push(buff_hoja); //guarda copia para reporte
        }
    }

    /**
     * CREARBOL
     * Cread el arbol de huffman
     * @return Sin retorno
     */
    void Crearbol(){
        while(frecuencia.size() > 1){
            //toma dos hojas
            hoja *buffer0 = frecuencia.top(); frecuencia.pop();
            hoja *buffer1 = frecuencia.top(); frecuencia.pop();
            //suma sus frecuencias
            int suma = buffer0->frecuencia + buffer1->frecuencia;
            //las une en una nueva hoja
            hoja *buffer2 = new hoja (0,suma,buffer0,buffer1);
            //ingresa la hoja de manera ordenada
            frecuencia.push(buffer2);
        }
        //asigna la raiz del arbol al ultimo elemento en la cola
        Arbol.raiz = frecuencia.top();frecuencia.pop();
    }

    /**
     * INDEXADO
     * Utilizado para debuggin, no requerido
     * @return Sin retorno
     */
    void indexado(){
        Arbol.asignar();
        //std::cout<< "Indexu" << Arbol.index.size()<<std::endl;;
        /*for (auto _libro : Arbol.index){
            std::cout << "Valor: "<< std::bitset<12>(_libro.first)<< " Ruta: " << std::hex<<(_libro.second) << std::endl;
        }*/
    }

    /**
     * COMPRIMIR
     * Lee el archivo original y guarda sus datos despues haberlo sustituido con el arbol de huffman
     * @return Sin retorno
     */
    void comprimir(){
        open(ruta);//inicializa lectura
        escritor salida(ruta);//inicializa escritura
        char buffer[2];//buffer de lectura
        long long int lectura = 0;
        while(File.read(buffer, 1)){
            int byte = Arbol.index[buffer[0]];//busca el equivalente de huffman
            if (byte == 0){//no tan necesario
                std::cout<< "Error"<< std::bitset<16>(buffer[0])<<std::endl;;
            }
            int bit = 0;
            for(bit=0;(byte>>bit)>0;bit++);//cuenta la longitud en bits
            bit-=2;
            while(bit>=0){//envia bit a bit para guardar
                salida.escribir((byte>>(bit))&0b1);
                bit--;
            }
            lectura ++;
        }
        salida.cerrar();
        File.clear();File.close();
    }

    /**
     * PRINT_STATS
     * Imprime el informe estadistico del arbol de huffman
     * @return Sin retorno
     */
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
            stats<<"\t"<<buffer->frecuencia<<'\n';
        }
        stats<<'\n';

        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";
        stats<<"--------------Fin del reporte---------------\n";
        stats<<"--------------------------------------------\n";
        stats<<"--------------------------------------------\n";

        stats.clear();stats.close();
    }

    /**
     * EXPORTAR
     * Exporta la tabla de indices en un formato especifico
     * El formato es:
     * 8 bytes para el tamaño del archivo original
     * 1 byte para indicar el tamaño N de las rutas
     * 1 byte para indicar el valor original del nodo
     * N bytes para indicar la ruta del nodo en el arbol de huffman, incluyendo el 1 de precedencia
     * @return Sin retorno
     */
    void exportar(){
        std::ofstream tabla(ruta+".table");
        for (int j = 7; j>=0;j--){
            char s = (fileBytes>>(8*j))&0xff;
            tabla << s;
        }
        
        //calcular tamaño de ruta
        char anchoruta = Arbol.cuentaBytes(Arbol.maxruta);

        #ifdef DEGUB
        std::cout<<"Longitud de ruta: "<<(int)(anchoruta)<< " B"<<std::endl;
        #endif

        tabla << anchoruta; //un byte para guardar los tamaños de ruta

        for(auto _libro : Arbol.index){//recorre el indice y lo almacena
            char a = _libro.first;
            tabla << a;
            for (int j=anchoruta-1; j>0;j--){//separa las rutas en bytes
                char b = (_libro.second>>(8*j))&0xff;
                tabla<<b;
            }
            char c = (_libro.second)&0xff;
            tabla << c;
            
        }
        tabla.clear(); tabla.close();
    }
};