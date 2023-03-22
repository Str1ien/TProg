//------------------------------------------------------------------------------
// File:   directorio.h
// Author: Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:   Marzo 2023
// Coms:   Fichero de implementación para la clase <Directorio>
//------------------------------------------------------------------------------

#pragma once

#include <list>
#include <sstream>
#include <memory>
#include "nodo.h"

class Directorio : public Nodo {
    private:
        std::list<std::shared_ptr<Nodo>> _children;    //? Hay que liberarlos en un destructor???
    public:
        // Constructor
        Directorio(const std::string& name) : Nodo(name) {}

        // Devuelve el tamaño del directorio, resultado de la suma de todos sus ficheros
        int getSize() const override {
            int sz = 0;
            for (auto i : _children) 
                sz += i->getSize();
            return sz;
        }

        // Añade un nodo al directorio
        void addNode(std::shared_ptr<Nodo> node) {
            _children.push_back(node);
        }

        // Elimina un nodo del directorio
        void delNode(std::shared_ptr<Nodo> node) {
            _children.remove(node);
        }

        // Busca un nodo en el directorio con nombre <name>. SI lo encuentra devuelve un puntero
        // al nodo, sino devuelve nullptr
        std::shared_ptr<Nodo> findNode(const std::string& name) {
            for (const auto& i : _children) {
                if (i->getName() == name)
                    return i;
            }

            return nullptr;
        }
    
        // Imprime por pantalla, a uno por línea, los nombres de los nodos en el directorio
        // (si <cmd> lo indica, se imprime también el tamaño del nodo)
        std::string print(const std::string& cmd) const {
            std::stringstream ss;

            for (auto i : _children) {
                ss << i->getName(); 
                if (cmd == "du")
                    ss << ", " << i->getSize();
                ss << std::endl;
            }
            return ss.str();
        }
};