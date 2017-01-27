// SmallConsole.h
// Author: Alexandr Jlobici
// Gr. I22
// April 2016

#ifndef SMALL_CONSOLE_H
#define SMALL_CONSOLE_H

#include "sqlite3.h"
#include <string>


// ��������� ��������� �������. ������ ������� �������������
// ����� ����� run()
class SmallConsole
{
public:
    // Methods

    // ����������� �� ��������� �������������� ��� ������ ������.
    SmallConsole();

    // ����������� � ����������� ��������� �� ������������
    // ����� ��� ���� ������ � ���� ������.
    SmallConsole(std::string nameOfDataBase,
                 std::string nameOfParentsChilds,
                 std::string nameOfFileContents);

    // �������������� ���� ������ SQLite. ����������� ������� run().
    int initDataBase(const std::string &dbName);

    // ��������� ��������� ������ ���� ������.
    // ����������� ������� run() � ����� �����.
    void finalizeDataBase();

    // ���������� true, ���� ��������� ������� ����������.
    bool isTableExists(const std::string tbName);
    bool isFolderExists(const std::string &parentID, const std::string &folderName);
    bool isFileExists(const std::string &parentID, const std::string &fileName);

    // ��������� �� ������� ������ ������, � ���� ��� �����������, �� ������ ��.
    // ���������� 0, ���� ������ �� ��������.
    int checkTables();

    // ��������� ���� ���������.
    void run();


    // Funcionts for menu commands

    // ������� ��������� ������ � ������� ����������.
    // ������������ � �������� �����.
    void prompt();

    // ������� ���� ���� �� ����� �� �������� ��������.
    void printFolderFromBeginning(const std::string &folderID);

    bool cd(const std::string &name);  // ������ ������� �����.
    void ls(const std::string &folderID);  // ������� ���������� �����.
    void mkdir(const std::string &folderName);  // ������ �����.
    void create(const std::string &fileName);  // ������ ����.

    // ������� ����� ��� ����.
    void del(const std::string &parentID, const std::string &name);

    // ��������������� ������� ��� del()
    void deleteFolder(std::string &folderID);
    void deleteFile(std::string &fileID);

    void write(const std::string &fileName);  // ���������� (���������) ����� � ����.
    void rewrite(const std::string &fileName);  // �������������� ����� � �����.
    void read(const std::string &fileName);  // ������� ���������� �����.

    void cat();  // ������������ � ������� ���-�� ��������� � ������� �����.
    void printHelp();


    // Setters

    void setDbName(const std::string &dbName);
    void setParentsChildsTbName(const std::string &tbName);
    void setFileContentsTbName(const std::string &tbName);
    void setRootID(const std::string &rootID);
    void setCurrentFolderID(const std::string &folderID);
    void setRootName(const std::string &rootName);


    // Getters

    std::string getDbName() const;
    std::string getParentsChildsTbName() const;
    std::string getFileContentsTbName() const;
    std::string getRootName() const;
    std::string getRootID() const;
    std::string getCurrentFolderID() const;
    std::string getParentID(const std::string &child_id);
    std::string getIdByName(const std::string &parentID, const std::string &name);

    // ���������� ���������� ����� ID.
    std::string getNewID();

private:
    // Data without setters and getters

    sqlite3 *db;  // ��������� �� ������ ���� ������.
    sqlite3_stmt *stmt;  // ������ sql �������.
    std::string query;  // ������ sql �������.


    // Data with setters and getters

    std::string dbName_;  // ��� ���� ������.

    // ��� �������, �������� ��������� ����� ���������� � ������.
    std::string parentsChildsTbName_;

    // ��� �������, ��������� ��������� ���������� ������.
    std::string fileContentsTbName_;

    std::string rootID_;  // ID �������� �����.
    std::string currentFolderID_;  // ID ������� �����.
    std::string rootName_;  // ���, �������������� �������� �������.
}; // end class Console

#endif // SMALL_CONSOLE_H
