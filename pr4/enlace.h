//------------------------------------------------------------------------------
// File:   enlace.h
// Author: Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:   Marzo 2023
// Coms:   Fichero de implementación para la clase Enlace
//------------------------------------------------------------------------------

#pragma once

#include <memory>
#include "nodo.h"

class Enlace : public Nodo {
    private:
        std::shared_ptr<Nodo> _ref;
    public:
        // Constructor
        Enlace(const std::string& name, std::shared_ptr<Nodo> target)
            : Nodo(name), _ref(target) {}

        // Devuelve el tamaño del nodo apuntado por el enlace
        int getSize() const override {
            return _ref->getSize();
        }
        
        // Devuelve el nodo apuntado por el enlace
        std::shared_ptr<Nodo> link() {
            return _ref;
        }
};
