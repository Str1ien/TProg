//------------------------------------------------------------------------------
// File:   shell.h
// Author: Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:   Marzo 2023
// Coms:   Fichero que implementa la clase Shell
//------------------------------------------------------------------------------

#pragma once

#include <string>
#include <memory>
#include <list>
#include "fichero.h"
#include "enlace.h"
#include "directorio.h"
#include "arbol_ficheros_error.h"

class Shell {
    private:
        std::list<std::shared_ptr<Directorio>> _rutaActiva;
        std::string _ruta;
    public:
        // Constructor
        Shell() : _ruta("/") {
            _rutaActiva.push_back(std::make_shared<Directorio>(""));
        }

        // Devuelve la ruta completa de forma textual, con todos los nombres de los directorios desde la raíz hasta
        // el directorio actual concatenados y separados por el separador '/'.
        std::string pwd() const {
            return _ruta;
        }

        // Devuelve un listado con el nombre de todos los nodos contenidos en la ruta actual, uno por línea.
        std::string ls() const {
            return _rutaActiva.back()->print("ls");
        }

        // Devuelve un listado con el nombre y el tamaño de todos los nodos contenidos en la ruta actual, uno por
        // línea.
        std::string du() const {
            return _rutaActiva.back()->print("du");
        }

        // Edita el fichero de nombre 'name' (en el directorio actual). Para simular la edición, simplemente se cambia
        // el tamaño del fichero al valor especificado como parámetro. Si el fichero no existe, se debe crear con
        // el nombre y tamaño especificados.
        void vi(const std::string& name, const int size) {
            if (size < 0) {
                throw negative_size(size);
            }
            // Buscamos si hay algun nodo de nombre <name>
            std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(name);
            if (elem == nullptr) { // Si no existe, lo añadimos
                _rutaActiva.back()->addNode(std::make_shared<Fichero>(name, size));
            } else { // Si existe...
                // Si elem es un shared_ptr<Enlace>, tomamos el enlace (de forma sucesiva)
                while (dynamic_pointer_cast<Enlace>(elem) != nullptr) {
                    std::shared_ptr<Enlace> ref = dynamic_pointer_cast<Enlace>(elem);
                    elem = ref->link();
                }

                // Si elem es un fichero, lo actualizamos
                if (dynamic_pointer_cast<Fichero>(elem) != nullptr) {
                    std::shared_ptr<Fichero> fichero = dynamic_pointer_cast<Fichero>(elem);
                    fichero->updateSize(size);
                } else { // Si elem no es ninguna de las anteriores, es un directorio => excepción
                    throw is_a_directory(elem->getName());
                }
            }
        }

        // Crea un directorio de nombre 'name' en el directorio activo.
        void mkdir(const std::string& name) {
            std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(name);
            if (elem == nullptr) { // Si no existe un nodo con nombre <name> lo añadimos
                _rutaActiva.back()->addNode(std::make_shared<Directorio>(name));
            } else { // Sino excepción
                throw dir_exists(name);
            }
        }

        // Hace que la ruta activa pase a referenciar a otro directorio.
        // La nueva ruta activa definida en 'path' debe referenciar un directorio o un enlace a un directorio.
        void cd(std::string path) {
            if (path != ".") { // Caso contrario, no hay que hacer nada (ya estamos en el propio directorio)
                if (path == "/") { // Si el path es "/" se vuelve a root directamente
                    std::shared_ptr<Directorio> root = _rutaActiva.front();
                    _rutaActiva.clear();
                    _rutaActiva.push_back(root);
                    _ruta = "/";
                } else if (path == "..") { // Si es ".." subimos al padre
                    if (_rutaActiva.back()->getName() != "") {
                        _rutaActiva.pop_back();
                        int pos = _ruta.find_last_of("/");
                        if (_rutaActiva.back()->getName() == "") // Mi padre es root
                            _ruta = "/";
                        else
                            _ruta = _ruta.substr(0, pos);
                    } else {
                        throw already_root();
                    }
                } else { // Es una ruta más compleja
                    if (path.find("/") == 0) { // Hay una barra al comienzo, vamos a root primero
                        cd("/");
                        path = path.substr(1);
                    }
                    std::shared_ptr<Nodo> elem;
                    bool fin = false;
                    while (!fin) { // Mientras queden directorios en el path por recorrer
                        int pos = path.find("/");
                        if (pos == -1) { // Ya no quedan "/"
                            fin = true;
                        }
                        std::string name = path.substr(0, pos);
                        if (name == ".") { // El propio directorio 
                            cd(".");
                        } else if (name == "..") { // Directorio padre
                            cd("..");
                        } else { // Otro directorio, lo buscamos
                            elem = _rutaActiva.back()->findNode(name);
                            if (elem != nullptr) { // Si se ha encontrado
                                // Si elem es un shared_ptr<Enlace>, tomamos el enlace (de forma sucesiva)
                                while (dynamic_pointer_cast<Enlace>(elem) != nullptr) {
                                    std::shared_ptr<Enlace> ref = dynamic_pointer_cast<Enlace>(elem);
                                    elem = ref->link();
                                }
                                // Si elem es un directorio
                                if (dynamic_pointer_cast<Directorio>(elem) != nullptr) {
                                    _rutaActiva.push_back(dynamic_pointer_cast<Directorio>(elem));
                                    if (_ruta == "/") {
                                        _ruta += name;
                                    } else {
                                        _ruta += "/" + name;
                                    }
                                } else { // Si elem no es ninguna de las anteriores, es un fichero => excepción
                                    throw is_a_file(name);
                                }
                            } else { // Si no se ha encontrado, excepción
                                throw elem_not_found(name);
                            }
                        } // Eliminamos del path el directorio al que se ha viajado
                        path = path.substr(pos + 1);
                    }
                }
            }
        }

        // Crea en el directorio actual un enlace simbólico de nombre 'name' que apunta al elemento identificado
        // mediante la ruta especificada en 'path', que puede ser de cualquier tipo. El nombre 'name' es un nombre
        // simple de nodo (se creará en el directorio activo), por lo que no puede contener una ruta completa.
        // La ruta definida en 'path' sí, de tal modo que se puede crear un enlace a un elemento en otro directorio
        // del árbol, que debe existir previamente.
        void ln(std::string& path, const std::string& name) {
            std::string dirActual = pwd();

            if (path == ".") { // Si el path es el directorio actual
                std::shared_ptr<Enlace> link = make_shared<Enlace>(name, dynamic_pointer_cast<Nodo>(_rutaActiva.back()));
            } else if (path == "..") {
                cd(".."); // Si el path es el directorio padre
                std::shared_ptr<Enlace> link = make_shared<Enlace>(name, dynamic_pointer_cast<Nodo>(_rutaActiva.back()));
                cd(dirActual);
                _rutaActiva.back()->addNode(link);
            } else { // En cualquier otro caso
                int pos = path.find_last_of("/");
                if (pos != -1) { // Hay barras en el path luego hay que recorrer la ruta hasta el directorio con el nodo
                    std::string dir = path.substr(0, pos);
                    path = path.substr(pos + 1);
                    if (path == ".") { // Si es el siguiente paso es el propio directorio
                        ln (dir, name);
                    } else if (path == "..") { // Si el siguiente paso es el directorio padre
                        pos = dir.find_last_of("/");
                        dir = dir.substr(0, pos);
                        ln(dir, name);
                    } else { // En cualquier otro caso, viajar a ese directorio
                        cd (dir);
                        std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(path); // Buscar el elemento
                        if (elem != nullptr) { // Si está, hacer el enlace, volver y añadirlo
                            std::shared_ptr<Enlace> link = make_shared<Enlace>(name, elem);
                            cd(dirActual);
                            _rutaActiva.back()->addNode(link);
                        } else { // Sino excepción
                            cd(dirActual);
                            throw elem_not_found(name);
                        }
                    }
                } else { // Si no hay barras de ruta ya estamos en el directorio donde esta el nodo que buscamos.
                    // Buscamos el nodo 
                    std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(path);
                    if (elem != nullptr) { // si existe añadimos el enlace
                        _rutaActiva.back()->addNode(make_shared<Enlace>(name, elem));
                    } else { // Si no existe excepción
                        throw elem_not_found(name);
                    }
                }
            }
        }

        // Devuelve el tamaño del nodo que referencia el path.
        int stat(std::string& path) {
            std::string dirActual = pwd();
            if (path == ".") { // Si es el propio directorio en el que estoy
                return _rutaActiva.back()->getSize();
            } else if (path == "..") { // Si es el directorio padre
                cd("..");
                int size = _rutaActiva.back()->getSize();
                cd(dirActual);
                return size;
            } else { // Sino...
                int pos = path.find_last_of("/");
                if (pos != -1) { // Si se ha encontrado "/"
                    std::string dir = path.substr(0, pos);
                    path = path.substr(pos + 1);
                    if (path == ".") { // Mismo directorio
                        return stat(dir);
                    } else if (path == "..") { // Directorio padre
                        pos = dir.find_last_of("/");
                        dir = dir.substr(0, pos);
                        return stat(dir);
                    } else { // En otro caso, viajamos a ese directorio
                        cd(dir);
                        std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(path);
                        if (elem != nullptr) { // Si existe el nodo, guardamos su tamaño y volvemos
                            int size = elem->getSize();
                            cd(dirActual);
                            return size;
                        } else { // Sino, volvemos y excepción
                            cd(dirActual);
                            throw elem_not_found(elem->getName());
                        }
                    }
                } else { // Buscamos el nodo
                    std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(path);
                    if (elem != nullptr) { // Si el nodo existe, devolvemos su tamaño
                        return elem->getSize();
                    } else { // Sino, excepción
                        throw elem_not_found(path);
                    }
                }
            }
        }

        // Elimina un nodo. Si es un fichero, es simplemente eliminado. Si es un enlace, elimina el enlace pero no
        // el nodo referenciado. Si es un directorio, elimina el directorio y todo su contenido. Si existen enlaces al
        // elemento borrado, ese elemento sigue siendo accesible a traves del enlace (todavía existe), pero no a
        // través de su ubicación original (que ha sido eliminada).
        void rm(std::string& path) {
            std::string dirActual = pwd();
            int pos = path.find_last_of("/");
            if (pos != -1) { // Si es una ruta compuesta, viajamos al path
                std::string dir = path.substr(0, pos);
                path = path.substr(pos + 1);
                cd(dir);
            }
            // Buscamos el nodo en el directorio
            std::shared_ptr<Nodo> elem = _rutaActiva.back()->findNode(path);
            if (elem != nullptr) { // Si existe
                if (dynamic_pointer_cast<Fichero>(elem) != nullptr) { // Si es un fichero
                    _rutaActiva.back()->delNode(dynamic_pointer_cast<Fichero>(elem));
                } else if (dynamic_pointer_cast<Enlace>(elem) != nullptr) { // Si es un enlace
                    _rutaActiva.back()->delNode(dynamic_pointer_cast<Enlace>(elem));
                } else { // Si es un directorio
                    _rutaActiva.back()->delNode(dynamic_pointer_cast<Directorio>(elem));
                }
            } else { // Si no, excepción
                throw elem_not_found(path);
            }
            cd(dirActual);
        }
};
