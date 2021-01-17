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
//              [] Leer el indice de compresion
//              [] Leer el comprimido por fragmentos
//              [] Exportar el archivo original al sustituir fragmentos
//
//  Git repository: https://github.com/bejarane/CA3125_file_compressor
////////////////////////////////////////////////////////////////////
struct hojaD{
    char valor = 0; //se lee byte por byte
    hojaD *derecha;
    hojaD *izquierda;

    //constructor nulo
    hojaD(char _valor){
        valor = _valor;
        derecha = nullptr;
        izquierda = nullptr;
    }

    //constructor puntero
    hojaD (char _valor, hojaD* _derecha, hojaD* _izquierda){
        valor = _valor;
        derecha = _derecha;
        izquierda = _izquierda;
    }
};


struct traductor{
    hojaD *raizT;
    std::string Rsalida;
    std::ofstream Fsalida;
    unsigned long int escritos;
    hojaD *buffer;
    unsigned long int fileBytes;
    char longitudruta =0;

    void inicio(std::string _Rsalida){
        raizT = new hojaD(35);
        escritos = 0;
        //std::cout<<"val0"<<raizT->valor<<std::endl;
        Rsalida = _Rsalida;
        Fsalida.open(Rsalida, std::ios::out | std::ios::binary);
    }

    //contador de bits http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
    /**
     * Cuenta la cantidad de bits encendidos dentro de una máscara de bits
     * @param ruta entero que contiene la máscara de bits
     * @return entero, la cantidad de bits encendidos dentro de la máscara
     */
    int cuentaBits(unsigned long int ruta){
        int contador; //maximo 64
        for (contador = 0; ruta; contador++){
            ruta = ruta>>1; // clear the least significant bit set
        }
        return contador; 
    }

    void agregar(char valor, unsigned long int ruta){
        int bits = cuentaBits(ruta)-1; //se le resta el 1 que no fue agregado en la compresion
        hojaD* hoja = raizT;
        //std::cout<<"agregar"<<std::bitset<9>(ruta)<<"\t";
        //std::cout<<"valor"<<valor<<bits<<std::endl;
        while (bits > 0){
            char bit = (ruta >> (bits-1))&0b1;
            //std::cout<<"bit"<<std::bitset<1>(bit)<<std::endl;
            if (bit){//izquierda
                if(hoja->izquierda){
                    //std::cout<<"viejo"<<std::bitset<2>(bit)<<std::endl;
                    hoja = hoja->izquierda;
                }else{
                    //std::cout<<"nuevo"<<std::bitset<2>(bit)<<std::endl;
                    hoja->izquierda = new hojaD(35);
                    hoja->derecha = new hojaD(35);
                    hoja = hoja->izquierda;
                }
            }else{//derecha
                if(hoja->derecha){
                    //std::cout<<"viejo"<<std::bitset<2>(bit)<<std::endl;
                    hoja = hoja->derecha;
                }else{
                    //std::cout<<"nuevo"<<std::bitset<2>(bit)<<std::endl;
                    hoja->derecha = new hojaD(35);
                    hoja->izquierda = new hojaD(35);
                    hoja = hoja->derecha;
                }
            }
            bits--;
        }
        hoja->valor=valor;
    }

    void guardar(char _buffer){
        //std::cout<<"Byte guardado"<<_buffer<<std::endl;
        char byte[] = {_buffer};
        Fsalida.write(byte,1);
        escritos++;
    }

    void escribir(char _bit){
        if(escritos >= fileBytes) return;
        char bit = _bit&0b1;
        //std::cout<<"dato"<<buffer->derecha->valor<<std::endl;
        if (bit){//izquierda
            if(/*buffer->izquierda->derecha&&*/buffer->izquierda->izquierda){
                buffer = buffer->izquierda;
            }else{//todos los apuntan a alguna parte, excepto al final
                guardar(buffer->izquierda->valor);
                buffer=raizT;
            }
        }else{//derecha
            if(buffer->derecha->derecha/*&&buffer->derecha->izquierda*/){
                buffer = buffer->derecha;
            }else{//todos los apuntan a alguna parte, excepto al final
                guardar(buffer->derecha->valor);
                buffer=raizT;
            }
        }
    }

    void cerrar(){
        Fsalida.clear();Fsalida.close();
        if (fileBytes != escritos){
            std::cout << "Error de descompresion" << std::endl;
        }
    }
};

struct decompresor{
    std::string Rcomprimido;
    std::string Rtabla;

    traductor index;

    decompresor(std::string Rc, std::string Rt, std::string Rs){
        Rcomprimido = Rc;
        Rtabla = Rt;
        index.inicio(Rs);
        //std::cout<<"Inicio index"<<std::endl;
    }

    void leertabla(){
        std::ifstream Ftabla;
        Ftabla.open(Rtabla, std::ios::in | std::ios::binary);

        char buffer[12];//mayor a un long long int de 64 bits + 1 byte de valor
        long long int lectura = 0;
        
        Ftabla.read(buffer,8);
        unsigned long int tamano =0;
        //std::cout<<"tamano archivo"<<std::bitset<32>(tamano)<<std::endl;
        tamano |= buffer[0];
        for(int j = 1; j<8;j++){
            tamano = tamano<<8;
            tamano |= buffer[j]&0xff;
            //std::cout<<"tamano archivo"<<std::bitset<64>(tamano)<<std::endl;
        }
        index.fileBytes = tamano;
        std::cout<<"Archivo original: "<<(index.fileBytes)<<" B"<<std::endl;

        Ftabla.read(buffer,1);
        index.longitudruta = buffer[0];
        std::cout<<"Longitud de ruta: "<<(int)(index.longitudruta)<< " B"<<std::endl;


        while(Ftabla.read(buffer, (1+index.longitudruta))){
            char valor = buffer[0];
            unsigned long int ruta = 0;
            char j;
            for (j=0;j<index.longitudruta-1;j++){
                ruta |= (buffer[j+1]&0xff);
                ruta = ruta<<8;
            }
            ruta |= (buffer[j+1]&0xff);
            index.agregar(valor,ruta);
            lectura+=1+index.longitudruta;
        }
        //alcanza EOF
        //std::cout<<"val leido"<<index.raizT->derecha->valor<<std::endl;

        //std::cout << "Bytes leídos tabla: "<< lectura << std::endl;
        Ftabla.clear();Ftabla.close();
    }

    void descomprimir(){
        std::ifstream Fcomprimido;
        Fcomprimido.open(Rcomprimido, std::ios::in | std::ios::binary);
        index.buffer = index.raizT;
        //std::cout << Rcomprimido <<std::endl;

        char buffer[2];
        long long int lectura = 0;
        
        while(Fcomprimido.read(buffer, 1)){
            int bits = 7;
            unsigned int valor = buffer[0];
            //std::cout<<"bitentry" << std::bitset<8>(valor)<<std::endl;
            while (bits >= 0){
                int bit = (valor>>(bits))&0b1;
                //std::cout<<"bit" << std::bitset<1>(bit)<<std::endl;
                index.escribir(bit);
                bits--;
            }
            lectura++;
        }
        //alcanza EOF
        std::cout << "Comprimido: "<< lectura << " B" << std::endl;
        std::cout << "Escrito: "<< index.escritos<< " B" << std::endl;
        index.cerrar();
        Fcomprimido.clear();Fcomprimido.close();


    }

};