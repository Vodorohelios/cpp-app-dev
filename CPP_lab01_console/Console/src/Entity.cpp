// Entity.cpp

#include "Entity.h"


// ����������� �� ���������
Entity::Entity()
{
    setName("");
    setParent(0);
}

// ����������� � �����������
Entity::Entity(const std::string &name, Folder * const parent)
{
    setName(name);
    setParent(parent);
}

// ���������� ����� ��������
void Entity::setParent(Folder * const parent)
{
    m_parent = parent;
}

// �������� ����� ��������
Folder* Entity::getParent() const
{
    return m_parent;
}

// ���������� ���
void Entity::setName(const std::string &name)
{
    m_name = name;
}

// �������� ���
std::string Entity::getName() const
{
    return m_name;
}
