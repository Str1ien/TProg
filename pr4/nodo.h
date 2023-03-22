//------------------------------------------------------------------------------
// File:   nodo.h
// Author: Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:   Marzo 2023
// Coms:   Fichero que implementa la clase Nodo
//------------------------------------------------------------------------------

#pragma once

#include <string>

class Nodo {
    protected:
        std::string _name;  // Nombre del nodo
    public:        
        // Constructor
        Nodo(const std::string& name) : _name(name) {}

        // Devuelve el nombre del nodo
        std::string getName() const {return _name;}

        // Devuelve el tamaño del nodo
        virtual int getSize() const = 0;
        
};
