// Console.cpp


#include "Console.h"
#include "Folder.h"
#include "File.h"
#include <iostream>
#include <string>
#include <algorithm> // ��� transform()


// �����������
Console::Console() : m_root("/", 0)
{
    // �� ��������� ������� ������ �������� ������
    setCurrentFolder(&m_root);
}

// ������ ����� ���������
void Console::run()
{
    std::string input; // ���������� ��� �������� ����� ������������

    // ����� �� ����� �����������
    std::cout << "Welcome to Console [Version 3.0]" << std::endl;
    std::cout << "Type \"help\" for instructions.\n" << std::endl;

    while (true)
    {
        // ������� ��������� ������ � ������� ���������� ������������ � �������� �����
        prompt();

        // �������� ���� ������������, �������� ��� � ���������� input
        std::getline(std::cin, input);

        // ����������� ��� ����� �������� �������� � ������
        std::transform(input.begin(), input.end(), input.begin(), tolower);

        /* MENU */
        // � ����������� �� ����, ��� ������������ ���, ��������� ���� �� ��������� ������:

        // ����� �� �������
        if (input == "exit")
        {
            std::cout << "\n Termination..." << std::endl;
            break;
        }

        // ������� � ������ �����
        else if (input.substr(0, 3) == "cd ")
        {
            if (input.size() > 3)
            {
                // ���� ��� ����� ������ ����� � �������� �������
                if (input.size() == 4 && input.substr(3, 1) == "/")
                    m_currentFolder = &m_root;

                // �����, ���� ��� ����� ���������� ���� (������������ �� '/')
                else if (input.substr(3, 1) == "/")
                {
                    m_currentFolder = &m_root; // ��������� � ������, ����� �� ���� ��������������� ����� � ������ �����
                    // ������� ���������, �� ���������� � ������ "cd "
                    cd(input.substr(3, std::string::npos)); // std::npos ��� ��������� ����-���������, ������� �������� "�� ����� ������"
                }

                else // �����, ���� ��� ����� ������������� ����
                    cd(input.substr(3, std::string::npos));

                std::cout << std::endl;
            }
        }

        // ���� "cd" ��� ���������
        else if (input.substr(0, 2) == "cd")
        {
            // ������ �� ������
        }

        // ������� ���������� ������� �����
        else if (input == "ls")
        {
            ls(m_currentFolder);
        }

        // ������� ���������� ��������� �����
        else if (input.substr(0, 3) == "ls ")
        {
            if (input.size() > 3)
            {
                // �������� � folderName ��� �����
                std::string folderName = input.substr(3, std::string::npos);

                // ������ ��������� �� �����, ������������ � ������� ��������
                Entity *childPtr = m_currentFolder->findChild(folderName);
                // ����������� ��������� � ��� Folder, ����� ������� ��������������� �������
                Folder *folderPtr = dynamic_cast<Folder *>(childPtr);

                if (folderPtr != 0) // ���� folderPtr �������� ��������� �� ������������ �����
                    ls(folderPtr);
                else
                    std::cout << "No such directory\n" << std::endl;
            }
            else // ���� �������� �� ������, �� ������� ���������� ������� �����
                ls(m_currentFolder);
        }

        // ���������� � ������� ���-�� ��������� � ������� ����������
        else if (input == "cat")
        {
            cat();
        }

        // ������� ������� ����������
        else if (input == "pwd")
        {
            printFolderFromBeginning(m_currentFolder);
            std::cout << std::endl << std::endl;
        }

        // ������� �����
        else if (input.substr(0, 6) == "mkdir ")
        {
            if (input.size() > 6)
            {
                mkdir(input.substr(6, std::string::npos));
                std::cout << std::endl;
            }
        }

        // ������� ����
        else if (input.substr(0, 7) == "create ")
        {
            if (input.size() > 7)
            {
                create(input.substr(7, std::string::npos));
                std::cout << std::endl;
            }
        }

        // ������� ����� ��� ����
        else if (input.substr(0, 4) == "del ")
        {
            if (input.size() > 4)
            {
                del(input.substr(4, std::string::npos));
                std::cout << std::endl;
            }
        }

        // �������� (��������) ����� � ����
        else if (input.substr(0, 6) == "write ")
        {
            if (input.size() > 6)
            {
                // ����, ���� ��� ����� ���������� � ������� ��������
                Entity *ptr = m_currentFolder->findChild(input.substr(6, std::string::npos));
                // ����������� ��� ��� ������������� ������� write()
                File *filePtr = dynamic_cast<File *>(ptr);

                // ���� ���� ����������
                if (filePtr != 0)
                {
                    filePtr->write(filePtr); // ��������� � ���� �����
                    std::cout << std::endl;
                }
            }
        }

        // ���������� ����� � �����
        else if (input.substr(0, 8) == "rewrite ")
        {
            if (input.size() > 8)
            {
                // ����, ���� ��� ����� ���������� � ������� ��������
                Entity *ptr = m_currentFolder->findChild(input.substr(8, std::string::npos));
                // ����������� ��� ��� ������������� ������� write()
                File *filePtr = dynamic_cast<File *>(ptr);

                // ���� ���� ����������
                if (filePtr != 0)
                {
                    filePtr->rewrite(filePtr); // ��������� � ���� �����
                    std::cout << std::endl;
                }
            }
        }

        // ������� ���������� �����
        else if (input.substr(0, 5) == "read ")
        {
            if (input.size() > 5)
            {
                // ����, ���� ��� ����� ���������� � ������� ��������
                Entity *ptr = m_currentFolder->findChild(input.substr(5, std::string::npos));
                // ����������� ��� ��� ������������� ������� read()
                File *filePtr = dynamic_cast<File *>(ptr);

                // ���� ���� ����������
                if (filePtr != 0)
                {
                    filePtr->read(filePtr); // ������� ���������� �����
                }
            }
        }

        // ���� ������ ������ enter ��� �������
        else if (input == "")
        {
            // ������ �� ������
        }

        // ������� ����������
        else if (input == "help")
        {
            // ������� ������ ��������� ������
            std::cout << "\n Available commands:" << std::endl;
            std::cout << "--------------------\n" << std::endl;
            std::cout << "help - displays the available commands." << std::endl;
            std::cout << "exit - exit the console." << std::endl;
            std::cout << "cd - change directory." << std::endl;
            std::cout << "ls - list content of current folder." << std::endl;
            std::cout << "cat - count and print number of elements in current directory." << std::endl;
            std::cout << "pwd - print working directory." << std::endl;
            std::cout << "mkdir - create directory." << std::endl;
            std::cout << "create - create file." << std::endl;
            std::cout << "del - delete file or directory (with or without content)." << std::endl;
            std::cout << "write - write (append) text in file." << std::endl;
            std::cout << "rewrite - rewrite text in file." << std::endl;
            std::cout << "read - print content of file.\n\n" << std::endl;
        }

        // ���� ����� ����������� �������
        else
        {
            std::cout << "Unknown command. Type \"help\" for assistance\n" << std::endl;
        }
    }
}

// ������� ��������� ������ � ������� ���������� ������������ � �������� �����
void Console::prompt()
{
    // ������� ���� ���� �cd(input.substr(3, std::string::npos)); // std::npos ��� ��������� ����-���������, ������� �������� "�� ����� ������"� ����� �� �������� ��������
    printFolderFromBeginning(m_currentFolder);

    std::cout << ">";
}

// ������� ���� ���� �� ����� �� �������� ��������
void Console::printFolderFromBeginning(Folder *currentFolder)
{
    Folder *parent = currentFolder->getParent();

    if (parent != 0)
    {
        printFolderFromBeginning(parent);
    }

    // ���� ��� �� �������� �������, � �������� ���� ����� �� �������� �������� ���������
    if (parent != 0 && parent->getName() != "/")
        std::cout << "/"; // ��������� �������� ��������� ����� �������� '/'

    std::cout << currentFolder->getName();
}

// ������� � ������ �����
bool Console::cd(const std::string &name)
{
    // ���� ��� ������� ������� �� ���� ������� �����, ��
    if (name == "..")
    {
        setCurrentFolder(m_currentFolder->getParent());
        return true;
    }

    // �����, ���� ������ ������ -- ��� ����������� '/', ��
    else if (name.substr(0, 1) == "/")
    {
        bool status;
        // ����, ���� ������ ��� �������� ����������� '/'
        size_t position = name.find('/', 1);

        // ���� ����������� '/' ��� ������
        if (position != std::string::npos)
        {
            // ������� ���������� cd() � ��������� �����, ����� ������������ �� ���� �������
            status = cd (name.substr(1, position - 1));

            // ���� ����� �� ����������
            if (status == false)
                return false; // �� �� �� ����������

            else // �����, ���������� �������� cd(), ����� ���������� ���������� �����
                cd(name.substr(position, std::string::npos));
        }

        else // ���� ������ �� �������� ������ ������������ '/', �� ����� ���������� ��������� ��� ���������� ������ �-��� cd()
            cd(name.substr(1, std::string::npos));

        return true;
    }

    else // �����, ��������� � ���� �� ����� �������� ��������
    {
        // ������ ������ ���������� (��� ���������� ����� ����������� ��������� ����������� �� ����� ����������� '/'
        if (name == "")
            return true;

        size_t strSize = name.size();
        std::string name2;

        // ���� ������ �� ����� �������� '/'
        if (name.substr(strSize - 1, 1) == "/")
            name2 = name.substr(0, strSize - 1); // �� �� ��� �������
        else
            name2 = name; // ����� ��������� ��� ����

        Entity *ptr = m_currentFolder->findChild(name2);

        // ���� ������ � ����� ������ ����������
        if (ptr != 0)
        {
            // ����������� � ��� Folder, ����� ������������ ������������� �-��� setCurrentFolder()
            Folder *folder = dynamic_cast<Folder *>(ptr);

            // ���� folder �������� ��������� �� ������, � �� ����, �� ��� �����, � �� ����, � ����� ��������� �-��� setCurrentFolder()
            if (folder != 0)
            {
                setCurrentFolder(folder); // ������ ������� �����
                return true;
            }

            else // ����� ������� ��������� �� ������
            {
                std::cout << "cd: " << name2 << " is not a directory" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << name2 << ": No such directory\n";
            return false;
        }
    }
}

// ������� ���������� �����
void Console::ls(Folder *folder)
{
    if (folder->getNumberOfChilds() > 0)
    {
        folder->printChildren();
        std::cout << std::endl << std::endl;
    }
    else
        std::cout << std::endl;
}

// ������� �����
void Console::mkdir(std::string &name)
{
    Folder *ptr = new Folder(name, 0); // �������� ������ � �������� �������� ������� ���������
    m_currentFolder->addChild(ptr); // ������� addChild ���� ����������������� �������� ����� �����
}

// ������� ����� (������ � ����������� ����������)
void Console::mkdir(const std::string &name)
{
    Folder *ptr = new Folder(name, 0); // �������� ������ � �������� �������� ������� ���������
    m_currentFolder->addChild(ptr); // ������� addChild ���� ����������������� �������� ����� �����
}

// ������� ����
void Console::create(std::string &name)
{
    File *ptr = new File(name, 0); // �������� ������ � �������� �������� ������� ���������
    m_currentFolder->addChild(ptr); // ������� addChild ���� ����������������� �������� ������ �����
}

// ������� ���� (������ � ����������� ����������)
void Console::create(const std::string &name)
{
    File *ptr = new File(name, 0); // �������� ������ � �������� �������� ������� ���������
    m_currentFolder->addChild(ptr); // ������� addChild ���� ����������������� �������� ������ �����
}

// ������� ����� ��� ���� (� ������� ������������ -- ����� ����������� ������� Entity::deleteSelf())
void Console::del(std::string &name)
{
    // ������� �� ����� ������� ����� ������� (����� ��� �����), �������� ��� � ��������� ptr
    Entity *ptr = m_currentFolder->findChild(name);

    // �, ���� ��� ���� ������� � ������� �����,
    if (ptr != 0)
        ptr->deleteSelf(); // �� ������� ������ (����� ��� ����)
}

// (������ � ����������� ����������) ������� ����� ��� ���� (� ������� ������������ -- ����� ����������� ������� Entity::deleteSelf())
void Console::del(const std::string &name)
{
    // ������� �� ����� ������� ����� ������� (����� ��� �����), �������� ��� � ��������� ptr
    Entity *ptr = m_currentFolder->findChild(name);

    // � ���������, ���� ��� ���� ������� � ������� �����,
    if (ptr != 0)
        ptr->deleteSelf(); // �� ������� ������ (����� ��� ����)
    else
        std::cout << "del: unable to delete - no such file or directory" << std::endl;
}

// ���������� � ������� ���-�� ��������� � ������� ����������
void Console::cat()
{
    std::cout << m_currentFolder->getNumberOfChilds()
    << " elements in current folder\n\n";
}

// ��������� ������� �����
void Console::setCurrentFolder(Folder *folder)
{
    if (folder != 0)
        m_currentFolder = folder;
}

// �������� ����� ������� �����
Folder* Console::getCurrentFolder() const
{
    return m_currentFolder;
}
