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

class arbol_ficheros_error : public std::exception {
    protected:
        std::string _aux;
    public:
        arbol_ficheros_error(const std::string& aux = "") : _aux(aux) {}
        virtual const char* what() const noexcept {
            return _aux.c_str();
        }
};

class negative_size : public arbol_ficheros_error {
    public:
        negative_size(const int sz) : arbol_ficheros_error(std::to_string(sz)) {}
        const char* what() const noexcept override {
            std::string s = "Invalid parameter: size must be greater or equal to 0, was " + _aux;
            return s.c_str();
        }
};

class is_a_directory : public arbol_ficheros_error {
    public:
        is_a_directory(const std::string& dirName) : arbol_ficheros_error(dirName) {}
        const char* what() const noexcept override {
            std::string s = _aux + " is a directory";
            return s.c_str();
        }
};

class dir_exists : public arbol_ficheros_error {
    public:
        dir_exists(const std::string& dirName) : arbol_ficheros_error(dirName) {}
        const char* what() const noexcept override {
            std::string s = "The directory " + _aux + " already exists";
            return s.c_str();
        }
};

class already_root : public arbol_ficheros_error {
    public:
        const char* what() const noexcept override {
            return "Path already root, cannot cd ..";
        }
};

class is_a_file : public arbol_ficheros_error {
    public:
        is_a_file(const std::string& fileName) : arbol_ficheros_error(fileName) {}
        const char* what() const noexcept override {
            std::string s = _aux + " is a file";
            return s.c_str();
        }
};

class elem_not_found : public arbol_ficheros_error {
    public:
        elem_not_found(const std::string& elem) : arbol_ficheros_error(elem) {}
        const char* what() const noexcept override {
            std::string s = _aux + " not found";
            return s.c_str();
        }
};
