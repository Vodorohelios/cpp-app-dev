// File.cpp

#include "File.h"
#include "Folder.h"
#include <iostream>
#include <cstring>
#include <windows.h>

#define EOF -1


// �����������
File::File(const std::string &name, Folder *parent) : Entity(name, parent)
{
    write(this);
}

// �������� (��������) ����� � ����
void File::write(File *file)
{
    std::cout << "(Enter ctrl+D to end input)" << std::endl;

    std::string wordBuffer("");

    getline(std::cin, wordBuffer);

    while (wordBuffer[wordBuffer.size()-1] != 4)
    {
        wordBuffer.push_back('\n');
        buffer.insert(buffer.size(), wordBuffer);
        getline(std::cin, wordBuffer);
    }
}

// ���������� ����� � �����
void File::rewrite(File *file)
{
    std::cout << "(Enter ctrl+D to end input)" << std::endl;

    std::string wordBuffer("");

    buffer.assign(wordBuffer);
    getline(std::cin, wordBuffer);

    while (wordBuffer[wordBuffer.size()-1] != 4)
    {
        wordBuffer.push_back('\n');
        buffer.insert(buffer.size(), wordBuffer);
        getline(std::cin, wordBuffer);
    }
}

// ������� ���������� �����
void File::read(File *file)
{
    std::cout << file->buffer << std::endl;
}

// �������� �����, ������� ������ ��� �������
void File::deleteSelf()
{
    // ������ ����� ��������
    Folder *parent = getParent();
    // � ������� ������ �������� �� ����� ������,
    // ��� ����� ������ ������ ������ (�����) �� ������� ������� �����
    // ������ � ������������� ������
    parent->removeChild(this);
}
