#include <bits/stdc++.h>

///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : FileCompreSsoR
//  File        : decompresor.h
//  Description :
//      Descompresor.
//
//  Authors     : E. Rodriguez
//
//  To-do       :
//              [X] Leer el indice de compresion
//              [X] Leer el comprimido por fragmentos
//              [X] Exportar el archivo original al sustituir fragmentos
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////

///TEMPLATE///
/**
 * TITULO
 * DESCRIPCION
 * @param X (TYPE) DESCRIPCION
 * @param X (TYPE) DESCRIPCION
 * @param X (TYPE) DESCRIPCION
 * @return DESCRIPCION
 */

#define DEBUG

/**
 * Objeto alternativo para la hoja original, menos propiedas y metodo propios
*/
struct hojaD{
    char valor = 0; //valor original
    hojaD *derecha;
    hojaD *izquierda;

    /**
     * HOJAD
     * Constructor para hojas vacias
     * @param _valor (char) Valor de la hoja, no necesario
     * @return Sin retorno
     */
    hojaD(char _valor){
        valor = _valor;
        derecha = nullptr;
        izquierda = nullptr;
    }

    /**
     * HOJAD
     * Constructor con todas las propiedades
     * @param _valor (char) Valor de la hoja
     * @param _derecha (hojaD*) Puntero a hoja derecha
     * @param _izquierda (hojaD*) Puntero a hoja izquierda
     */
    hojaD (char _valor, hojaD* _derecha, hojaD* _izquierda){
        valor = _valor;
        derecha = _derecha;
        izquierda = _izquierda;
    }
};

/**
 * Objeto encargado de traducir un archivo comprimido
*/
struct traductor{
    hojaD *raizT; //raiz del arbol de Huffman
    std::string Rsalida; //ruta al archivo de salida
    std::ofstream Fsalida;
    unsigned long int escritos;//contador de bytes escritos
    hojaD *buffer;//puntero movil para lectura del comprimido
    unsigned long int fileBytes;//tamaño real del archivo original
    char longitudruta =0;//bytes requeridos para la ruta

    /**
     * INICIO
     * Inicializa el arbol y el archivo de salida
     * @param _Rsalida (std::string) Ruta del archivo de salida
     * @return Sin retorno
     */
    void inicio(std::string _Rsalida){
        raizT = new hojaD(35);
        escritos = 0;
        Rsalida = _Rsalida;
        Fsalida.open(Rsalida, std::ios::out | std::ios::binary);
    }

    /**
     * CUENTABITS
     * Cuenta la cantidad de bits que contiene una nueva ruta
     * @param ruta (unsigned long int) Ruta binaria
     * @return Sin retorno
     */
    int cuentaBits(unsigned long int ruta){
        int contador; //maximo 64
        for (contador = 0; ruta; contador++){
            ruta = ruta>>1;
        }
        return contador; 
    }

/**
 * AGREGAR
 * Agrega una hoja al arbol y completa la ruta hasta ese punto
 * @param valor (char) Valor real de la hoja
 * @param ruta (unsigned long int) Ruta en binario
 * @return Sin retorno
 */
    void agregar(char valor, unsigned long int ruta){
        int bits = cuentaBits(ruta)-1; //se le resta el 1 precedente que no fue agregado en la compresion
        hojaD* hoja = raizT;//devuelve el puntero movil al comienzo del arbol
        while (bits > 0){//recorre el arbol y lo construye bit a bit
            char bit = (ruta >> (bits-1))&0b1;
            if (bit){//izquierda
                if(hoja->izquierda){
                    hoja = hoja->izquierda;
                }else{
                    hoja->izquierda = new hojaD(35);
                    hoja->derecha = new hojaD(35);
                    hoja = hoja->izquierda;
                }
            }else{//derecha
                if(hoja->derecha){
                    hoja = hoja->derecha;
                }else{
                    hoja->derecha = new hojaD(35);
                    hoja->izquierda = new hojaD(35);
                    hoja = hoja->derecha;
                }
            }
            bits--;
        }
        hoja->valor=valor;
    }

    /**
     * GUARDAR
     * Graba un byte en el archivo de salida
     * @param _buffer (char) Byte a grabar
     * @return Sin retorno
     */
    void guardar(char _buffer){
        char byte[] = {_buffer};//lo inserta en un array
        Fsalida.write(byte,1);//lo graba
        escritos++;
    }

    /**
     * ESCRIBIR
     * Inserta un bit en la cola de lectura
     * Al encontrar un nodo final, graba el byte correspondiente
     * @param _bit (char) Un bit para avanzar en la lectura del arbol
     * @return Sin retorno
     */
    void escribir(char _bit){
        if(escritos >= fileBytes) return;//si ya completó el original, lo demás son
        //0s debido a la completación de bytes
        char bit = _bit&0b1;//asegura que sea solo un bit
        if (bit){//izquierda
            if(buffer->izquierda->izquierda){
                buffer = buffer->izquierda;
            }else{//todos los apuntan a alguna parte, excepto al final
                guardar(buffer->izquierda->valor);
                buffer=raizT;
            }
        }else{//derecha
            if(buffer->derecha->derecha){
                buffer = buffer->derecha;
            }else{//todos los apuntan a alguna parte, excepto al final
                guardar(buffer->derecha->valor);
                buffer=raizT;
            }
        }
    }

    /**
     * CERRAR
     * finaliza el proceso de descompresion y verifica el resultado
     * @return Sin retorno
     */
    void cerrar(){
        Fsalida.clear();Fsalida.close();
        if (fileBytes != escritos){
            std::cout << "Error de descompresion" << std::endl;
        }
    }
};


/**
 * Objeto encargado de gestionar la descompresion y sus métodos
*/
struct decompresor{
    std::string Rcomprimido;
    std::string Rtabla;
    traductor index;

    /**
     * DECOMPRESOR
     * Constructor
     * @param Rc (std::string) Ruta al archivo comprimido
     * @param Rt (std::string) Ruta al archivo de tabla
     * @param Rs (std::string) Ruta al archivo de salida
     * @return Sin retorno
     */
    decompresor(std::string Rc, std::string Rt, std::string Rs){
        Rcomprimido = Rc;
        Rtabla = Rt;
        index.inicio(Rs);
    }

    /**
     * LEERTABLA
     * Leer la tabla de rutas y solicita la creación del arbol
     * @return Sin retorno
     */
    void leertabla(){
        std::ifstream Ftabla;
        Ftabla.open(Rtabla, std::ios::in | std::ios::binary);

        char buffer[12];//mayor a un long long int de 64 bits + 1 byte de valor
        long long int lectura = 0;
        
        Ftabla.read(buffer,8);//lee 8 bytes para tamaño del archivo
        unsigned long int tamano =0;
        tamano |= buffer[0];

        for(int j = 1; j<8;j++){
            tamano = tamano<<8;
            tamano |= buffer[j]&0xff;
        }
        index.fileBytes = tamano;

        #ifdef DEBUG
        std::cout<<"Archivo original: "<<(index.fileBytes)<<" B"<<std::endl;
        #endif

        Ftabla.read(buffer,1);//lee 1 byte para tamaño maximo de ruta
        index.longitudruta = buffer[0];

        #ifdef DEBUG
        std::cout<<"Longitud de ruta: "<<(int)(index.longitudruta)<< " B"<<std::endl;
        #endif

        while(Ftabla.read(buffer, (1+index.longitudruta))){//lee la tabla en grupos
            char valor = buffer[0];//lee el valor original
            unsigned long int ruta = 0;
            char j;
            for (j=0;j<index.longitudruta-1;j++){//une los bytes de la ruta
                ruta |= (buffer[j+1]&0xff);
                ruta = ruta<<8;
            }
            ruta |= (buffer[j+1]&0xff);
            index.agregar(valor,ruta);//integra el nodo al arbol
            lectura+=1+index.longitudruta;//cuenta bytes leidos en tabla
        }
        //alcanza EOF
        Ftabla.clear();Ftabla.close();
    }

    /**
     * DESCOMPRIMIR
     * Procesa la descompresión del archivo
     * @return Sin retorno
     */
    void descomprimir(){
        std::ifstream Fcomprimido;//abre el archivo
        Fcomprimido.open(Rcomprimido, std::ios::in | std::ios::binary);
        index.buffer = index.raizT;//se posiciona al inicio

        char buffer[2];//buffer de lectura, byte por byte
        long long int lectura = 0;//contador de lectura
        
        while(Fcomprimido.read(buffer, 1)){//lee un byte
            int bits = 7;//reinicia bits pendientes
            unsigned int valor = buffer[0];//toma el valor original
            while (bits >= 0){//bit a bit
                int bit = (valor>>(bits))&0b1;//recorta 1 bit
                index.escribir(bit);//avanza el arbol
                bits--;
            }
            lectura++;
        }
        //alcanza EOF
        #ifdef DEBUG
        std::cout << "Comprimido: "<< lectura << " B" << std::endl;
        std::cout << "Escrito: "<< index.escritos<< " B" << std::endl;
        #endif

        index.cerrar();
        Fcomprimido.clear();Fcomprimido.close();


    }

};