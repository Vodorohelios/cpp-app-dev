// Entity.h
// �����, �� �������� ��������� ������� Folder � File
#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#define DLLEXPORT __declspec(dllexport)

// ��������������� ���������� ������ Folder,
// ����� �� ����� ������� ��������� Folder *m_parent
class Folder;


// ����������� ������� ����� ��� Folder � File
class DLLEXPORT Entity
{
public:
    Entity(); // ����������� �� ���������
    Entity(const std::string &name, Folder * const parent); // ����������� � �����������
    virtual ~Entity() {} // ����������� ����������

    // set � get �������
    void setParent(Folder * const parent);
    Folder* getParent() const;

    void setName(const std::string &name);
    std::string getName() const;

    // ������ ����������� �������, ������� ������� ���� �����, ���� ����,
    // � ����������� �� ���� ���������� � �������
    virtual void deleteSelf() = 0;
private:
    std::string m_name; // ��� ����� ��� �����
    Folder *m_parent; // ����������, ���������� ����� ��������
}; // end class Entity

#endif // ENTITY_H
