#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

// Definición de los tamaños disponibles para los archivos
enum Size { SMALL, MEDIUM, LARGE };

// Función para obtener el tamaño basado en el argumento de cadena
Size getSize(const std::string& sizeStr) {
    if (sizeStr == "SMALL") return SMALL;
    if (sizeStr == "MEDIUM") return MEDIUM;
    if (sizeStr == "LARGE") return LARGE;
    throw std::invalid_argument("Argumento de tamaño inválido");
}

// Función para obtener el tamaño del archivo en bytes basado en el tamaño especificado
size_t getFileSize(Size size) {
    switch (size) {
        case SMALL: return 512 * 1024 * 1024;  // 512 MB
        case MEDIUM: return 1024 * 1024 * 1024; // 1 GB
        case LARGE: return 2 * 1024 * 1024 * 1024; // 2 GB
    }
    throw std::invalid_argument("Tamaño inválido");
}

int main(int argc, char* argv[]) {
    // Verificar que el número de argumentos sea correcto
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " -size <SIZE> -output <RUTA DEL ARCHIVO DE SALIDA>" << std::endl;
        return 1;
    }

    std::string sizeStr;
    std::string outputPath;

    // Procesar los argumentos de la línea de comandos
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-size") {
            if (i + 1 < argc) {
                sizeStr = argv[++i];
            } else {
                std::cerr << "Error: Falta el valor para -size" << std::endl;
                return 1;
            }
        } else if (std::string(argv[i]) == "-output") {
            if (i + 1 < argc) {
                outputPath = argv[++i];
            } else {
                std::cerr << "Error: Falta el valor para -output" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Argumento inválido: " << argv[i] << std::endl;
            return 1;
        }
    }

    // Convertir el argumento de tamaño a un valor del enum
    Size size;
    try {
        size = getSize(sizeStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Obtener el tamaño del archivo en bytes
    size_t fileSize = getFileSize(size);
    size_t numIntegers = fileSize / sizeof(int);

    // Crear y abrir el archivo de salida en modo binario
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo de salida" << std::endl;
        return 1;
    }

    // Inicializar la semilla para generar números aleatorios
    std::srand(std::time(nullptr));

    // Escribir números aleatorios en el archivo
    for (size_t i = 0; i < numIntegers; ++i) {
        int randomInt = std::rand();
        outFile.write(reinterpret_cast<const char*>(&randomInt), sizeof(randomInt));
    }

    // Cerrar el archivo de salida
    outFile.close();
    std::cout << "¡Archivo generado con éxito!" << std::endl;

    return 0;
}
