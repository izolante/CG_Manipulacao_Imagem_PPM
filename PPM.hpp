#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED

#include <string>
#include <fstream>

class PPM
{
public:
    PPM();
    ~PPM();

    bool ler(std::string caminho);
    bool gravar(std::string caminho);

    //retorna o vetor de pixels por referência! As alteraçõe realizadas fora da classe serão gravadas no vetor!
    unsigned char* & getPixels();

    void Quadro(int x1, int y1, int x2, int y2,unsigned char corR, unsigned char corG, unsigned char corB);
    void CriaFundo(int x, int y);
    void PegaQuadro(int x1, int y1, int x2, int y2);
    void LinhaAleatoria(int x, int y, unsigned char corR, unsigned char corG, unsigned char corB);
    void getPixel(int x, int y, unsigned char *corR, unsigned char *corG, unsigned char *corB);

    void setPixel(int x, int y, unsigned char corR, unsigned char corG, unsigned char corB);

    int getLargura();
    int getAltura();


private:
    bool lerDado(std::ifstream &arq, std::string *dado);

    std::string tipo;
	int larg;
	int alt;
	int vmax;
	unsigned char* pixels; // unsigned char -> 0 a 255

};

#endif