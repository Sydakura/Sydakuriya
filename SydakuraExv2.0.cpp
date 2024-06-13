#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <windows.h>
using namespace std;
string resaltado(40, '-');
class Producto {
public:
    string nombre;
    double precio;
    int stock;
    int stockInicial;

    Producto(string nombre, double precio, int stock) : nombre(nombre), precio(precio), stock(stock), stockInicial(stock) {}
};

class Vendedor {
public:
    string nombre;
    string ci;
    double totalVentas;
    int numeroVentas;
    double bono;

    Vendedor(string nombre, string ci) : nombre(nombre), ci(ci), totalVentas(0), numeroVentas(0), bono(0) {}
};

vector<Vendedor> vendedores;
Vendedor* vendedorActual = NULL;

vector<Producto> lacteos;
vector<Producto> limpieza;
vector<Producto> escritorio;
vector<Producto> electrodomesticos;

void inicializarInventario() {
    lacteos.push_back(Producto("Leche", 10, 50));
    lacteos.push_back(Producto("Queso", 20, 30));
    lacteos.push_back(Producto("Yoghurt", 15, 20));

    limpieza.push_back(Producto("Ace", 25, 40));
    limpieza.push_back(Producto("Cera pisos", 30, 50));
    limpieza.push_back(Producto("Escoba", 35, 60));

    escritorio.push_back(Producto("Papel", 5, 100));
    escritorio.push_back(Producto("Tinta", 50, 20));
    escritorio.push_back(Producto("Lapiceros", 3, 200));

    electrodomesticos.push_back(Producto("Licuadora", 450, 10));
    electrodomesticos.push_back(Producto("Horno Electrico", 600, 5));
    electrodomesticos.push_back(Producto("Batidora", 400, 15));
}

void registrarVendedoresPredeterminados() {
    vendedores.push_back(Vendedor("Luis Camacho", "54274253"));
    vendedores.push_back(Vendedor("Camila Arnez", "83442424"));
    vendedores.push_back(Vendedor("Pedro Teran", "892374943"));
    vendedores.push_back(Vendedor("Sydakura", "12345"));
}

bool controlAcceso() {
    string nombre, ci;
    int intentos = 3;
    while (intentos > 0) {
        cout << "Ingrese su nombre: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Ingrese su carnet de identidad: ";
        cin >> ci;

        for (size_t i = 0; i < vendedores.size(); ++i) {
            if (vendedores[i].ci == ci && vendedores[i].nombre == nombre) {
                vendedorActual = &vendedores[i];
                return true;
            }
        }

        cout << "Vendedor no encontrado. Le quedan " << --intentos << " intentos.\n";
        if (intentos == 0) {
            return false;
        }
    }
    return false;
}

void cambiarUsuario() {
    vendedorActual = NULL;
    if (!controlAcceso()) {
        cout << "Fallo en el inicio de sesion. Saliendo del programa...\n";
        exit(1);
    }
}

void mostrarListaProductos() {
    cout << "Categorias:\n";
    cout << "1. Lacteos\n";
    cout << "2. Limpieza\n";
    cout << "3. Material de Escritorio\n";
    cout << "4. Electrodomesticos\n";
    cout << resaltado << "\n";
}

void mostrarProductosCategoria(vector<Producto>& productos) {
    for (size_t i = 0; i < productos.size(); ++i) {
        cout << i + 1 << ". " << productos[i].nombre << ": " << productos[i].precio << " Bs. (Stock: " << productos[i].stock << ")\n";
    }
}

double realizarVenta(Vendedor* vendedor) {
    int categoria, productoIndex, cantidad;
    double montoVenta = 0;

    mostrarListaProductos();

    cout << "Seleccione la categoria del producto: ";
    cin >> categoria;
    cout << resaltado << "\n";

    vector<Producto>* productos = NULL;

    switch (categoria) {
        case 1:
            productos = &lacteos;
            break;
        case 2:
            productos = &limpieza;
            break;
        case 3:
            productos = &escritorio;
            break;
        case 4:
            productos = &electrodomesticos;
            break;
        default:
            cout << "Categoria invalida.\n";
            return 0;
    }

    mostrarProductosCategoria(*productos);
    cout << resaltado << "\n";
    cout << "Seleccione el producto: ";
    cin >> productoIndex;
    if (productoIndex < 1 || productoIndex > productos->size()) {

        cout << "Producto invalido.\n";
        return 0;
    }
    cout << resaltado << "\n";
    cout << "Ingrese la cantidad: ";
    cin >> cantidad;

    Producto& producto = (*productos)[productoIndex - 1];

    if (producto.stock < cantidad) {
        cout << "Stock insuficiente.\n";
        return 0;
    }

    producto.stock -= cantidad;
    montoVenta = producto.precio * cantidad;

    double descuento = 0;
    if (categoria == 4 && montoVenta > 500) { // Electrodomesticos
        descuento = montoVenta * 0.05;
        montoVenta *= 0.95;
        cout << "Descuento del 5% aplicado a electrodomesticos por monto mayor a 500 Bs. Descuento: " << descuento << " Bs.\n";
    } else if (montoVenta > 300) {
        descuento = montoVenta * 0.10;
        montoVenta *= 0.90;
        cout << "Descuento del 10% aplicado por monto mayor a 300 Bs. Descuento: " << descuento << " Bs.\n";
    }

    cout << "Precio total sin descuento: " << producto.precio * cantidad << " Bs.\n";
    cout << "Descuento aplicado: " << descuento << " Bs.\n";
    cout << "Precio total con descuento: " << montoVenta << " Bs.\n";

    vendedor->totalVentas += montoVenta;
    vendedor->numeroVentas++;

    return montoVenta;
}

void listaDeVendedores() {
    cout << "Lista de Vendedores:\n";
    for (size_t i = 0; i < vendedores.size(); ++i) {
        cout << "- " << vendedores[i].nombre << "\n";
    }
}

void calcularPromedioVentas() {
    double sumaTotal = 0;
    for (size_t i = 0; i < vendedores.size(); ++i) {
        sumaTotal += vendedores[i].totalVentas;
    }
    double promedio = vendedores.empty() ? 0 : sumaTotal / vendedores.size();
    cout << "El promedio de ventas es: " << promedio << " Bs.\n";
}

void calcularBonos() {
    for (size_t i = 0; i < vendedores.size(); ++i) {
        if (vendedores[i].numeroVentas > 3 && vendedores[i].bono == 0) {
            vendedores[i].bono = vendedores[i].totalVentas * 0.02;
            cout << "Bono de 2% aplicado a " << vendedores[i].nombre << ". Bono: " << vendedores[i].bono << " Bs.\n";
        }
    }
}

double calcularTotalCategoria(vector<Producto>& productos) {
    double total = 0;
    for (size_t i = 0; i < productos.size(); ++i) {
        int vendidos = productos[i].stockInicial - productos[i].stock;
        total += productos[i].precio * vendidos;
    }
    return total;
}

void mostrarResumen() {
    double totalRecaudado = 0;
    Vendedor* mejorVendedor = vendedores.empty() ? NULL : &vendedores[0];

    for (size_t i = 0; i < vendedores.size(); ++i) {
        cout << "Vendedor: " << vendedores[i].nombre << "\n";
        cout << "Total Recaudado: " << vendedores[i].totalVentas << " Bs.\n";
        cout << "Numero de Ventas: " << vendedores[i].numeroVentas << "\n";
        cout << "Bono Ganado: " << vendedores[i].bono << " Bs.\n";
        cout << "Sueldo fijo: 73 Bs.\n";
        cout << "Total Ganado: " << vendedores[i].totalVentas + vendedores[i].bono + 73 << " Bs.\n\n";
        totalRecaudado += vendedores[i].totalVentas;

        if (mejorVendedor && vendedores[i].totalVentas > mejorVendedor->totalVentas) {
            mejorVendedor = &vendedores[i];
        }
    }

    cout << "Total Recaudado por todos los vendedores: " << totalRecaudado << " Bs.\n";
    if (mejorVendedor) {
        cout << "Mejor Vendedor: " << mejorVendedor->nombre << " con " << mejorVendedor->totalVentas << " Bs.\n";
    }

    cout << "Total vendido en Lacteos: " << calcularTotalCategoria(lacteos) << " Bs.\n";
    cout << "Total vendido en Limpieza: " << calcularTotalCategoria(limpieza) << " Bs.\n";
    cout << "Total vendido en Material de Escritorio: " << calcularTotalCategoria(escritorio) << " Bs.\n";
    cout << "Total vendido en Electrodomesticos: " << calcularTotalCategoria(electrodomesticos) << " Bs.\n";
}

//string resaltado(40, '-');
int main() {
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
 SetConsoleTextAttribute(hConsole, 7);
    inicializarInventario();
    registrarVendedoresPredeterminados();

    if (!controlAcceso()) {
        cout << "Fallo en el inicio de sesion. Saliendo del programa...\n";
        return 1;
    }

    int opcion;
    do {
            SetConsoleTextAttribute(hConsole, 1);
        cout << "1. Registrar venta\n";
        SetConsoleTextAttribute(hConsole, 2);
        cout << "2. Calcular promedio de ventas\n";
        SetConsoleTextAttribute(hConsole, 3);
        cout << "3. Mostrar lista de vendedores\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "4. Mostrar resumen\n";
        SetConsoleTextAttribute(hConsole, 4);
        cout << "5. Finalizar jornada\n";
        SetConsoleTextAttribute(hConsole, 6);
        cout << "6. Salir\n";
        SetConsoleTextAttribute(hConsole, 8);
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                SetConsoleTextAttribute(hConsole, 2);
                cout << resaltado << "\n";
                double monto = realizarVenta(vendedorActual);
                SetConsoleTextAttribute(hConsole, 7);
                cout << resaltado << "\n";
                cout << "Venta realizada. Monto total: " << monto << " Bs.\n";
                cout << resaltado << "\n";
                break;
            }
            case 2:
                SetConsoleTextAttribute(hConsole, 7);
                cout << resaltado << "\n";
                calcularPromedioVentas();
                cout << resaltado << "\n";
                break;
            case 3:
                SetConsoleTextAttribute(hConsole, 7);
                cout << resaltado << "\n";
                listaDeVendedores();
                cout << resaltado << "\n";
                break;
            case 4:
                SetConsoleTextAttribute(hConsole, 5);
                cout << resaltado << "\n";
                mostrarResumen();
                cout << resaltado << "\n";
                break;
            case 5:
                SetConsoleTextAttribute(hConsole, 4);
                calcularBonos();
                cambiarUsuario();
                break;
            case 6:
                SetConsoleTextAttribute(hConsole, 6);
                cout << "Saliendo del programa.\n";
                break;
            default:
                SetConsoleTextAttribute(hConsole, 9);
                cout << "Opcion invalida.\n";
                break;
        }
    } while (opcion != 6);

    return 0;
}
