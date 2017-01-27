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
    Console(); // ����������� �� ���������
    void run(); // ������ ����� ���������
    void prompt(); // ������� ��������� ������ � ������� ���������� ������������ � �������� �����
    void printFolderFromBeginning(Folder *currentFolder); // ������� ���� ���� �� ����� �� �������� ��������
    bool cd(const std::string &name); // ������� � ������ �����
    void ls(Folder *); // ������� ���������� �����
    void cat(); // ���������� � ������� ���-�� ��������� � ������� ����������
    void mkdir(std::string &name); // ������� �����
    void mkdir(const std::string &name); // ������ � ����������� ����������
    void create(std::string &name); // ������� ����
    void create(const std::string &name); // ������ � ����������� ����������
    void del(std::string &name); // ������� ����� ��� ����
    void del(const std::string &name); // ������ � ����������� ����������

    void setCurrentFolder(Folder *folder); // ��������� ������� �����
    Folder* getCurrentFolder() const; // �������� ����� ������� �����
private:
    Folder m_root; // �������� �����
    Folder *m_currentFolder; // ������� �����
}; // end class Console

#endif // CONSOLE_H
