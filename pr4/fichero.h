//------------------------------------------------------------------------------
// File:   fichero.h
// Author: Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:   Marzo 2023
// Coms:   Fichero de implementación para la clase Fichero
//------------------------------------------------------------------------------

#pragma once

#include "nodo.h"
#include <memory>

class Fichero : public Nodo {
    private:
        int _size;
    public:
        Fichero(const std::string& name, int size = 0) : Nodo(name), _size(size) {}

        //Devuelve el tamaño del fichero
        virtual int getSize() const override {
            return _size;
        }

        void updateSize(const int size) {
            _size = size;
        }
};