#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>
using namespace std;

#include <Eigen/Dense>
using namespace Eigen;

#include "PPM.hpp"

#define _PI 3.14159265358979323846

float degree2rad(float ang)
{
	float r = ang * (float)(_PI / 180.0);

	return r;
}

Matrix3f getMatRotacao(float angulo)
{

	Matrix3f R = Matrix3f::Identity();
	float rad = degree2rad(angulo);
	float cosT = cos(rad);
	float sinT = sin(rad);

	R(0, 0) = cosT;
	R(0, 1) = -sinT;
	R(1, 0) = sinT;
	R(1, 1) = cosT;

	return R;
}

Matrix3f getMatTranslacao(float x, float y)
{

	Matrix3f T = Matrix3f::Identity();

	Vector2f centro = Vector2f(x, y);

	// // translação
	T(0, 2) = centro(0);
	T(1, 2) = centro(1);

	return T;
}

Matrix3f getMatTranslacaoInver(float x, float y)
{

	Matrix3f T = Matrix3f::Identity();

	Vector2f centro = Vector2f(x, y);

	// // translação
	T(0, 2) = -centro(0);
	T(1, 2) = -centro(1);

	return T;
}

Matrix3f getMatCisalhamentoHorizontal(float valor)
{

	Matrix3f T = Matrix3f::Identity();
	T(0, 1) = valor;
	return T;
}
Matrix3f getMatCisalhamentoVertical(float valor)
{

	Matrix3f T = Matrix3f::Identity();
	T(1, 0) = valor;
	return T;
}

Matrix3f getMatReflexaoY()
{

	Matrix3f T = Matrix3f::Identity();
	T(0, 0) = -1;
	return T;
}
Matrix3f getMatReflexaoX()
{

	Matrix3f T = Matrix3f::Identity();

	T(1, 1) = -1;
	return T;
}

Matrix3f getMatEscala(float x, float y)
{

	Matrix3f T = Matrix3f::Identity();

	T(0, 0) = x;
	T(1, 1) = y;
	return T;
}

void OrdenacaoPPM(Matrix3f M, PPM *imgE)
{

	PPM *imgS = new PPM();

	float cx = (imgE->getLargura() - 1) / 2.0;
	float cy = (imgE->getAltura() - 1) / 2.0;

	Matrix3f auxM = getMatTranslacao(cx, cy) * M * getMatTranslacaoInver(cx, cy);
	Matrix3f aux = auxM.inverse();

	imgS->CriaFundo(imgE->getLargura(), imgE->getAltura());
	// ordenacao
	for (int x = 0; x < imgE->getLargura(); x++)
	{
		for (int y = 0; y < imgE->getAltura(); y++)
		{
			Vector3f cS = Vector3f(x, y, 1);
			Vector3f cE = aux * cS;
			cE(0) = round(cE(0));
			cE(1) = round(cE(1));

			if (cE(0) >= 0 && cE(0) < imgE->getLargura() && cE(1) >= 0 && cE(1) < imgE->getAltura())
			{
				unsigned char corR;
				unsigned char corG;
				unsigned char corB;

				imgE->getPixel(cE(0), cE(1), &corR, &corG, &corB);
				imgS->setPixel(cS(0), cS(1), corR, corG, corB);
			}
		}
	}

	imgS->gravar("output.ppm");
	cout << "\n Imagem salva em output.ppm";
}

int convertToASCII(string s)
{
	int j = 0;
	for (int i = 0; i < s.length(); i++)
	{

		j = j + (int)s[i];
		// cout << (int)s[i]<< endl;
	}

	return j;
	// cout << "valor final de J: " << j << endl;
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	PPM *imgE = new PPM();

	string input, inputUser;
	float val = 0, val2 = 0;
	string valChar;
	int valAux;

	//input = "numeros.ppm";
	
	cout << "\n Digite o nome da imagem para entrada com sua extensao (ex: numeros.ppm): ";
	cin >> input;
	fflush(stdin);

	while (!imgE->ler(input))
	{
		cout << "\n Digite o nome da imagem para entrada com sua extensao (ex: numeros.ppm): ";
		cin >> input;
		fflush(stdin);
	}

	Matrix3f M = Matrix3f::Identity();

	do
	{

		cout << "\n Transformacao geometrica (R, T, S, RE, CI ou '0' para sair): ";
		getline(cin, inputUser);
		fflush(stdin);
		// switch entre os valores ASCII para os tipos de transformacao
		switch(convertToASCII(inputUser))
		{
			// rotacao
		case 114:
		case 82:
			cout << "\n Qual o valor da rotação? : ";
			cin >> val;
			fflush(stdin);
			M = getMatRotacao(val) * M;
			OrdenacaoPPM(M, imgE);

			break;

		// translacao
		case 116:
		case 84:
			cout << "\n Qual o valor de x? : ";
			cin >> val;
			fflush(stdin);

			cout << "\n Qual o valor de y? : ";
			cin >> val2;
			fflush(stdin);

			M = getMatTranslacao(val, val2) * M;
			OrdenacaoPPM(M, imgE);
			break;

		// escala
		case 115:
		case 83:
			cout << "\n Qual o valor de x? : ";
			cin >> val;
			fflush(stdin);

			cout << "\n Qual o valor de y? : ";
			cin >> val2;
			fflush(stdin);

			M = getMatEscala(val, val2) * M;
			OrdenacaoPPM(M, imgE);
			break;

		// reflexao
		case 215:
		case 151:
			cout << "\n Reflexão no eixo X ou Y? : ";
			getline(cin, valChar);
			fflush(stdin);
			if(convertToASCII(valChar) == 120 || convertToASCII(valChar) == 88)
				M = getMatReflexaoX() * M;
			else
				M = getMatReflexaoY() * M;

			OrdenacaoPPM(M, imgE);
			break;

		// cisalhamento
		case 204:
		case 140:
			cout << "\n Cisalhamento [h]orizontal ou [v]ertical? : ";
			getline(cin, valChar);
			fflush(stdin);
			if(convertToASCII(valChar) == 104 || convertToASCII(valChar) == 72)
			{
				cout << "Qual o valor do cisalhamento horizontal? : ";
				cin >> val;
				fflush(stdin);
				M = getMatCisalhamentoHorizontal(val) * M;
			}
			else
			{
				cout << "Qual o valor do cisalhamento vertical? : ";
				cin >> val;
				fflush(stdin);
				M = getMatCisalhamentoVertical(val) * M;
			}
			OrdenacaoPPM(M, imgE);
			break;
		}
	} while (inputUser != "0");
}