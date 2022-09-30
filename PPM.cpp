#include "PPM.hpp"

#include <iostream>
using namespace std;

PPM::PPM()
{
    pixels = nullptr;
	larg = 0;
	alt = 0;
	tipo = "";
}

PPM::~PPM() {
    if (pixels)
        delete pixels;
    pixels = nullptr;
}


int PPM::getLargura()
{
    return larg;
}

int PPM::getAltura()
{
    return alt;
}

unsigned char* & PPM::getPixels()
{
    return pixels;
}

void PPM::getPixel(int x, int y, unsigned char *corR, unsigned char *corG, unsigned char *corB)
{
    if (!pixels) //Se "pixels==NULL" retorna
        return;

    *corR = pixels[y*larg*3 + x*3]; //cor do canal R para pixels de coordenada (x, y)
    *corG = pixels[y*larg*3 + x*3 + 1];
    *corB = pixels[y*larg*3 + x*3 + 2];
}

void PPM::setPixel(int x, int y, unsigned char corR, unsigned char corG, unsigned char corB)
{
    if (!pixels)//Se "pixels==NULL" retorna
        return;

    pixels[y*larg*3 + x*3] = corR;
    pixels[y*larg*3 + x*3 + 1] = corG;
    pixels[y*larg*3 + x*3 + 2] = corB;
    
        
}


bool PPM::ler(string caminho) {
    ifstream arq;
    string aux;

    

    arq.open(caminho);
    if (!arq.is_open()) {
        cout << "endereco do arquivo invalido!\n";
        return false;
    }

    if (!lerDado(arq, &aux)) {
        cout << "Nao foi possível ler o tipo da imagem!\n";
        return false;
    }
    tipo = aux;


    if (!lerDado(arq, &aux)) {
        cout << "Nao foi possível ler a largura da imagem\n";
        return false;
    }
    larg = atoi(aux.c_str());

    if (!lerDado(arq, &aux)) {
        cout << "Nao foi possível ler a altura da imagem\n";
        return false;
    }
    alt = atoi(aux.c_str());


    if (!lerDado(arq, &aux)) {
        cout << "Nao foi possível ler o valor máximo de cor!\n";
        return false;
    }
    vmax = atoi(aux.c_str());

    if (pixels) //verifica se o vetor de pixels já está alocado (pixels != NULL) e desaloca memória.
        delete pixels;

    //aloca o vetor de pixels    
    pixels = new unsigned char[larg * alt * 3];

    //lê os valores de cor dos pixels
    int i = 0;
    while (!arq.eof()) 
    {
        if (!lerDado(arq, &aux)) {
            break;
        }

        pixels[i] = (unsigned char) atoi(aux.c_str());
        i++;
    }

    //cout << "i: " << i << endl;
    if (i != larg * alt * 3) {
        cout << "A quantidades de pixels lida é inválida!\n";
        return false;
    }

    //cout << this->tipo << endl;
    //cout << this->larg << endl;
    //cout << this->alt << endl;
    //cout << this->vmax << endl;


    return true;
}


bool PPM::gravar(string caminho) {

    if (!pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open()) {
        cout << "Endereco do arquivo invalido!\n";
        return false;
    }

    arq << tipo << endl;

    arq << larg << " " << alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = larg * alt *3 ;
    for (int i = 0; i < tam; i++)
    {
        arq << (int) pixels[i] << endl;
        arq.flush();
    }
        

    arq.close();
    return true;
}


bool PPM::lerDado(ifstream &arq, string *dado) {
    string aux;
    while (!arq.eof()) {
        arq >> aux;
        if (aux.size() > 0 && aux[0] == '#') {
            std::getline(arq, aux);
        }else if(aux.size() > 0){
            *dado = aux;
            return true;
        }

        aux = "";
    }
    return false;
}


void PPM::Quadro(int x1, int y1, int x2, int y2,unsigned char corR, unsigned char corG, unsigned char corB){


    //unsigned char vetorAuxiliar2[alt][larg*3];  //vetor auxiliar para guardar o quadro
    //unsigned char corO = 0;
    //unsigned char cor1 = 0;
    //unsigned char cor2 = 0;


    // imprimir os pixels do vetor
    /*
    for(int y=0;y<alt;y++){
        for(int x=0;x<larg;x++){   //atribui o quadro a imagem
                    getPixel(x,y,&corO,&cor1, &cor2);
               cout<<(int) corO <<" "<<(int)cor1 << " "<< (int)cor2<<" ";
        }  
        cout<<"\n";
        
    }
   */

    //seta os pixel com a cor

        //linha superior
        for(int x=x1;x<=x2;x++){
                setPixel(x,y1,corR,corG,corB);
        }
 
        //inferior    o y é linhas e o x é coluna
        for(int x=x1;x<=x2;x++){
                setPixel(x,y2,corR,corG,corB);
        }

        
        //linha esquerda  coluna x1 de y1 ate y2
        for(int y=y1;y<=y2;y++){
               setPixel(x1,y,corR,corG,corB);
        }

        //linha direita  coluna x2 de y1 ate y2
        for(int y=y1;y<=y2;y++){
               setPixel(x2,y,corR,corG,corB);
        }


    
}


void PPM::CriaFundo(int largura, int altura){

    this->tipo = "P3";
    this->larg = largura;
    this->alt = altura;
    
    pixels =  new unsigned char [larg * alt*3];
    for(int i=0;i<larg *alt*3;i++)
        pixels[i] = 0;
 

}


void PPM::LinhaAleatoria(int alt, int larg, unsigned char corR, unsigned char corG, unsigned char corB){

    //pixels =  new unsigned char [larg * alt*3];

    for(int y=50;y<alt;y+=50){
        for(int x=0;x<larg;x++){
            unsigned char corAleatoriaVermelha = rand()%255;
            unsigned char corAleatoriaVerde = rand()%255;
            unsigned char corAleatoriaAzul = rand()%255;
            pixels[y*3*larg + x*3]= corAleatoriaVermelha;
            pixels[y*larg*3 + x*3 + 1] = corAleatoriaVerde;
            pixels[y*larg*3 + x*3 + 2] = corAleatoriaAzul;
            setPixel(x,y,corAleatoriaVermelha,corAleatoriaVerde,corAleatoriaAzul);
        }
    } 



}


void PPM::PegaQuadro(int x1, int y1, int x2, int y2){



}