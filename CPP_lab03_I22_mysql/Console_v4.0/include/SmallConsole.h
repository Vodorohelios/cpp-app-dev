// SmallConsole.h
// Author: Alexandr Jlobici
// Gr. I22
// April 2016

#ifndef SMALL_CONSOLE_H
#define SMALL_CONSOLE_H

#include "sqlite3.h"
#include <string>


// Эмулирует поведение консоли. Запуск объекта производиться
// через метод run()
class SmallConsole
{
public:
    // Methods

    // Конструктор по умолчанию инициализирует все данные класса.
    SmallConsole();

    // Конструктор с параметрами принимает от пользователя
    // имена для базы данных и двух таблиц.
    SmallConsole(std::string nameOfDataBase,
                 std::string nameOfParentsChilds,
                 std::string nameOfFileContents);

    // Инициализирует базу данных SQLite. Запускается методом run().
    int initDataBase(const std::string &dbName);

    // Корректно завершает работу базы данных.
    // Запускается методом run() в конце цикла.
    void finalizeDataBase();

    // Возвращает true, если указанная таблица существует.
    bool isTableExists(const std::string tbName);
    bool isFolderExists(const std::string &parentID, const std::string &folderName);
    bool isFileExists(const std::string &parentID, const std::string &fileName);

    // Проверяет на наличие нужных таблиц, и если они отсутствуют, то создаёт их.
    // Возвращает 0, если ошибок не возникло.
    int checkTables();

    // Запускает цикл программы.
    void run();


    // Funcionts for menu commands

    // Выводит командную строку с текущим положением.
    // пользователя в иерархии папок.
    void prompt();

    // Выводит весь путь от корня до текущего каталога.
    void printFolderFromBeginning(const std::string &folderID);

    bool cd(const std::string &name);  // Меняет текущую папку.
    void ls(const std::string &folderID);  // Выводит содержимое папки.
    void mkdir(const std::string &folderName);  // Создаёт папку.
    void create(const std::string &fileName);  // Создаёт файл.

    // Удаляет папку или файл.
    void del(const std::string &parentID, const std::string &name);

    // Вспомогательные функции для del()
    void deleteFolder(std::string &folderID);
    void deleteFile(std::string &fileID);

    void write(const std::string &fileName);  // Записывает (добавляет) текст в файл.
    void rewrite(const std::string &fileName);  // Перезаписывает текст в файле.
    void read(const std::string &fileName);  // Выводит содержимое файла.

    void cat();  // Подсчитывает и выводит кол-во элементов в текущей папке.
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

    // Возвращает уникальный номер ID.
    std::string getNewID();

private:
    // Data without setters and getters

    sqlite3 *db;  // Указатель на объект базы данных.
    sqlite3_stmt *stmt;  // Объект sql запроса.
    std::string query;  // Строка sql запроса.


    // Data with setters and getters

    std::string dbName_;  // Имя базы данных.

    // Имя таблицы, хранящей отношения между родителями и детьми.
    std::string parentsChildsTbName_;

    // Имя таблицы, храняещей текстовое содержание файлов.
    std::string fileContentsTbName_;

    std::string rootID_;  // ID корневой папки.
    std::string currentFolderID_;  // ID текущей папки.
    std::string rootName_;  // Имя, представляющее корневой каталог.
}; // end class Console

#endif // SMALL_CONSOLE_H
