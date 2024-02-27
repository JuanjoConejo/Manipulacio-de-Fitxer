#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

// Definició de la estructura per emmagatzemar la capçalera d'un GIF
struct CapcaleraGIF {
    char signatura[4]; // Canviat a 4 per incloure el caràcter nul
    char versio[4]; // Canviat a 4 per incloure el caràcter nul
};

// Funció per llegir la capçalera d'un GIF des d'un fitxer
CapcaleraGIF llegirCapcaleraGIF(std::ifstream& fitxer) {
    CapcaleraGIF capcalera;
    fitxer.read(capcalera.signatura, 3);
    capcalera.signatura[3] = '\0'; // Afegit el caràcter nul al final de la cadena
    fitxer.read(capcalera.versio, 3);
    capcalera.versio[3] = '\0'; // Afegit el caràcter nul al final de la cadena
    return capcalera;
}

// Funció per llegir les dimensions d'un GIF des d'un fitxer
void llegirDimensionsGIF(std::ifstream& fitxer, int& amplada, int& alcada) {
    fitxer.seekg(6); // Posicionar el punter del fitxer a la posició on es troben les dimensions
    fitxer.read(reinterpret_cast<char*>(&amplada), sizeof(amplada)); // Llegir l'amplada del GIF
    fitxer.read(reinterpret_cast<char*>(&alcada), sizeof(alcada)); // Llegir l'alçada del GIF
}

// Funció per comptar el nombre de colors d'un GIF
int comptarColors(std::ifstream& fitxer) {
    fitxer.seekg(10); // Posicionar el punter del fitxer a la posició on es troba la informació dels colors
    char infoColor;
    fitxer.read(&infoColor, 1); // Llegir la informació dels colors
    return 1 << ((infoColor & 0x07) + 1); // Càlcul del nombre de colors basat en la informació llegida
}

// Funció per duplicar un fitxer GIF
void duplicarGIF(const std::string& nomFitxerOriginal, const std::string& nomFitxerDuplicat) {
    std::ifstream fitxerOriginal(nomFitxerOriginal, std::ios::binary); // Obrir el fitxer original en mode binari
    std::ofstream fitxerDuplicat(nomFitxerDuplicat, std::ios::binary); // Crear un fitxer duplicat en mode binari

    // Copiar el contingut del fitxer original al fitxer duplicat
    fitxerDuplicat << fitxerOriginal.rdbuf();
    std::cout << "Fitxer duplicat amb exit com a " << nomFitxerDuplicat << std::endl; // Mostrar missatge d'èxit
}

int main() {
    std::string nomFitxer;
    std::cout << "Introdueix la ruta del fitxer GIF: ";
    std::cin >> nomFitxer; // Demanar a l'usuari que introdueixi la ruta del fitxer GIF

    std::ifstream fitxer(nomFitxer, std::ios::binary); // Obrir el fitxer GIF en mode binari
    if (!fitxer) {
        std::cerr << "No s'ha pogut obrir el fitxer." << std::endl; // Missatge d'error si no es pot obrir el fitxer
        return 1;
    }

    // Llegir la capçalera, les dimensions i comptar els colors del GIF
    CapcaleraGIF capcalera = llegirCapcaleraGIF(fitxer);
    int amplada, alcada;
    llegirDimensionsGIF(fitxer, amplada, alcada);
    int numColors = comptarColors(fitxer);

    // Mostrar la informació del GIF llegida
    std::cout << "Informacio del fitxer GIF:" << std::endl;
    std::cout << "Signatura: " << capcalera.signatura << std::endl;
    std::cout << "Versio: " << capcalera.versio << std::endl;
    std::cout << "Amplada: " << amplada << " pixels" << std::endl;
    std::cout << "Alcada: " << alcada << " pixels" << std::endl;
    std::cout << "Nombre de colors: " << numColors << std::endl;

    char eleccio;
    std::cout << "Desitja duplicar el fitxer? (s/n): ";
    std::cin >> eleccio; // Demanar a l'usuari si desitja duplicar el fitxer
    if (eleccio == 's' || eleccio == 'S') {
        std::string nomFitxerDuplicat;
        std::cout << "Introdueix el nom del fitxer duplicat: ";
        std::cin >> nomFitxerDuplicat; // Demanar a l'usuari el nom pel fitxer duplicat
        duplicarGIF(nomFitxer, nomFitxerDuplicat); // Duplicar el fitxer GIF
    }
    return 0;
}