//------------------------------------------------------------------------------
// File:    shell.h
// Author:  Daniel Herce (NIP 848884), Alain Villagrasa (NIP 816787)
// Date:    Marzo 2023
// Coms:    Fichero que implementa la jerarquía de excepciones posibles sobre la 
//          clase Shell
//------------------------------------------------------------------------------

#pragma once

#include <exception>
#include <string>

class arbol_ficheros_error {
    protected:
        std::string _aux;
    public:
        arbol_ficheros_error(const std::string& aux = "") : _aux(aux) {}
        virtual const std::string what() const noexcept {
            return _aux;
        }
};

class negative_size : public arbol_ficheros_error {
    public:
        negative_size(const int sz) : arbol_ficheros_error(std::to_string(sz)) {}
        const std::string what() const noexcept override {
            return "Invalid parameter: size must be greater or equal to 0, was " + _aux;
        }
};

class is_a_directory : public arbol_ficheros_error {
    public:
        is_a_directory(const std::string& dirName) : arbol_ficheros_error(dirName) {}
        const std::string what() const noexcept override {
            return _aux + " is a directory";
        }
};

class dir_exists : public arbol_ficheros_error {
    public:
        dir_exists(const std::string& dirName) : arbol_ficheros_error(dirName) {}
        const std::string what() const noexcept override {
            return "The directory " + _aux + " already exists";
        }
};

class already_root : public arbol_ficheros_error {
    public:
        const std::string what() const noexcept override {
            return "Path already root, cannot cd ..";
        }
};

class is_a_file : public arbol_ficheros_error {
    public:
        is_a_file(const std::string& fileName) : arbol_ficheros_error(fileName) {}
        const std::string what() const noexcept override {
            return _aux + " is a file";
        }
};

class elem_not_found : public arbol_ficheros_error {
    public:
        elem_not_found(const std::string& elem) : arbol_ficheros_error(elem) {}
        const std::string what() const noexcept override {
            return _aux + " not found";
        }
};

