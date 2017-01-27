// Folder.cpp

#include "Folder.h"
#include "Entity.h"
#include <iostream>
#include <algorithm> // ��� ��������� find()


// �����������
Folder::Folder(const std::string &name, Folder *parent)
    : Entity(name, parent)
{
    // ������ ����
}

// ����������
Folder::~Folder()
{
    if (m_children.size() > 0)
    {
        // ������� �������� ��� ����������� �� ������� m_children
        std::vector<Entity *>::iterator i = m_children.begin();

        // �������� �� ������� "������" � ������� � ������� ���
        for ( ; i != m_children.end(); ++i)
        {
            delete *i;
        }
    }
}

// �������� ���������� ��������� � ������ �����
size_t Folder::getNumberOfChilds() const
{
    return m_children.size();
}

// ������������ ������ � ������ �����
void Folder::addChild(Entity *child)
{
    // ������� �������� (�� ������, ���� ������������ ������� ������� ���� copy/cut):
    // �������� ����� �������� � ���������� parent
    Folder *parent = child->getParent();

    // ���� �������� ������� (�� NULL) � �� ����� ���� �����
    if (parent && parent != this)
        parent->removeChild(child); // ������� ������ � ������� ��������

    // ������, ���� ������ �� ���������� � ����� ��������, �� ������� ���
    if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
    {
        m_children.push_back(child); // ��������� ������ � ������ ����� ��������
        m_children.back()->setParent(this); // ����������� ������ � ������ ��������
    }
}

// ������� ������ (�������) �� ������� ������� ����� (��������) � ����������� ������
void Folder::removeChild(const Entity *child)
{
    // ������ �������� ��� ����������� �� �������
    std::vector<Entity *>::iterator location = std::find(m_children.begin(), m_children.end(), child);

    // ���� ������� ������������� ������� � ������� �����, �� ������ ���
    if (location != m_children.end())
    {
        delete *location; // ����������� ������, ������� ��������, ������� �� ����� �������
        m_children.erase(location); // ������� ��������������� ������� � �������
    }
}

// �������� �����, ������� ������� ��� �������, ������ � � ���������� (���� �������)
void Folder::deleteSelf()
{
    if (m_children.size() > 0) // ���� �����, ������� �� ����� �������, �������� � ���� ��������, ��...
    {
        // ������ �������� ��� ����������� �� �������
        std::vector<Entity *>::iterator i;

        // � ������� ���� �����, ���������� ������� deleteSelf()
         for ( ; m_children.size() != 0; ) // for ( ; i != j; ++i)
            {
                // ����������� ����� ������ �������, ��� ��� � �-��� removeChild() ����� ������������ m_children.erase(l);
                i = m_children.begin();
                (*i)->deleteSelf();
            }
    }

    // ������ ����� ��������
    Folder *parent = getParent();
    // � ������� ������ �������� �� ����� ������,
    // ��� ����� ������ ������ ������ (�����) �� ������� ������� �����
    // ������ � ������������� ������
    parent->removeChild(this);
}

// ������� �� ����� ����� ���� �������� � ������ �����
void Folder::printChildren()
{
    // ������ �������� ��� ����������� �� �������
    std::vector<Entity *>::iterator i = m_children.begin();

    // ������� ��� ������� ������
    for ( ; i != m_children.end(); ++i)
    {
        std::cout << (*i)->getName() << "  ";
    }
}

// �� ����� ������� ����� � ������� ����� ��� ��������
Entity* Folder::findChild(std::string &name)
{
    // ������ �������� ��� ����������� �� �������
    std::vector<Entity *>::iterator i = m_children.begin();

    // ��������� ������ ������� � �������
    for ( ; i != m_children.end(); ++i)
    {
        if ( name == (*i)->getName() ) // ���� ������� ��� ������� � ������ �������� � �������,
            return *i;  // ����� ���������� ��������� �� ��������� ������
    }

    return 0; // ����� ���������� ������� ���������
}

// (������ � ����������� ����������) �� ����� ������� ����� � ������� ����� ��� ��������
Entity* Folder::findChild(const std::string &name)
{
    // ������ �������� ��� ����������� �� �������
    std::vector<Entity *>::iterator i = m_children.begin();

    // ��������� ������ ������� � �������
    for ( ; i != m_children.end(); ++i)
    {
        if ( name == (*i)->getName() ) // ���� ������� ��� ������� � ������ �������� � �������,
            return *i;  // ����� ���������� ��������� �� ��������� ������
    }

    return 0; // ����� ���������� ������� ���������
}
