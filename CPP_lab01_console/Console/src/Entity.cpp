// Entity.cpp

#include "Entity.h"


// конструктор по умолчанию
Entity::Entity()
{
    setName("");
    setParent(0);
}

// конструктор с параметрами
Entity::Entity(const std::string &name, Folder * const parent)
{
    setName(name);
    setParent(parent);
}

// установить адрес родителя
void Entity::setParent(Folder * const parent)
{
    m_parent = parent;
}

// получить адрес родителя
Folder* Entity::getParent() const
{
    return m_parent;
}

// установить имя
void Entity::setName(const std::string &name)
{
    m_name = name;
}

// получить имя
std::string Entity::getName() const
{
    return m_name;
}
