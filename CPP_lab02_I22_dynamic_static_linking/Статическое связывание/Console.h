// Console.h
// Author: Alexandr Jlobici
// Gr. I22
// March 2016


#ifndef CONSOLE_H
#define CONSOLE_H

#include "Folder.h"

class Console
{
public:
    Console(); // конструктор по умолчанию
    void run(); // запуск цикла программы
    void prompt(); // вывести командную строку с текущим положением пользователя в иерархии папок
    void printFolderFromBeginning(Folder *currentFolder); // вывести весь путь от корня до текущего каталога
    bool cd(const std::string &name); // перейти в другую папку
    void ls(Folder *); // вывести содержимое папки
    void cat(); // подсчитать и вывести кол-во элементов в текущей директории
    void mkdir(std::string &name); // создать папку
    void mkdir(const std::string &name); // версия с константным параметром
    void create(std::string &name); // создать файл
    void create(const std::string &name); // версия с константным параметром
    void del(std::string &name); // удалить папку или файл
    void del(const std::string &name); // версия с константным параметром

    void setCurrentFolder(Folder *folder); // назначить текущую папку
    Folder* getCurrentFolder() const; // получить адрес текущей папки
private:
    Folder m_root; // корневая папка
    Folder *m_currentFolder; // текущая папка
}; // end class Console

#endif // CONSOLE_H
