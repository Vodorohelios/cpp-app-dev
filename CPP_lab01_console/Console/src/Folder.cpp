// Folder.cpp

#include "Folder.h"
#include "Entity.h"
#include <iostream>
#include <algorithm> // для алгоритма find()


// конструктор
Folder::Folder(const std::string &name, Folder *parent)
    : Entity(name, parent)
{
    // пустое тело
}

// деструктор
Folder::~Folder()
{
    if (m_children.size() > 0)
    {
        // создаем итератор для прохождения по вектору m_children
        std::vector<Entity *>::iterator i = m_children.begin();

        // проходим по каждому "ребёнку" в векторе и удаляем его
        for ( ; i != m_children.end(); ++i)
        {
            delete *i;
        }
    }
}

// получить количество элементов в данной папке
size_t Folder::getNumberOfChilds() const
{
    return m_children.size();
}

// присоединить ребёнка к данной папке
void Folder::addChild(Entity *child)
{
    // сначала проверка (на случай, если понадобиться создать функции типа copy/cut):
    // сохраним адрес родителя в переменной parent
    Folder *parent = child->getParent();

    // если родитель имеется (не NULL) и не равен этой папке
    if (parent && parent != this)
        parent->removeChild(child); // удаляем ребёнка у старого родителя

    // Теперь, если ребёнок не содержится в новом родителе, то добавим его
    if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
    {
        m_children.push_back(child); // вставляем ребёнка в вектор детей родителя
        m_children.back()->setParent(this); // привязываем ребёнка к новому родителю
    }
}

// удаляем объект (ребенка) из вектора текущей папки (родителя) и освобождаем память
void Folder::removeChild(const Entity *child)
{
    // создаём итератор для прохождения по вектору
    std::vector<Entity *>::iterator location = std::find(m_children.begin(), m_children.end(), child);

    // если элемент действительно имеется в векторе детей, то удалим его
    if (location != m_children.end())
    {
        delete *location; // освобождаем память, занятую объектом, который мы хотим удалить
        m_children.erase(location); // удаляем соответствующий элемент в векторе
    }
}

// удаление папки, которая вызвала эту функцию, вместе с её содержимым (если имеется)
void Folder::deleteSelf()
{
    if (m_children.size() > 0) // если папка, которую мы хотим удалить, содержит в себе элементы, то...
    {
        // создаём итератор для прохождения по вектору
        std::vector<Entity *>::iterator i;

        // и удаляем всех детей, рекурсивно вызывая deleteSelf()
         for ( ; m_children.size() != 0; ) // for ( ; i != j; ++i)
            {
                // присваиваем новое начало вектора, так как в ф-ции removeChild() будет использовано m_children.erase(l);
                i = m_children.begin();
                (*i)->deleteSelf();
            }
    }

    // возьмём адрес родителя
    Folder *parent = getParent();
    // и вызовим полное удаление им этого ребёнка,
    // тем самым удалив данный объект (папку) из вектора текущей папки
    // вместе с освобождением памяти
    parent->removeChild(this);
}

// вывести на экран имена всех объектов в данной папке
void Folder::printChildren()
{
    // создаём итератор для прохождения по вектору
    std::vector<Entity *>::iterator i = m_children.begin();

    // выводим имя каждого ребёнка
    for ( ; i != m_children.end(); ++i)
    {
        std::cout << (*i)->getName() << "  ";
    }
}

// по имени объекта найти и вернуть адрес его родителя
Entity* Folder::findChild(std::string &name)
{
    // создаём итератор для прохождения по вектору
    std::vector<Entity *>::iterator i = m_children.begin();

    // проверяем каждый элемент в векторе
    for ( ; i != m_children.end(); ++i)
    {
        if ( name == (*i)->getName() ) // если искомое имя совпало с именем элемента в векторе,
            return *i;  // тогда возвращаем указатель на найденный объект
    }

    return 0; // иначе возвращаем нулевой указатель
}

// (версия с константным параметром) по имени объекта найти и вернуть адрес его родителя
Entity* Folder::findChild(const std::string &name)
{
    // создаём итератор для прохождения по вектору
    std::vector<Entity *>::iterator i = m_children.begin();

    // проверяем каждый элемент в векторе
    for ( ; i != m_children.end(); ++i)
    {
        if ( name == (*i)->getName() ) // если искомое имя совпало с именем элемента в векторе,
            return *i;  // тогда возвращаем указатель на найденный объект
    }

    return 0; // иначе возвращаем нулевой указатель
}
