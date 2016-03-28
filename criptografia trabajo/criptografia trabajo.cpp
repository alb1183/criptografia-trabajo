// criptografia trabajo.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

int mod (int a, int b) {
   if(b < 0)
     return mod(-a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}

void contar_caracteres(char* ruta, int* resultado, char* caracteres, int* total, int clave_long, int stats) {
	ifstream Fichero;
	Fichero.open(ruta);
	if(Fichero.fail()) {
		cout << "No se ha podido abrir el fichero." << endl;
	}

	// Inicializo ambos arrays(caracteres y ceros)
	for(int i = 0; i < 26; i++)	{
		resultado[i] = 0;
		caracteres[i] = 'a' + i;
	}
	
	string linea;
	int count = 0;
	int posicion = 0;
	while(getline(Fichero, linea)) {
		transform(linea.begin(), linea.end(), linea.begin(), ::tolower);
		int linea_long = linea.length();
		for(int i = 0; i < linea_long; i++) {
			if(posicion == stats || stats == -1) {
				if(linea[i] >= 'a' && linea[i] <= 'z') {
					resultado[linea[i] - 'a']++;
					count++;
				}
			}
			posicion = (posicion + 1) % clave_long;
		}
	}
	*total = count;
	Fichero.close();
}

void ordenar_array(char* indices, int* valores) {
	for(int j = 0; j < 27; j++) {
		int max = valores[j];
		char max_i = indices[j];
		int max_p = j;
		for(int i = j+1; i < 28; i++) {
			if(valores[i] > max) {
				max = valores[i];
				max_i = indices[i];
				max_p = i;
			}
		}
		valores[max_p] = valores[j];
		indices[max_p] = indices[j];
		valores[j] = max;
		indices[j] = max_i;
		cout << ".";
	}
	cout << endl;
}

void escribir_tabla(char * caracteres, int* resultado, int total, int modo) { // TODO: Modo(0: relativa, 1: absoluta)
	for(int j = 0; j < 2; j++) {
		for(int i = j*13; i < 13+(j*13); i++)	{
			char letra = caracteres[i];
			cout << left << setw(5) << setfill(' ') << letra;
		}
		cout << endl;
		for(int i = j*13; i < 13+(j*13); i++)	{
			double porcentaje = (modo == 1) ? resultado[i] : (100.0 * resultado[i] / total);
			cout << left << setw(5) << setprecision(1) << fixed << setfill(' ') << porcentaje;
		}
		cout << endl;
	}
}



void cifrar_cesar(char* entrada, char* salida, int desplazamiento) {
	ifstream Fichero;
	ofstream Salida;
	Fichero.open(entrada);
	Salida.open(salida);
	if(Fichero.fail()) {
		cout << "No se ha podido abrir el fichero." << endl;
	}
	string linea;

	cout << "Codificando..." << endl;

	while(getline(Fichero, linea)) {
		transform(linea.begin(), linea.end(), linea.begin(), ::tolower); // Lo dejo todo en minusculas
		int linea_long = linea.length();
		for(int i = 0; i < linea_long; i++) {
			if(linea[i] >= 'a' && linea[i] <= 'z') {
				char letra = 'a' + (((linea[i] - 'a') + desplazamiento) % 26); // Desplazo
				Salida << letra;
			}else{
				Salida << linea[i]; // No desplazo
			}
		}
		Salida << "\n";
	}
			
	cout << "Codificado." << endl;

	Fichero.close();
	Salida.close();
}

void descifrar_cesar(char* entrada, char* salida, int desplazamiento) {
	ifstream Fichero;
	ofstream Salida;
	Fichero.open(entrada);
	Salida.open(salida);
	if(Fichero.fail()) {
		cout << "No se ha podido abrir el fichero." << endl;
	}
	string linea;

	cout << "Descodificando..." << endl;

	while(getline(Fichero, linea)) {
		int linea_long = linea.length();
		for(int i = 0; i < linea_long; i++) {
			if(linea[i] >= 'a' && linea[i] <= 'z') {
				char letra = 'a' + mod((linea[i] - 'a') - desplazamiento, 26); // Desplazo, Modulo especial(cambio los negativos)
				Salida << letra;
			}else{
				Salida << linea[i]; // No desplazo
			}
		}
		Salida << "\n";
	}
			
	cout << "Descodificado." << endl;

	Fichero.close();
	Salida.close();
}

void cifrar_vigenere(char* entrada, char* salida, string clave) {
	ifstream Fichero;
	ofstream Salida;
	Fichero.open(entrada);
	Salida.open(salida);
	if(Fichero.fail()) {
		cout << "No se ha podido abrir el fichero." << endl;
	}
	string linea;

	/*char tabla[26][26]; // Tabla de codificacion
	// Preparo la tabla
	for(int i = 0; i < 26; i++) {
		for(int j = 0; j < 26; j++) {
			tabla[i][j] =  'a' + ((i+j) % 26);
		}
	}*/ // TODO: Cambio de metodo
	
	int clave_long = clave.length();

	cout << "Codificando..." << endl;

	while(getline(Fichero, linea)) {
		transform(linea.begin(), linea.end(), linea.begin(), ::tolower); // Lo dejo todo en minusculas
		int clave_p = 0;
		int linea_long = linea.length();
		for(int i = 0; i < linea_long; i++) {
			if(linea[i] >= 'a' && linea[i] <= 'z') {
				char letra = 'a' + (((linea[i] - 'a') + (clave[clave_p % clave_long] - 'a')) % 26); // Desplazo
				//cout << "cambio:" << linea[i] << " por " << letra << " ya que " << linea[i] - 'a' << " " << clave[clave_p % clave_long] - 'a' << " " << clave[clave_p % clave_long] << endl;
				clave_p++;
				Salida << letra;
			}
		}
		Salida << "\n";
	}
			
	cout << "Codificado." << endl;

	Fichero.close();
	Salida.close();
}

void descifrar_vigenere(char* entrada, char* salida, string clave) {
	ifstream Fichero;
	ofstream Salida;
	Fichero.open(entrada);
	Salida.open(salida);
	if(Fichero.fail()) {
		cout << "No se ha podido abrir el fichero." << endl;
	}
	string linea;
	
	int clave_long = clave.length();

	cout << "Descodificando..." << endl;

	while(getline(Fichero, linea)) {
		int linea_long = linea.length();
		for(int i = 0; i < linea_long; i++) {
			if(linea[i] >= 'a' && linea[i] <= 'z') {
				char letra = 'a' + mod((linea[i] - 'a') - (clave[i % clave_long] - 'a'), 26); // Desplazo, Modulo especial(cambio los negativos)
				Salida << letra;
			}else{
				Salida << linea[i]; // No desplazo
			}
		}
		Salida << "\n";
	}
			
	cout << "Descodificado." << endl;

	Fichero.close();
	Salida.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
	int menu;
	int submenu;

	cout << "Menu: " << endl;
	cout << "1) Cifrar texto" << endl << "2) Descifrar texto" << endl << "3) Romper cifrado" << endl;
	cout << "Opcion: ";
	cin >> menu;
	
	if(menu == 1) {
		system("cls");
		cout << "Metodo de cifrado: " << endl;
		cout << "1) Cesar(desplazamiento)" << endl;
		cout << "2) Vigenere(polialfabetico)" << endl;
		cout << "Opcion: ";
		cin >> submenu;

		if(submenu == 1) {
			system("cls");
			int desplazamiento = 0;

			cout << "Despazamiento: ";
			cin >> desplazamiento;

			cifrar_cesar("texto_claro.txt", "texto_cifrado.txt", desplazamiento);

		}else if(submenu == 2) {
			system("cls");
			string clave;

			cout << "Clave de cifrado: ";
			cin >> clave;
						
			cifrar_vigenere("texto_claro_min.txt", "texto_cifrado.txt", clave);

		}else{
			cout << "Opcion no valida" << endl;
		}
	}else if(menu == 2) {
		system("cls");
		cout << "Metodo de descifrado: " << endl;
		cout << "1) Cesar(desplazamiento)" << endl;
		cout << "2) Vigenere(polialfabetico)" << endl;
		cout << "Opcion: ";
		cin >> submenu;

		if(submenu == 1) {
			system("cls");
			int desplazamiento = 0;

			cout << "Despazamiento: ";
			cin >> desplazamiento;

			descifrar_cesar("texto_cifrado.txt", "texto_descifrado.txt", desplazamiento);
		}else if(submenu == 2) {
			system("cls");
			string clave;

			cout << "Clave de cifrado: ";
			cin >> clave;

			descifrar_vigenere("texto_cifrado.txt", "texto_descifrado.txt", clave);
		}else{
			cout << "Opcion no valida" << endl;
		}
	}else if(menu == 3) {
		system("cls");
		cout << "Metodo a romper: " << endl;
		cout << "1) Cesar(desplazamiento)" << endl;
		cout << "2) Vigenere(polialfabetico)" << endl;
		cout << "Opcion: ";
		cin >> submenu;

		if(submenu == 1) {
			system("cls");
			cout << "La forma para romperlo que voy a usar es la de calcular las frecuencias" << endl;
			cout << "relativas de las letras en español usando un fragmento del Quijote y" << endl;
			cout << "luego las ordeno de mayor a menor, despues hago lo mismo para el texto" << endl;
			cout << "cifrado y comparo las mas usadas y con esas estimo un posible valor" << endl;
			cout << "para el desplazamiento comparando la distancia media de las letras." << endl << endl;

			
			char caracteres[26]; // Array de indices
			int resultado[26];
			int total = 0;
			contar_caracteres("texto_muestreo.txt", resultado, caracteres, &total, 1, -1);
	
			cout << "Total de caracteres contados: " << total << endl; // Es curioso que los 4 primero capitulos del quijote dan exactamente 39000 xD
			cout << "Porcentajes de aparacion de las letras en el texto en claro:" << endl;
			escribir_tabla(caracteres, resultado, total, 0);

			cout << endl << "Las ordeno de mayor a menor y me quedo las cinco mayores." << endl;
			cout << "Ordenando";
			
			ordenar_array(caracteres, resultado);
			cout << "Ordenado" << endl;
			
			cout << "Cinco mayores porcentajes en el texto en claro(sin contar el espacio):" << endl;
			// Array ordenado
			for(int i = 0; i < 6; i++)	{
				char letra = caracteres[i];
				cout << left << setw(5) << setfill(' ') << letra;
			}
			cout << endl;
			for(int i = 0; i < 6; i++)	{
				double porcentaje = (100.0 * resultado[i] / total);
				cout << left << setw(5) << setprecision(1) << fixed << setfill(' ') << porcentaje;
			}
			cout << endl << endl;
			
			cout << "***** Texto Codificado *****" << endl << "Leyendo texto codificado..." << endl;
			char caracteres_c[26]; // Array de indices
			int resultado_c[26]; // el 26 es el espacio y el 27 la ñ
			int total_c = 0;
			contar_caracteres("texto_cifrado.txt", resultado_c, caracteres_c, &total_c, 1, -1);

			cout << "Total de caracteres contados: " << total_c << endl;
			cout << "Porcentajes de aparacion de las letras en el texto codificado:" << endl;
			escribir_tabla(caracteres_c, resultado_c, total_c, 0);

			cout << endl << "Las ordeno de mayor a menor y me quedo las cinco mayores." << endl;
			cout << "Ordenando";
			
			ordenar_array(caracteres_c, resultado_c);
			cout << "Ordenado" << endl;
			
			cout << "Cinco mayores porcentajes en el texto codificado(sin contar el espacio):" << endl;
			// Array ordenado
			for(int i = 0; i < 6; i++)	{
				char letra = caracteres_c[i];
				cout << left << setw(5) << setfill(' ') << letra;
			}
			cout << endl;
			for(int i = 0; i < 6; i++)	{
				double porcentaje = (100.0 * resultado_c[i] / total_c);
				cout << left << setw(5) << setprecision(1) << fixed << setfill(' ') << porcentaje;
			}
			cout << endl;

			cout << "Visto estos resultados vamos a intentar obtener el desplazamiento" << endl;
			cout << "usando las dos primeras letras. Luego la correspondencia seria: " << endl;
			cout << caracteres[0] << "->" << caracteres_c[0] << endl;
			cout << caracteres[1] << "->" << caracteres_c[1] << endl;
			int desplazamiento_0 = mod(caracteres_c[0]-caracteres[0], 26);
			int desplazamiento_1 = mod(caracteres_c[1]-caracteres[1], 26); // Modulo 26 para evitar negativos
			cout << "El desplazamiento de la primera es: " << desplazamiento_0 << " y el de la segunda: " << desplazamiento_1 << endl;

			if(desplazamiento_0 == desplazamiento_1) {
				cout << "Ambos desplazamientos coinciden, luego " << desplazamiento_0 << " es un desplazamiento muy probable." << endl;
				cout << "Descifrando..." << endl;
				descifrar_cesar("texto_cifrado.txt", "texto_descifrado.txt", desplazamiento_0);
				cout << "Descifrado" << endl;
			}else{
				cout << "No coinciden los desplazamientos D:" << endl;
			}
		

		}else if(submenu == 2) {
			system("cls");
			int longitud_clave;

			cout << "Longitud de la clave: " << endl;
			cin >> longitud_clave;

			cout << "Leo el texto de muestreo" << endl;
			char caracteres[26]; // Array de indices
			int resultado[26];
			int total = 0;
			contar_caracteres("texto_muestreo.txt", resultado, caracteres, &total, 1, -1);
	
			cout << "Total de caracteres contados: " << total << endl; // Es curioso que los 4 primero capitulos del quijote dan exactamente 39000 xD
			cout << "Porcentajes de aparacion de las letras en el texto en claro:" << endl;
			escribir_tabla(caracteres, resultado, total, 0);

			cout << endl << "Las ordeno de mayor a menor y me quedo las cinco mayores." << endl;
			cout << "Ordenando";
			
			ordenar_array(caracteres, resultado);
			cout << "Ordenado" << endl;
			
			cout << "Cinco mayores porcentajes en el texto en claro(sin contar el espacio):" << endl;
			// Array ordenado
			for(int i = 0; i < 6; i++)	{
				char letra = caracteres[i];
				cout << left << setw(5) << setfill(' ') << letra;
			}
			cout << endl;
			for(int i = 0; i < 6; i++)	{
				double porcentaje = (100.0 * resultado[i] / total);
				cout << left << setw(5) << setprecision(1) << fixed << setfill(' ') << porcentaje;
			}
			cout << endl << endl;

			int** subcriptogramas = new int*[longitud_clave];
			char** subcriptogramas_c = new char*[longitud_clave];
			int* subcriptogramas_t = new int[longitud_clave];
			for(int i = 0; i < longitud_clave; ++i) {
				subcriptogramas[i] = new int[26];
				subcriptogramas_c[i] = new char[26];
			}
			// Creo los subcriptogramas
			for(int i = 0; i < longitud_clave; ++i) 
				contar_caracteres("texto_cifrado.txt", subcriptogramas[i], subcriptogramas_c[i], &subcriptogramas_t[i], longitud_clave, i);

			// Letras mas frecuentes(modulo 26)
			int a = caracteres[0] - 'a';
			int b = caracteres[1] - 'a';
			int c = caracteres[2] - 'a';

			// las ordeno de mayor a menor
			if(b < a) {
				int temp = a;
				a = b;
				b = temp;
			}
			if(c < b) {
				int temp = b;
				b = c;
				c = temp;
			}
			if(b < a) {
				int temp = a;
				a = b;
				b = temp;
			}

			// Hago las distancias relativas
			b -= a;
			c -= a;

			// Nueva tabla con la suma de los valores a distantias a, b y c
			int** subcriptogramas_s = new int*[longitud_clave];
			for(int i = 0; i < longitud_clave; ++i) {
				subcriptogramas_s[i] = new int[26];
				for(int j = 0; j < 26; j++) {
					int b_d = (j + b) % 26;
					int c_d = (j + c) % 26;
					subcriptogramas_s[i][j] = subcriptogramas[i][j] + subcriptogramas[i][c_d] + subcriptogramas[i][c_d];
				}
			}

			// Dibujo la tabla
			for(int i = 0; i < longitud_clave; i++) {
				cout << "Subcriptograma " << i+1 << ", total de letras: " << subcriptogramas_t[i] << endl;
				escribir_tabla(subcriptogramas_c[i], subcriptogramas_s[i], subcriptogramas_t[i], 1);
				cout << endl << "Las ordeno de mayor a menor y me quedo los dos mayores." << endl;
				cout << "Ordenando";
				ordenar_array(subcriptogramas_c[i], subcriptogramas_s[i]);
				cout << "Ordenado" << endl;

				cout << "Visto estos resultados vamos a intentar obtener el desplazamiento" << endl;
				cout << "usando las dos primeras letras. Luego la correspondencia seria: " << endl;
				cout << caracteres[0] << "->" << subcriptogramas_c[i][0] << endl;
				cout << caracteres[1] << "->" << subcriptogramas_c[i][1] << endl;
				int desplazamiento_0 = mod(subcriptogramas_c[i][0]-caracteres[0], 26);
				int desplazamiento_1 = mod(subcriptogramas_c[i][1]-caracteres[1], 26); // Modulo 26 para evitar negativos
				cout << "El desplazamiento de la primera es: " << desplazamiento_0 << " y el de la segunda: " << desplazamiento_1 << endl;

				if(desplazamiento_0 == desplazamiento_1) {
					cout << "Ambos desplazamientos coinciden, luego " << desplazamiento_0 << " es un desplazamiento muy probable." << endl;
					char letra = 'a' + desplazamiento_0;
					cout << "Por ello la letra numero " << i << " de la constaseña seria " << letra << endl;
				}else{
					cout << "No coinciden los desplazamientos D:" << endl;
				}

				cout << endl;
			}

		}else{
			cout << "Opcion no valida" << endl;
		}
	}else{
		cout << "Opcion incorrecta" << endl;
	}
	
	cout << "Pulsa intro para salir" << endl;
	cin.get(); cin.get(); // Pause

	return 0;
}

