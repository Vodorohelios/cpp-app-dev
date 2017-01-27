// File.cpp

#include "File.h"
#include "Folder.h"
#include <iostream>
#include <cstring>
#include <windows.h>

#define EOF -1


// конструктор
File::File(const std::string &name, Folder *parent) : Entity(name, parent)
{
    write(this);
}

// записать (добавить) текст в файл
void File::write(File *file)
{
    std::cout << "(Enter ctrl+D to end input)" << std::endl;

    std::string wordBuffer("");

    getline(std::cin, wordBuffer);

    while (wordBuffer[wordBuffer.size()-1] != 4)
    {
        wordBuffer.push_back('\n');
        buffer.insert(buffer.size(), wordBuffer);
        getline(std::cin, wordBuffer);
    }
}

// переписать текст в файле
void File::rewrite(File *file)
{
    std::cout << "(Enter ctrl+D to end input)" << std::endl;

    std::string wordBuffer("");

    buffer.assign(wordBuffer);
    getline(std::cin, wordBuffer);

    while (wordBuffer[wordBuffer.size()-1] != 4)
    {
        wordBuffer.push_back('\n');
        buffer.insert(buffer.size(), wordBuffer);
        getline(std::cin, wordBuffer);
    }
}

// вывести содержимое файла
void File::read(File *file)
{
    std::cout << file->buffer << std::endl;
}

// удаление файла, который вызвал эту функцию
void File::deleteSelf()
{
    // возьмём адрес родителя
    Folder *parent = getParent();
    // и вызовим полное удаление им этого ребёнка,
    // тем самым удалив данный объект (папку) из вектора текущей папки
    // вместе с освобождением памяти
    parent->removeChild(this);
}
