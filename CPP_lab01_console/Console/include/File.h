// File.h

#ifndef FILE_H
#define FILE_H

#include "Entity.h"
#include <string>

class File : public Entity
{
public:
    File(const std::string &name, Folder *parent); // �����������
    virtual void deleteSelf(); // �������� �����, ������� ������ ��� �������
    void write(File *file); // �������� (��������) ����� � ����
    void rewrite(File *file); // ���������� ����� � �����
    void read(File *file); // ������� ���������� �����
private:
    std::string buffer; // ����� ��� �������� ������ � �����
}; // end class File

#endif // FILE_H
