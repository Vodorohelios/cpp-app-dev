// Folder.h

#ifndef FOLDER_H
#define FOLDER_H

#include "Entity.h"
#include <vector>


class Folder : public Entity
{
public:
    Folder(const std::string &name, Folder *parent); // �����������
    ~Folder(); // ����������
    size_t getNumberOfChilds() const; // �������� ���������� ��������� � ������ �����
    void addChild(Entity *child); // ������������ ������ � ������ �����
    void removeChild(const Entity *child); // ������� ������ � ���������� ������
    virtual void deleteSelf(); // �������� �����, ������� ������� ��� �������, ������ � � ���������� (���� �������)
    void printChildren(); // ������� �� ����� ����� ���� �������� � ������ �����

    // �� ����� ������� ����� � ������� ����� ��� ��������
    Entity* findChild(std::string &name);
    Entity* findChild(const std::string &name); // ������ � ����������� ����������
private:
    std::vector<Entity *> m_children; // ������, �������� "�����" ������ ����� (�.�. �������� ������ ����� � �����)
}; // end class Folder

#endif // FOLDER_H
