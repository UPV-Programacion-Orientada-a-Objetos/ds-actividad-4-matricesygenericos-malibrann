#include <iostream>

template <typename T>
class Campo2D {
private:
    T** _datos;
    int _filas;
    int _columnas;

public:
    Campo2D(int f, int c) {
        _filas = f;
        _columnas = c;
        
        _datos = new T*[_filas];
        for(int i = 0; i < _filas; i++) {
            _datos[i] = new T[_columnas];
            for(int j = 0; j < _columnas; j++) {
                _datos[i][j] = T(0);
            }
        }
        
        std::cout << "Creando Grid de " << _filas << "x" << _columnas << "...\n";
    }
    
    ~Campo2D() {
        std::cout << "Destructor invocado. Liberando memoria de la Matriz 2D...\n";
        for(int i = 0; i < _filas; i++) {
            delete[] _datos[i];
        }
        delete[] _datos;
    }
    
    void redimensionar(int nuevaF, int nuevaC) {
        std::cout << "Redimensionando Grid a " << nuevaF << "x" << nuevaC << "...\n";
        
        T** nuevosDatos = new T*[nuevaF];
        for(int i = 0; i < nuevaF; i++) {
            nuevosDatos[i] = new T[nuevaC];
            for(int j = 0; j < nuevaC; j++) {
                nuevosDatos[i][j] = T(0);
            }
        }
        
        int minFilas = (_filas < nuevaF) ? _filas : nuevaF;
        int minColumnas = (_columnas < nuevaC) ? _columnas : nuevaC;
        
        for(int i = 0; i < minFilas; i++) {
            for(int j = 0; j < minColumnas; j++) {
                nuevosDatos[i][j] = _datos[i][j];
            }
        }
        
        for(int i = 0; i < _filas; i++) {
            delete[] _datos[i];
        }
        delete[] _datos;
        
        _datos = nuevosDatos;
        _filas = nuevaF;
        _columnas = nuevaC;
        
        std::cout << "Datos copiados. Memoria antigua liberada.\n";
    }
    
    void setValor(int f, int c, T valor) {
        if(f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            _datos[f][c] = valor;
        } else {
            std::cout << "Indice fuera de rango.\n";
        }
    }
    
    T getValor(int f, int c) {
        if(f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            return _datos[f][c];
        }
        std::cout << "Indice fuera de rango.\n";
        return T(0);
    }
    
    T calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC) {
        if(inicioF < 0 || finF >= _filas || inicioC < 0 || finC >= _columnas) {
            std::cout << "Region fuera de limites.\n";
            return T(0);
        }
        
        if(inicioF >= finF || inicioC >= finC) {
            std::cout << "Region invalida.\n";
            return T(0);
        }
        
        T sumaGradientes = T(0);
        int conteo = 0;
        
        for(int i = inicioF; i < finF; i++) {
            for(int j = inicioC; j < finC; j++) {
                T valorActual = _datos[i][j];
                T diferenciaTotal = T(0);
                int vecinosContados = 0;
                
                if(i + 1 < _filas) {
                    T diff = _datos[i+1][j] - valorActual;
                    if(diff < 0) diff = -diff;
                    diferenciaTotal += diff;
                    vecinosContados++;
                }
                
                if(j + 1 < _columnas) {
                    T diff = _datos[i][j+1] - valorActual;
                    if(diff < 0) diff = -diff;
                    diferenciaTotal += diff;
                    vecinosContados++;
                }
                
                if(vecinosContados > 0) {
                    sumaGradientes += diferenciaTotal / vecinosContados;
                    conteo++;
                }
            }
        }
        
        if(conteo > 0) {
            return sumaGradientes / conteo;
        }
        
        return T(0);
    }
    
    void mostrarGrid() {
        std::cout << "\nGrid Actual:\n";
        for(int i = 0; i < _filas; i++) {
            std::cout << "| ";
            for(int j = 0; j < _columnas; j++) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    int getFilas() { return _filas; }
    int getColumnas() { return _columnas; }
};

template <typename T>
void ejecutarSimulador() {
    int filas = 3;
    int columnas = 3;
    
    std::cout << "Estableciendo valores iniciales...\n";
    Campo2D<T> campo(filas, columnas);
    
    campo.setValor(0, 0, T(10));
    campo.setValor(0, 1, T(8));
    campo.setValor(0, 2, T(5));
    campo.setValor(1, 0, T(12));
    campo.setValor(1, 1, T(9));
    campo.setValor(1, 2, T(6));
    campo.setValor(2, 0, T(15));
    campo.setValor(2, 1, T(11));
    campo.setValor(2, 2, T(7));
    
    campo.mostrarGrid();
    
    int opcion;
    do {
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Calcular Gradiente Promedio\n";
        std::cout << "2. Redimensionar Grid\n";
        std::cout << "3. Modificar Valor\n";
        std::cout << "4. Mostrar Grid\n";
        std::cout << "5. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;
        
        switch(opcion) {
            case 1: {
                int inicioF, finF, inicioC, finC;
                std::cout << "Ingrese Fila Inicial: ";
                std::cin >> inicioF;
                std::cout << "Ingrese Fila Final: ";
                std::cin >> finF;
                std::cout << "Ingrese Columna Inicial: ";
                std::cin >> inicioC;
                std::cout << "Ingrese Columna Final: ";
                std::cin >> finC;
                
                std::cout << "\nCalculando Gradiente Promedio en la region [" 
                     << inicioF << "," << finF << "]x[" 
                     << inicioC << "," << finC << "]...\n";
                
                T gradiente = campo.calcularGradientePromedio(inicioF, finF, inicioC, finC);
                std::cout << "Gradiente Promedio calculado: " << gradiente << " unidades/metro.\n";
                break;
            }
            case 2: {
                int nuevaF, nuevaC;
                std::cout << "Ingrese nuevas Filas: ";
                std::cin >> nuevaF;
                std::cout << "Ingrese nuevas Columnas: ";
                std::cin >> nuevaC;
                campo.redimensionar(nuevaF, nuevaC);
                break;
            }
            case 3: {
                int f, c;
                T valor;
                std::cout << "Ingrese Fila: ";
                std::cin >> f;
                std::cout << "Ingrese Columna: ";
                std::cin >> c;
                std::cout << "Ingrese Valor: ";
                std::cin >> valor;
                campo.setValor(f, c, valor);
                break;
            }
            case 4:
                campo.mostrarGrid();
                break;
            case 5:
                std::cout << "\nSaliendo...\n";
                break;
            default:
                std::cout << "Opcion invalida.\n";
        }
    } while(opcion != 5);
}

int main() {
    std::cout << "--- Simulador Generico de Campo 2D ---\n\n";
    
    int tipo;
    std::cout << "Seleccione el tipo de dato:\n";
    std::cout << "1. INT\n";
    std::cout << "2. FLOAT\n";
    std::cout << "3. DOUBLE\n";
    std::cout << "Opcion: ";
    std::cin >> tipo;
    
    switch(tipo) {
        case 1:
            std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo INT) <<\n";
            ejecutarSimulador<int>();
            break;
        case 2:
            std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo FLOAT) <<\n";
            ejecutarSimulador<float>();
            break;
        case 3:
            std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo DOUBLE) <<\n";
            ejecutarSimulador<double>();
            break;
        default:
            std::cout << "Tipo invalido.\n";
    }
    
    std::cout << "Sistema cerrado.\n";
    return 0;
}