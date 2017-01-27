// Folder.h

#ifndef FOLDER_H
#define FOLDER_H

#include "Entity.h"
#include <vector>


class Folder : public Entity
{
public:
    Folder(const std::string &name, Folder *parent); // конструктор
    ~Folder(); // деструктор
    size_t getNumberOfChilds() const; // получить количество элементов в данной папке
    void addChild(Entity *child); // присоединить ребёнка к данной папке
    void removeChild(const Entity *child); // удалить ребёнка и освободить память
    virtual void deleteSelf(); // удаление папки, которая вызвала эту функцию, вместе с её содержимым (если имеется)
    void printChildren(); // вывести на экран имена всех объектов в данной папке

    // по имени объекта найти и вернуть адрес его родителя
    Entity* findChild(std::string &name);
    Entity* findChild(const std::string &name); // версия с константным параметром
private:
    std::vector<Entity *> m_children; // вектор, хранящий "детей" данной папки (т.е. хранящий другие файлы и папки)
}; // end class Folder

#endif // FOLDER_H
