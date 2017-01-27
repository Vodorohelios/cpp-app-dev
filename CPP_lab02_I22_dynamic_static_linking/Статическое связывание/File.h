// File.h

#ifndef FILE_H
#define FILE_H

#include "Entity.h"
#include <string>

class File : public Entity
{
public:
    File(const std::string &name, Folder *parent); // конструктор
    virtual void deleteSelf(); // удаление файла, который вызвал эту функцию
    void write(File *file); // записать (добавить) текст в файл
    void rewrite(File *file); // переписать текст в файле
    void read(File *file); // вывести содержимое файла
private:
    std::string buffer; // буфер для хранения текста в файле
}; // end class File

#endif // FILE_H
