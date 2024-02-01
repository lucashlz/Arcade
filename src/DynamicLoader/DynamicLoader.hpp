/*
** EPITECH PROJECT, 2023
** DynamicLoader.hpp
** File description:
** DynamicLoader
*/

#pragma once

#include <stdexcept>
#include <dlfcn.h>
#include <memory>
#include <string>

namespace arcade
{
    template <class T>
    class DynamicLoader {
        public:
            using FunctionPtr = T*(*)(bool o);

            DynamicLoader() : _handle(nullptr) {}

            ~DynamicLoader()
            {
                if (this->_handle) dlclose(this->_handle);
            }

            std::unique_ptr<T> loadPlugin(std::string path, bool open_window = true)
            {
                if (this->_handle != nullptr) {
                    dlclose(this->_handle);
                    this->_handle = nullptr;
                }
                if (!(this->_handle = dlopen(path.c_str(), RTLD_LAZY))) throw std::runtime_error("dlopen failed: " + std::string(dlerror()));
                this->_create = reinterpret_cast<FunctionPtr>(dlsym(this->_handle, "createIt"));
                if (!this->_create) throw std::runtime_error("dynload: can't create instance");
                return std::unique_ptr<T>(this->_create(open_window));
            }

            void *getSym(std::string sym)
            {
                void *ret = nullptr;

                if (this->_handle) ret = dlsym(this->_handle, sym.c_str());
                return ret;
            }
        private:
            void *_handle;
            FunctionPtr _create;
    };
}