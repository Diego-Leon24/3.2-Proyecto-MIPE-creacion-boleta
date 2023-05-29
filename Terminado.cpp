#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib> //para borrar terminal
#include <conio.h> // Necesario para la función _getch
#include <limits>
using namespace std;
// Variable global para almacenar el total de ventas del día
double totalVentasDia = 0.0;
double gananciatotalDia = 0.0;
const int ESPACIO_NOMBRE = 35;
const int ESPACIO_CANTIDAD = 10;
const int ESPACIO_PRECIO = 10;
const int CODIGO = 10;
static string permisoderegistro;
static string ventanombreUsuario;

class Cliente
{
private:
    string u_dni;
    int usuario;
    string n_dni;

public:
    friend class Venta;
    friend class Producto;
    friend class registroventa;
} cliente;

class Producto
{
private:
    string nombre;
    int cantidad;
    double precio;
    double preciocosto;
    double precioventa;
    double precioganancia;

public:
    friend class registroventa;
    friend class Venta;
    void agregarProducto()
    {

        double precioVenta;
        double precioCosto;

        // Solicitar los datos del producto al usuario
        cout << "Ingrese el nombre del producto (0 para cancelar): ";
        cin >> nombre;

        // Verificar si se ingresó "0" para cancelar la operación
        if (nombre == "0")
        {
            cout << "Operacion cancelada." << endl;
            cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
            _getch();
            system("cls");
            return;
        }

        cout << "Ingrese la cantidad: ";
        cin >> cantidad;

        cout << "Ingrese el precio de venta: ";
        cin >> precioVenta;

        cout << "Ingrese el precio de costo: ";
        cin >> precioCosto;

        ofstream archivoProductos("productos.txt", ios_base::app);
        ofstream archivoCosto("costo.txt", ios_base::app);

        if (archivoProductos.is_open() && archivoCosto.is_open())
        {
            // Escribir en el archivo "productos.txt"
            archivoProductos << nombre << " " << cantidad << " " << precioVenta << endl;

            // Escribir en el archivo "costo.txt"
            archivoCosto << nombre << " " << precioCosto << " " << precioVenta << " " << (precioVenta - precioCosto) << endl;

            archivoProductos.close();
            archivoCosto.close();

            cout << "Producto agregado correctamente." << endl;
        }
        else
        {
            cerr << "No se pudo abrir uno o ambos archivos." << endl;
        }
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }

    static bool compararPorNombre(const Producto &a, const Producto &b)
    {
        return a.nombre < b.nombre;
    }

    void mostrarProductos()
    {
        ifstream archivo("productos.txt");
        if (archivo.is_open())
        {
            vector<Producto> productos;
            Producto p;
            while (archivo >> p.nombre >> p.cantidad >> p.precio)
            {
                productos.push_back(p);
            }
            archivo.close();

            sort(productos.begin(), productos.end(), compararPorNombre);
            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
            cout << left << setw(ESPACIO_NOMBRE) << "Nombre" << setw(ESPACIO_CANTIDAD) << "Cantidad" << setw(ESPACIO_PRECIO) << "Precio" << endl;
            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
            for (const auto &producto : productos)
            {
                cout << left << setw(ESPACIO_NOMBRE) << producto.nombre << setw(ESPACIO_CANTIDAD) << producto.cantidad << "$" << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << producto.precio << endl;
            }
            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
        }
        else
        {
            cerr << "No se pudo abrir el archivo" << endl;
        }
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }
    void ProductosCostos()
    {
        ifstream archivo("costo.txt");
        if (archivo.is_open())
        {
            vector<Producto> productoscost;
            Producto p;
            while (archivo >> p.nombre >> p.preciocosto >> p.precioventa >> p.precioganancia)
            {
                productoscost.push_back(p);
            }
            archivo.close();

            sort(productoscost.begin(), productoscost.end(), compararPorNombre);

            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO + 20) << "" << setfill(' ') << endl;
            cout << left << setw(ESPACIO_NOMBRE - 4) << "Nombre" << setw(ESPACIO_CANTIDAD + 6) << "Precio Costo" << setw(ESPACIO_PRECIO + 6) << "Precio Venta" << setw(ESPACIO_PRECIO + 6) << "Precio Ganancia" << endl;
            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO + 20) << "" << setfill(' ') << endl;
            for (const auto &producto : productoscost)
            {
                cout << left << setw(ESPACIO_NOMBRE) << producto.nombre << "$" << setw(ESPACIO_CANTIDAD + 4) << fixed << setprecision(2) << producto.preciocosto << "$" << setw(ESPACIO_PRECIO + 7) << fixed << setprecision(2) << producto.precioventa << "$" << setw(ESPACIO_PRECIO + 5) << fixed << setprecision(2) << producto.precioganancia << endl;
            }
            cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO + 20) << "" << setfill(' ') << endl;
        }
        else
        {
            cerr << "No se pudo abrir el archivo" << endl;
        }
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }

    void eliminarProducto()
    {
        string nombreProducto;
        cout << "Ingrese el nombre del producto a eliminar (0 para cancelar): ";
        cin >> nombreProducto;

        if (nombreProducto == "0")
        {
            cout << "Operación cancelada." << endl;
            cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
            _getch();
            system("cls");
            return;
        }

        // Eliminar el producto del archivo "productos.txt"
        ifstream archivoProductosIn("productos.txt");
        ofstream archivoProductosOut("productos_tmp.txt");
        if (archivoProductosIn.is_open() && archivoProductosOut.is_open())
        {
            std::string nombre;
            int cantidad;
            double precioVenta;
            while (archivoProductosIn >> nombre >> cantidad >> precioVenta)
            {
                if (nombre != nombreProducto)
                {
                    archivoProductosOut << nombre << " " << cantidad << " " << precioVenta << std::endl;
                }
            }
            archivoProductosIn.close();
            archivoProductosOut.close();
            remove("productos.txt");
            rename("productos_tmp.txt", "productos.txt");

            cout << "Producto eliminado correctamente de 'productos.txt'." << std::endl;
        }
        else
        {
            cerr << "No se pudo abrir uno o ambos archivos de productos." << std::endl;
            return;
        }

        // Eliminar el producto del archivo "costo.txt"
        ifstream archivoCostoIn("costo.txt");
        ofstream archivoCostoOut("costo_tmp.txt");
        if (archivoCostoIn.is_open() && archivoCostoOut.is_open())
        {
            string nombre;
            double precioCosto;
            double precioVenta;
            double ganancia;
            while (archivoCostoIn >> nombre >> precioCosto >> precioVenta >> ganancia)
            {
                if (nombre != nombreProducto)
                {
                    archivoCostoOut << nombre << " " << precioCosto << " " << precioVenta << " " << ganancia << std::endl;
                }
            }
            archivoCostoIn.close();
            archivoCostoOut.close();
            remove("costo.txt");
            rename("costo_tmp.txt", "costo.txt");

            cout << "Producto eliminado correctamente de 'costo.txt'." << std::endl;
        }
        else
        {
            cerr << "No se pudo abrir uno o ambos archivos de costo." << std::endl;
            return;
        }

        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }
};
struct Usuario
{
    string nombreUsuario;
    string contraseña;
    string permiso;
};
class registroventa
{
public:
    friend class Venta;
    // Función para guardar una venta en el archivo "ventas.txt"
    void guardarVenta(const vector<Producto> &productos, const vector<int> &seleccionados, const vector<int> &cantidadesVendidas, double totalVenta)
    {
        ifstream archivo("costo.txt");
        if (!archivo.is_open())
        {
            cerr << "No se pudo abrir el archivo de costos." << endl;
            return;
        }

        // Crear un mapa para almacenar los precios de costo y ganancias por nombre de producto
        map<string, pair<double, double>> preciosGanancias;
        string nombreProducto;
        double ganancia;
        while (archivo >> nombreProducto >> ganancia)
        {
            preciosGanancias[nombreProducto].second = ganancia;
        }
        archivo.close();

        ofstream archivoVentas("ventas.txt", ios_base::app);
        if (!archivoVentas.is_open())
        {
            cerr << "No se pudo abrir el archivo de ventas." << endl;
            return;
        }

        // Obtener fecha y hora actual
        time_t tiempoActual = time(nullptr);
        tm *fechaHoraActual = localtime(&tiempoActual);
        char buffer[80];
        strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", fechaHoraActual);
        archivoVentas << setfill('-') << setw(73) << "" << setfill(' ') << endl;
        // Escribir información de la venta en el archivo
        archivoVentas << "Numero de usuario :" << cliente.usuario << endl;
        archivoVentas << "DNI DEL CLIENTE: " << cliente.n_dni << endl;
        archivoVentas << "Fecha y hora: " << buffer << endl;
        archivoVentas << "Productos vendidos:" << endl;
        archivoVentas << left << setw(CODIGO) << "CODIGO" << setw(ESPACIO_NOMBRE) << "Nombre" << setw(ESPACIO_CANTIDAD) << "Cantidad" << setw(ESPACIO_PRECIO) << "Precio" << setw(ESPACIO_PRECIO) << "Ganancia" << endl;
        archivoVentas << setfill('-') << setw(73) << "" << setfill(' ') << endl;

        double gananciaTotal = 0.0; // Variable para almacenar la ganancia total

        for (std::vector<int>::size_type i = 0; i < seleccionados.size(); i++)
        {
            int indiceProducto = seleccionados[i] - 1;
            archivoVentas << left << setw(CODIGO) << seleccionados[i] << ". " << setw(ESPACIO_NOMBRE) << productos[indiceProducto].nombre << setw(ESPACIO_CANTIDAD) << cantidadesVendidas[i] << "$" << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << productos[indiceProducto].precio << "$";

            // Obtener la ganancia del producto actual
            double ganancia = preciosGanancias[productos[indiceProducto].nombre].second;

            // Calcular la ganancia por producto
            double gananciaProducto = ganancia * cantidadesVendidas[i];
            archivoVentas << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << gananciaProducto << endl;

            gananciaTotal += gananciaProducto; // Agregar la ganancia del producto a la ganancia total
        }

        archivoVentas << "" << setfill(' ') << endl;
        archivoVentas << "Total de venta: $" << totalVenta << endl;
        archivoVentas << "Ganancia total: $" << gananciaTotal << endl;
        cout << endl;
        // Incrementar el total de ventas del día
        totalVentasDia += totalVenta;
        gananciatotalDia += gananciaTotal;
        archivoVentas.close();

        /// Si es que es empleado el usuario
        if (permisoderegistro == "empleado") // Utilizando la variable global permiso de registro
        {
            string nombreArchivo = ventanombreUsuario + "_ventas.txt";
            ifstream archivo("costo.txt");
            if (!archivo.is_open())
            {
                cerr << "No se pudo abrir el archivo de costos." << endl;
                return;
            }

            ofstream archivoVentas(nombreArchivo, ios_base::app);
            if (!archivoVentas.is_open())
            {
                cerr << "No se pudo abrir el archivo de ventas." << endl;
                return;
            }

            // Obtener fecha y hora actual
            time_t tiempoActual = time(nullptr);
            tm *fechaHoraActual = localtime(&tiempoActual);
            char buffer[80];
            strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", fechaHoraActual);
            archivoVentas << setfill('-') << setw(73) << "" << setfill(' ') << endl;
            // Escribir información de la venta en el archivo
            archivoVentas << "Numero de usuario :" << cliente.usuario << endl;
            archivoVentas << "DNI DEL CLIENTE: " << cliente.n_dni << endl;
            archivoVentas << "Fecha y hora: " << buffer << endl;
            archivoVentas << "Productos vendidos:" << endl;
            archivoVentas << left << setw(CODIGO) << "CODIGO" << setw(ESPACIO_NOMBRE) << "Nombre" << setw(ESPACIO_CANTIDAD) << "Cantidad" << setw(ESPACIO_PRECIO) << "Precio" << setw(ESPACIO_PRECIO) << "Ganancia" << endl;
            archivoVentas << setfill('-') << setw(73) << "" << setfill(' ') << endl;

            for (std::vector<int>::size_type i = 0; i < seleccionados.size(); i++)
            {
                int indiceProducto = seleccionados[i] - 1;
                archivoVentas << left << setw(CODIGO) << seleccionados[i] << ". " << setw(ESPACIO_NOMBRE) << productos[indiceProducto].nombre << setw(ESPACIO_CANTIDAD) << cantidadesVendidas[i] << "$" << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << productos[indiceProducto].precio << "$";

                // Obtener la ganancia del producto actual
                double ganancia = preciosGanancias[productos[indiceProducto].nombre].second;

                // Calcular la ganancia por producto
                double gananciaProducto = ganancia * cantidadesVendidas[i];
                archivoVentas << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << gananciaProducto << endl;
            }

            archivoVentas << "" << setfill(' ') << endl;
            archivoVentas << "Total de venta: $" << totalVenta << endl;
            archivoVentas << "Ganancia total: $" << gananciaTotal << endl;
            cout << endl;

            struct TotalesVentas
            {
                double total_venta;
                double ganancia_total;
            };

            TotalesVentas totales;
            std::ifstream archivo_ventas(nombreArchivo);

            if (archivo_ventas.is_open())
            {
                std::string linea;
                bool encontrados_encabezados = false;
                while (std::getline(archivo_ventas, linea))
                {
                    if (!encontrados_encabezados)
                    {
                        if (linea.find("CODIGO    Nombre") != std::string::npos)
                        {
                            encontrados_encabezados = true;
                        }
                        continue;
                    }
                    size_t pos = linea.find("$");
                    if (pos != std::string::npos)
                    {
                        std::string valor_str = linea.substr(pos + 1);
                        double valor = std::stod(valor_str);
                        if (linea.find("Total de venta:") != std::string::npos)
                        {
                            totales.total_venta += valor;
                        }
                        else if (linea.find("Ganancia total:") != std::string::npos)
                        {
                            totales.ganancia_total += valor;
                        }
                    }
                }
                archivo_ventas.close();
            }

            archivoVentas << "|| Ventas TOTAL DEL EMPLEADO ||" << endl;
            archivoVentas << "|| TOTAL De venta :  " << totales.total_venta << "   ||" << endl;
            archivoVentas << "|| Ganancia TOTAL : " << totales.ganancia_total << " ||" << endl;
            archivoVentas << "" << endl;
            archivoVentas.close();
            cout << "Venta registrada correctamente." << endl;
        }
        else
        {
            cout << "Se registro la venta pero no como empleado" << endl;
        }
    }
    // Función para escribir el total de ventas del día en el archivo "total_ventas.txt"
    void escribirTotalVentasDia()
    {
        ofstream archivo("ventas.txt", ios_base::app);
        if (!archivo.is_open())
        {
            cout << "No se pudo abrir el archivo de total de ventas." << endl;
            return;
        }

        // Obtener fecha actual
        time_t tiempoActual = time(nullptr);
        tm *fechaActual = localtime(&tiempoActual);
        char buffer[80];
        strftime(buffer, 80, "%d/%m/%Y", fechaActual);
        archivo << "|| Ventas TOTALES DEL DIA ||" << endl;
        archivo << "Total de ventas del " << buffer << ": $" << totalVentasDia << endl;
        archivo << "Total de ganancia del " << buffer << ": $" << gananciatotalDia << endl;
        archivo << setfill('-') << setw(73) << "" << setfill(' ') << endl;
        archivo << "" << endl;
        archivo.close();
    }

    void verHistorialVentas()
    {
        ifstream archivo("ventas.txt");
        if (archivo.is_open())
        {
            string linea;
            while (getline(archivo, linea))
            {
                cout << linea << endl;
            }
            archivo.close();
        }
        else
        {
            cerr << "No se pudo abrir el archivo de ventas." << endl;
        }
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }

    void mostrarHistorialVentasEmpleado()
    {
        // Abrir el archivo de usuarios
        ifstream archivoUsuarios("usuarios.txt");
        if (!archivoUsuarios.is_open())
        {
            cerr << "No se pudo abrir el archivo de usuarios." << endl;
            return;
        }

        vector<Usuario> empleados;

        // Leer y procesar los datos de los usuarios
        string lineaUsuario;
        while (getline(archivoUsuarios, lineaUsuario))
        {
            stringstream ss(lineaUsuario);
            string nombreUsuario, contraseña, permiso;

            // Leer los datos de cada usuario
            ss >> nombreUsuario >> contraseña >> permiso;

            // Comprobar si el usuario tiene el permiso de empleado
            if (permiso == "empleado")
            {
                Usuario empleado;
                empleado.nombreUsuario = nombreUsuario;
                empleado.contraseña = contraseña;
                empleado.permiso = permiso;
                empleados.push_back(empleado);
            }
        }

        archivoUsuarios.close();

        // Mostrar la lista de empleados disponibles
        cout << "Empleados disponibles:" << endl;
        for (const Usuario &empleado : empleados)
        {
            cout << empleado.nombreUsuario << endl;
        }

        // Pedir al usuario que seleccione un empleado
        string nombreEmpleadoSeleccionado;
        cout << "Seleccione un empleado: ";
        cin >> nombreEmpleadoSeleccionado;

        // Buscar al empleado seleccionado en la lista
        bool empleadoEncontrado = false;
        for (const Usuario &empleado : empleados)
        {
            if (empleado.nombreUsuario == nombreEmpleadoSeleccionado)
            {
                empleadoEncontrado = true;
                break;
            }
        }

        if (!empleadoEncontrado)
        {
            cout << "El empleado seleccionado no es válido." << endl;
            cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
            _getch();
            system("cls");
            return;
        }

        // Mostrar el historial de ventas del empleado seleccionado
        string nombreArchivo = nombreEmpleadoSeleccionado + "_ventas.txt";
        ifstream archivoVentas(nombreArchivo);
        cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
        cout << "Historial de ventas para el empleado: " << nombreEmpleadoSeleccionado << endl;
        cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;

        if (archivoVentas.is_open())
        {
            string lineaVenta;
            while (getline(archivoVentas, lineaVenta))
            {

                string linea;
                while (getline(archivoVentas, linea))
                {
                    cout << linea << endl;
                }
                archivoVentas.close();
            }

            archivoVentas.close();
        }
        else
        {
            cerr << "No se pudo abrir el archivo de historial de ventas." << std::endl;
        }
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }
};

class Venta : public registroventa
{
public:
    void venderProducto()
    {
        // Abrir archivo de productos
        ifstream archivo("productos.txt");
        if (!archivo.is_open())
        {
            cout << "No se pudo abrir el archivo de productos." << endl;
            return;
        }

        // Leer productos del archivo y almacenarlos en un vector
        vector<Producto> productos;
        string linea;
        while (getline(archivo, linea))
        {
            stringstream registro(linea);
            Producto p;
            registro >> p.nombre >> p.cantidad >> p.precio;
            productos.push_back(p);
        }
        archivo.close();

        string input;
        bool esNumero = false;
        int cantidadProductos;

        do
        {
            cout << "Ingrese la cantidad de productos a vender (o 0 para cancelar): ";
            cin >> input;

            // Verificar si la entrada es un número válido
            esNumero = true;
            for (char c : input)
            {
                if (!isdigit(c))
                {
                    esNumero = false;
                    break;
                }
            }

            if (!esNumero)
            {
                cout << "Entrada inválida. Por favor, ingrese un número válido." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cantidadProductos = stoi(input);

            if (cantidadProductos < 0 || cantidadProductos > static_cast<int>(productos.size()))
            {
                cout << "Cantidad inválida. Por favor, ingrese un valor válido." << endl;
                esNumero = false; // Volver a solicitar la entrada
            }
            if (cantidadProductos == 0)
            {
                cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
                _getch();
                system("cls");
                return;
            }
        } while (!esNumero);

        // Mostrar lista de productos disponibles
        cout << "Productos disponibles:" << endl;
        cout << left << setw(CODIGO) << "CODIGO" << setw(ESPACIO_NOMBRE) << "Nombre" << setw(ESPACIO_CANTIDAD) << "Cantidad" << setw(ESPACIO_PRECIO) << "Precio" << endl;
        cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
        for (std::vector<Producto>::size_type i = 0; i < productos.size(); i++)
        {
            cout << left << setw(CODIGO) << i + 1 << ". " << setw(ESPACIO_NOMBRE) << productos[i].nombre << setw(ESPACIO_CANTIDAD) << productos[i].cantidad << "$" << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << productos[i].precio << endl;
        }

        // Seleccionar productos a vender
        vector<int> seleccionados;
        int seleccion;
        for (int i = 0; i < cantidadProductos; i++)
        {
            do
            {
                cout << "Ingrese el número del producto #" << i + 1 << " a vender (o 0 para cancelar): ";
                cin >> input;

                // Verificar si la entrada es un número válido
                esNumero = true;
                for (char c : input)
                {
                    if (!isdigit(c))
                    {
                        esNumero = false;
                        break;
                    }
                }

                if (!esNumero)
                {
                    cout << "Entrada inválida. Por favor, ingrese un número válido." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                seleccion = stoi(input);

                if (seleccion < 0 || seleccion > static_cast<int>(productos.size()) || find(seleccionados.begin(), seleccionados.end(), seleccion) != seleccionados.end())
                {
                    cout << "Selección inválida. Por favor, ingrese un valor válido." << endl;
                    esNumero = false; // Volver a solicitar la entrada
                }
            } while (!esNumero);

            if (seleccion == 0)
            {
                cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
                _getch();
                system("cls");
                return;
            }

            seleccionados.push_back(seleccion);
        }

        // Obtener cantidades a vender
        vector<int> cantidadesVendidas;
        for (int i = 0; i < cantidadProductos; i++)
        {
            int cantidadVendida;
            do
            {
                cout << "Ingrese la cantidad del producto #" << i + 1 << " a vender (o 0 para cancelar): ";
                cin >> input;
                // Verificar si la entrada es un número válido
                esNumero = true;
                for (char c : input)
                {
                    if (!isdigit(c))
                    {
                        esNumero = false;
                        break;
                    }
                }

                if (!esNumero)
                {
                    cout << "Entrada inválida. Por favor, ingrese un número válido." << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cantidadVendida = stoi(input);
                if (cantidadVendida < 0 || cantidadVendida > productos[seleccionados[i] - 1].cantidad)
                {
                    cout << "Cantidad inválida. Por favor, ingrese un valor válido." << endl;
                    esNumero = false; // Volver a solicitar la entrada
                }
            } while (!esNumero);

            if (cantidadVendida == 0)
            {
                cout << "Presiona cualquier tecla para continuar (Borrará la terminal)..." << endl;
                _getch();
                system("cls");
                return;
            }

            cantidadesVendidas.push_back(cantidadVendida);
        }

        // Actualizar cantidades de productos en el vector
        for (int i = 0; i < cantidadProductos; i++)
        {
            productos[seleccionados[i] - 1].cantidad -= cantidadesVendidas[i];
        }

        // Actualizar archivo de productos
        ofstream archivoActualizado("productos.txt");
        if (!archivoActualizado.is_open())
        {
            cout << "No se pudo abrir el archivo de productos para actualizar." << endl;
            return;
        }

        for (std::vector<Producto>::size_type i = 0; i < productos.size(); i++)
        {
            archivoActualizado << productos[i].nombre << " " << productos[i].cantidad << " " << productos[i].precio << endl;
        }
        archivoActualizado.close();

        // Calcular total de venta
        double totalVenta = 0.0;
        for (int i = 0; i < cantidadProductos; i++)
        {
            totalVenta += cantidadesVendidas[i] * productos[seleccionados[i] - 1].precio;
        }

        // Imprimir Boletas productos
        cout << "Por fabor, antes de emitir su boleta indicar su DNI:" << endl;
        // repetir hasta que sea valido el dni
        do
        {
            std::cout << "Ingrese su DNI (8 dígitos): ";
            std::cin >> cliente.n_dni;

            // Verificar la longitud del DNI
            if (cliente.n_dni.size() != 8)
            {
                std::cout << "*Error, DNI incorrecto. Ingrese su DNI nuevamente." << std::endl;
                continue;
            }

            // Verificar si todos los caracteres son dígitos
            bool esNumero = true;
            for (char c : cliente.n_dni)
            {
                if (!isdigit(c))
                {
                    esNumero = false;
                    break;
                }
            }

            if (!esNumero)
            {
                cout << "*Error, DNI incorrecto. Ingrese su DNI nuevamente." << std::endl;
                continue;
            }
            cliente.u_dni = cliente.n_dni;
            // Si el DNI es válido, se asigna a la variable cliente.n_dni
            break;
        } while (true);
        cliente.usuario = (rand() % 999);
        cliente.u_dni = "0";
        cout << "" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "MYPE: " << endl;
        cout << "" << endl;
        cout << "       _____   _____  ____    ____  _____    " << endl;
        cout << "      |_   _| | ____|/ ___| / ___|| ____|    " << endl;
        cout << "        | |   |  _|  \\___  \\___ | |___|    " << endl;
        cout << "        | |   | |___  ___) | ___) | |___     " << endl;
        cout << "        |_|   |_____|/____/ |____/|_____|    " << endl;
        cout << "                                             " << endl;
        cout << "" << endl;
        cout << "      R.U.C.: N 2056821563" << endl;
        cout << "        BOLETA ELECTRONICA" << endl;
        cout << "          N" << (rand() % 999999) << endl;
        cout << "" << endl;
        cout << "Cliente: U" << cliente.usuario << endl;
        cout << "DNI: " << cliente.n_dni << endl;
        cout << "" << endl;

        cout << left << setw(ESPACIO_NOMBRE) << "Nombre" << setw(ESPACIO_CANTIDAD) << "Cantidad" << setw(ESPACIO_PRECIO) << "Precio" << endl;
        cout << setfill('-') << setw(ESPACIO_NOMBRE + ESPACIO_CANTIDAD + ESPACIO_PRECIO) << "" << setfill(' ') << endl;
        for (int i = 0; i < cantidadProductos; i++)
        {
            cout << left << setw(ESPACIO_NOMBRE) << productos[seleccionados[i] - 1].nombre << setw(ESPACIO_CANTIDAD) << cantidadesVendidas[i] << "$" << setw(ESPACIO_PRECIO) << fixed << setprecision(2) << productos[seleccionados[i] - 1].precio << endl;
        }
        // Mostrar total de venta y despedida

        cout << "" << endl;
        cout << "Total:                        S/." << totalVenta << endl;
        cout << "IGV (2%)                      S/." << totalVenta * 2 / 100 << endl;
        cout << "PAGAR:                        S/." << float((totalVenta + totalVenta * 2 / 100)) << endl;
        cout << "" << endl;
        cout << "         Gracias por su preferencia!" << endl;
        cout << " Tu tienda de abarrotes TESSE de confianza" << endl;
        cout << "" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "" << endl;
        guardarVenta(productos, seleccionados, cantidadesVendidas, totalVenta);
        escribirTotalVentasDia();
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
    }

    friend class Menus;
};

class Menus
{
public:
    static void MenuEmpleado();
    static void MenuAdmin();
    static void MenuDueño();
};

class UsuariosLogin
{
private:
    string username;
    string password;
    string permiso;

public:
    friend class registroventa;
    friend class Menus;
    void PantallaSesion();

    bool iniciarSesion(const string &username, const string &password);

    void registrarUsuario();

    void mostrarUsuarios();
};

void UsuariosLogin::PantallaSesion()
{
    bool Identificador = false;
    int contador = 0;
    int Max = 3;

    do
    {
        cout << "-------------------------------------" << endl;
        cout << "1. Iniciar sesion" << endl;
        cout << "Nombre de usuario: ";
        cin >> username;
        cout << "Contrasena: ";
        cin >> password;
        ventanombreUsuario = username;

        if (iniciarSesion(username, password))
        {

            break;
        }
        else
        {
            contador++;
            cout << "Nombre de usuario o contrasena incorrectos" << endl;
            cout << "Le quedan " << --Max << " intentos " << endl;
        }

        if (contador == 3)
        {
            cout << "Demasiados intentos fallidos. Saliendo del programa......." << endl;
            exit(1);
        }
    } while (!Identificador);
}
bool UsuariosLogin::iniciarSesion(const string &username, const string &password)
{
    ifstream usuarios("usuarios.txt");
    string line;

    while (getline(usuarios, line))
    {
        vector<string> campos;
        size_t pos = 0;
        string token;

        while ((pos = line.find(' ')) != string::npos)
        {
            token = line.substr(0, pos);
            campos.push_back(token);
            line.erase(0, pos + 1);
        }

        campos.push_back(line);

        if (campos.size() != 3)
            continue;

        if (campos[0] == username && campos[1] == password)
        {
            usuarios.close();

            if (campos[2] == "owner")
            {
                cout << "-------------------------------------" << endl;
                cout << "Inicio de sesion exitoso como Owner." << endl;
                cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
                _getch();
                system("cls");
                Menus::MenuDueño();
            }
            else if (campos[2] == "admin")
            {
                cout << "-------------------------------------" << endl;
                cout << "Inicio de sesion exitoso como Administrador." << endl;
                cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
                _getch();
                system("cls");
                Menus::MenuAdmin();
            }
            else if (campos[2] == "empleado")
            {
                cout << "-------------------------------------" << endl;
                cout << "Inicio de sesion exitoso como Empleado." << endl;
                // Almacenar el nombre de usuario en una variable miembro de UsuariosLogin
                permisoderegistro = campos[2];
                cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
                _getch();
                system("cls");
                Menus::MenuEmpleado();
            }

            return true;
        }
    }
    usuarios.close();
    return false;
}

void UsuariosLogin::registrarUsuario()

{
    cout << "-------------------------------------" << endl;
    // Solicitar los datos del producto al usuario
    cout << "Ingrese el nombre del usuario (0 para cancelar): ";
    cin >> username;

    // Verificar si se ingresó "0" para cancelar la operación
    if (username == "0")
    {
        cout << "Operacion cancelada." << endl;
        cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
        _getch();
        system("cls");
        return;
    }

        cout << "Ingrese la contrasena: ";
    cin >> password;
    cout << "Ingrese el tipo de permiso (admin/empleado/owner): ";
    cin >> permiso;

    while (permiso != "admin" && permiso != "owner" && permiso != "empleado")
    {
        cout << "Tipo de permiso inválido. Ingrese 'admin' o 'empleado' o 'owner': ";
        cin >> permiso;
    }

    ofstream usuarios("usuarios.txt", ios::app);

    usuarios << username << " " << password << " " << permiso << endl;

    usuarios.close();
    cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
    _getch();
    system("cls");
}

void UsuariosLogin::mostrarUsuarios()
{
    ifstream usuarios("usuarios.txt");
    string line;
    cout << "Estos son todos los usuarios registrados:" << endl;
    while (getline(usuarios, line))
    {

        vector<string> campos;
        size_t pos = 0;
        string token;

        while ((pos = line.find(' ')) != string::npos)
        {
            token = line.substr(0, pos);
            campos.push_back(token);
            line.erase(0, pos + 1);
        }
        campos.push_back(line);

        if (campos.size() != 3)
        {
            continue;
        }
        cout << "-------------------------------------" << endl;
        cout << "Nombre de usuario: " << campos[0] << endl;
        cout << "Contrasena: " << campos[1] << endl;
        cout << "Permiso: " << campos[2] << endl;
    }

    usuarios.close();
    cout << "Presiona cualquier tecla para continuar (Borrara la terminal)..." << endl;
    _getch();
    system("cls");
}

void Menus::MenuEmpleado()
{
    Producto a1;
    Venta a2;
    UsuariosLogin a3;

    char opcion;

    do
    {
        cout << "-------------------------------------" << endl;
        cout << "        |||  MENU EMPLEADO |||       " << endl;
        cout << "-------------------------------------" << endl;
        cout << "Seleccione una opcion:\n";
        cout << "a) Mostrar lista de productos\n";
        cout << "b) Venta de productos\n";
        cout << "c) Iniciar sesion nuevamente\n";
        cout << "d) Salir\n";
        cin >> opcion;

        switch (opcion)
        {

        case 'a':
            a1.mostrarProductos();
            break;
        case 'b':
            a2.venderProducto();
            break;
        case 'c':
            a3.PantallaSesion();
            break;

        case 'd':
            cout << "Adios.\n";
            exit(1);
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }
    } while (opcion != 'd');
}

void Menus::MenuAdmin()
{
    Producto a1;
    Venta a2;
    UsuariosLogin a3;
    registroventa a4;

    char opcion;

    do

    {
        cout << "-------------------------------------" << endl;
        cout << "        |||  MENU ADMIN  |||         " << endl;
        cout << "-------------------------------------" << endl;
        cout << "Seleccione una opcion:\n";
        cout << "a) Mostrar lista de productos\n";
        cout << "b) Agregar producto\n";
        cout << "c) Eliminar producto\n";
        cout << "d) Iniciar sesion nuevamente\n";
        cout << "e) Mostrar precio de los productos\n";
        cout << "f) Ver historial de ventas (General)\n";
        cout << "g) Ver historial de ventas de los empleados\n";
        cout << "h) Salir\n";
        cin >> opcion;

        switch (opcion)
        {
        case 'a':
            a1.mostrarProductos();
            break;
        case 'b':
            a1.agregarProducto();
            break;
        case 'c':
            a1.eliminarProducto();
            break;
        case 'd':
            a3.PantallaSesion();
            break;
        case 'e':
            a1.ProductosCostos();
            break;

        case 'f':
            a2.verHistorialVentas();
            break;
        case 'g':
            a4.mostrarHistorialVentasEmpleado();
            break;

        case 'h':
            cout << "Adios.\n";
            exit(1);
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }
    } while (opcion != 'h');
}

void Menus::MenuDueño()
{
    Producto a1;
    Venta a2;
    UsuariosLogin a3;

    char opcion;

    do
    {
        cout << "-------------------------------------" << endl;
        cout << "        |||  MENU DUENO  |||         " << endl;
        cout << "-------------------------------------" << endl;

        cout << "Seleccione una opcion:\n";
        cout << "a) Mostrar todos los usuarios \n";
        cout << "b) Registrar nuevo usuario \n";
        cout << "c) Mostrar lista de productos\n";
        cout << "d) Venta de productos\n";
        cout << "e) Iniciar sesion nuevamente\n";
        cout << "f) Mostrar precio de los productos\n";
        cout << "g) Ver historial de ventas (General)\n";
        cout << "h) Ver historial de ventas de los empleados\n";
        cout << "i) Agregar producto\n";
        cout << "j) Eliminar producto\n";
        cout << "k) Salir\n";
        cin >> opcion;

        switch (opcion)
        {
        case 'a':
            a3.mostrarUsuarios();
            break;
        case 'b':
            a3.registrarUsuario();
            break;
        case 'c':
            a1.mostrarProductos();
            break;
        case 'd':
            a2.venderProducto();
            break;
        case 'e':
            a3.PantallaSesion();
            break;
        case 'f':
            a1.ProductosCostos();
            break;
        case 'g':
            a2.verHistorialVentas();
            break;
        case 'h':
            a2.mostrarHistorialVentasEmpleado();
            break;
        case 'i':
            a1.agregarProducto();
            break;
        case 'j':
            a1.eliminarProducto();
            break;
        case 'k':
            cout << "Adios.\n";
            exit(1);
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }
    } while (opcion != 'k');
}

int main()
{ // Interface del sistema ----------------------------------------
    cout << "**************************************************************************   " << endl;
    cout << "      _____            ______    ______   ______      __   __  ___  __          " << endl;
    cout << "     |       |     |  |      |  |        |      |    |  | |__| |__ |  |      " << endl;
    cout << "     |_____  |     |  | _____|  | ____   |   ___|    |__| |    |__ |  |      " << endl;
    cout << "           | |     |  |         |        |  |___                              " << endl;
    cout << "      _____| |_____|  |         |_____   |      |                             " << endl;
    cout << "      ____________________________________________________________________   " << endl;
    cout << "**************************************************************************   " << endl;
    cout << "Empresa: MarkerTESSE S.A.C  - R.U.C.: 2056821563" << endl;
    cout << "" << endl;
    UsuariosLogin usuario;
    usuario.PantallaSesion();

    return 0;
}
