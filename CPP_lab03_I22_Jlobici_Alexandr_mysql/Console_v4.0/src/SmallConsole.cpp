// SmallConsole.cpp

#include "SmallConsole.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring> // for strcpy()


// ����������� �� ��������� �������������� ��� ������ ������.
SmallConsole::SmallConsole()
{
    std::string str = "small_console.dat";
    setDbName(str);

    str = "parents_childs";
    setParentsChildsTbName(str);

    str = "file_contents";
    setFileContentsTbName(str);

    setRootID("0");
    setCurrentFolderID(getRootID());
    setRootName("/");
}


// ����������� � ����������� ��������� �� ������������
// ����� ��� ���� ������ � ���� ������.
SmallConsole::SmallConsole(std::string nameOfDataBase,
                           std::string nameOfParentsChilds,
                           std::string nameOfFileContents)
{
    setDbName(nameOfDataBase);
    setParentsChildsTbName(nameOfParentsChilds);
    setFileContentsTbName(nameOfFileContents);

    setRootID(0);
    setCurrentFolderID(getRootID());
    setRootName("/");
}


// �������������� (��������� ������������ ��� ������ �����)
// ���� ������ SQLite. ���������� 0, ���� �������.
int SmallConsole::initDataBase(const std::string &nameOfDb)
{
    int result;

    // ������� ������������ ��� �������� ����� ���� ������.
    result = sqlite3_open(nameOfDb.c_str(), &db);

    // ��������, ���� ���������� ������� ���� ������.
    if (result != SQLITE_OK)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    return 0;
}


// ����������� ������� ������� � ��������� ���� ������.
void SmallConsole::finalizeDataBase()
{
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


bool SmallConsole::isTableExists(const std::string tbName)
{
    int result; // �������� ��������� ����������.

    // ���������, ������� ���������� ������ � ��������� ������.
    query = "SELECT COUNT(TYPE) FROM sqlite_master WHERE type='table'"
            " AND name='" + tbName + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the table '"
                << tbName << "'\n";
        return false;
    }

    // �������� ���������� �������� ����� �������
    result = sqlite3_column_int(stmt, 0);

    // ���� ������ ������ 0 (�.�. �� ����� �������
    // � ��������� ������ �� ����������)
    if (result == 0)
        return false;
    else
        return true;
} // end function isTableExists


bool SmallConsole::isFolderExists(const std::string &parentID,
                                  const std::string &folderName)
{
    int result; // �������� ��������� ����������.

    query = "SELECT COUNT(child_name) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_type = 'folder' AND child_name = '"
            + folderName + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the folder '"
                << folderName << "'\n";
        return false;
    }

    // �������� ���������� �������� ����� �������
    result = sqlite3_column_int(stmt, 0);

    // ���� ������ ������ 0 (�.�. �� ����� �����
    // � ��������� ������ �� ����������)
    if (result == 0)
    {
        return false;
    }

    else
    {
        return true;
    }
}


bool SmallConsole::isFileExists(const std::string &parentID, const std::string &fileName)
{
    int result; // �������� ��������� ����������.

    query = "SELECT COUNT(child_name) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_type = 'file' AND child_name = '" + fileName + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the file '"
                << fileName << "'\n";
        return false;
    }

    // �������� ���������� �������� ����� �������
    result = sqlite3_column_int(stmt, 0);

    // ���� ������ ������ 0 (�.�. �� ������ �����
    // � ��������� ������ �� ����������)
    if (result == 0)
        return false;
    else
        return true;
}


// ��������� �� ������� ������ ������, � ���� ��� �����������, �� ������ ��.
int SmallConsole::checkTables()
{
    int result;

    // ��������, ���� ������� parentsChiledsTbName_ ����������.
    result = isTableExists(getParentsChildsTbName());

    // ���� ������� �� ����������
    if (result == 0)
    {
        // ����� �������� ����� �������
        query = "CREATE TABLE " + getParentsChildsTbName() + " ("
            "child_id INTEGER PRIMARY KEY,"
            "parent_id INTEGER,"
            "child_type TEXT,"
            "child_name TEXT );";

        char bufArr[255]; // ������-�����
        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error: can not create table: " << getParentsChildsTbName()
                    << std::endl;
            return 1;
        }
    }


    // ��������, ���� ������� fileContentsTbName_ ����������.

    result = isTableExists(getFileContentsTbName());

    // ���� ������� �� ����������
    if (result == 0)
    {
        // ����� �������� ����� �������
        query = "CREATE TABLE " + getFileContentsTbName() + " ("
            "file_id INTEGER PRIMARY KEY,"
            "file_buffer TEXT );";

        char bufArr[255]; // ������-�����
        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error: can not create table: " << getFileContentsTbName()
                    << std::endl;
            return 1;
        }
    }

    return 0;
} // end funciton checkTables


// ��������� ���� ���������.
// ����� ������ �� ����� �������� ����� finalizeDataBase().
void SmallConsole::run()
{
    int result;
    // �������������� ���� ������ SQLite.
    result = initDataBase(getDbName());

    // ���� ������������� �� ������ �������,
    if (result != 0)
        return;  // ����� ������� �� ���������.

    // ��������� ������� ������ ������.
    result = checkTables();

    // ����  �������� ������ �� ������ �������,
    if (result != 0)
        return;  // ����� ������� �� ���������.

    // ���������� ��� �������� ����� ������������.
    std::string input;

    // ������� �� ����� �����������.
    std::cout << "Welcome to SmallConsole [Version 4.0]" << std::endl;
    std::cout << "Type \"help\" for instructions.\n" << std::endl;

    // �������� ���� ���������
    while (true)
    {
        // ������� ��������� ������ � ������� ����������
        // ������������ � �������� �����.
        prompt();

        // �������� ���� ������������, �������� ��� � ���������� input.
        std::getline(std::cin, input);

        ///* MENU */
        // � ����������� �� ����, ��� ������������ ���, ����������� ���� �� ��������� ������:

        // ����� �� �������
        if (input == "exit")
        {
            std::cout << "\n Terminated..." << std::endl;
            break;
        }

        // ������� � ������ �����
        else if (input.substr(0, 3) == "cd ")
        {
            if (input.size() > 3)
            {
                // ���� ��� ����� ������ ����� � �������� �������
                if (input.size() == 4 && input.substr(3, 1) == getRootName())
                    setCurrentFolderID(getRootID());

                // �����, ���� ��� ����� ���������� ���� (������������ �� '/')
                else if (input.substr(3, 1) == getRootName())
                {
                    // ��������� � ������, ����� �� ���� ���������������
                    // ����� � ������ �����.
                    setCurrentFolderID(getRootID());

                    // ������� ���������, �� ���������� � ������ "cd "
                    cd(input.substr(3, std::string::npos)); // std::npos -- "�� ����� ������"
                }

                else // �����, ���� ��� ����� ������������� ����
                    cd(input.substr(3, std::string::npos));

                std::cout << std::endl;
            }
        }

        // ���� "cd" ��� ���������.
        else if (input == "cd")
        {
            // ������ �� ������
        }

        // ������� ���������� ������� �����.
        else if (input == "ls")
        {
            ls(getCurrentFolderID());
            std::cout << std::endl;
        }

        // ������� ���������� ��������� �����.
        else if (input.substr(0, 3) == "ls ")
        {
            if (input.size() > 3)
            {
                // �������� � folderName ��� �����.
                std::string folderName = input.substr(3, std::string::npos);

                // �� ����� ����� ID �����, ������������ � ������� ��������.

                // ���� ����� � ����� ������ ����������
                if (isFolderExists(getCurrentFolderID(), folderName))
                {
                    std::string targetID = getIdByName(getCurrentFolderID(), folderName);

                    if (targetID != "-1")  // ���� ID ���� �������
                        ls(targetID);

                    std::cout << std::endl;
                }
                else
                    std::cout << "No such directory\n" << std::endl;
            }
            else // ���� �������� �� ������, �� ������� ���������� ������� �����
            {
                ls(getCurrentFolderID());
                std::cout << std::endl;
            }
        }

        // ���������� � ������� ���-�� ��������� � ������� ����������
        else if (input == "cat")
        {
            cat();
        }

        // ������� ������� ����������
        else if (input == "pwd")
        {
            printFolderFromBeginning(getCurrentFolderID());
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
                del(getCurrentFolderID(), input.substr(4, std::string::npos));
                std::cout << std::endl;
            }
        }

        // �������� (��������) ����� � ����
        else if (input.substr(0, 6) == "write ")
        {
            if (input.size() > 6)
            {
                write(input.substr(6, std::string::npos));
                std::cout << std::endl;
            }
        }

        // ���� "write" ��� ���������.
        else if (input == "write")
        {
            // ������ �� ������
        }

        // ������������ ����� � �����.
        else if (input.substr(0, 8) == "rewrite ")
        {
            if (input.size() > 8)
            {
                rewrite(input.substr(8, std::string::npos));
                std::cout << std::endl;
            }
        }

        // ���� "rewrite" ��� ���������.
        else if (input == "rewrite")
        {
            // ������ �� ������
        }

        // ������� ���������� �����
        else if (input.substr(0, 5) == "read ")
        {
            if (input.size() > 5)
            {
                read(input.substr(5, std::string::npos));
                std::cout << std::endl;
            }
        }

        // ���� "read" ��� ���������.
        else if (input == "read")
        {
            // ������ �� ������
        }

        // ���� ������ ������ enter ��� �������
        else if (input == "")
        {
            // ������ �� ������
        }

        // ������� ����������
        else if (input == "help")
        {
            printHelp();
        }

        // ���� ����� ����������� �������
        else
        {
            std::cout << "Unknown command. Type \"help\" for assistance.\n" << std::endl;
        }
    } // end while

    // ��������� ������ ���� ������ SQLite.
    finalizeDataBase();
} // end function run


// ������� ��������� ������ � ������� ���������� ������������ � �������� �����
void SmallConsole::prompt()
{
    // ������� ���� ���� �� ����� �� �������� ��������.
    printFolderFromBeginning(getCurrentFolderID());
    std::cout << ">";
}


// ������� ���� ���� �� ����� �� �������� ��������
void SmallConsole::printFolderFromBeginning(const std::string &folderID)
{
    int result;

    std::string parentID;

    // �����! � ��������� �������� ��� ��������,
    // ������� ������ �������� ��� ���� ����� getParentID().
    if (folderID != getRootID())
    {
        parentID = getParentID(folderID);
        printFolderFromBeginning(parentID);
    }

    if (folderID == getRootID())
        std::cout << getRootName();

    // ���� ��� ����� �� �������� �������� ���������
    else
    {
        if (parentID != getRootID())
            std::cout << "/"; // ��������� �������� ��������� ����� �������� '/'


        query = "SELECT child_name FROM " + getParentsChildsTbName()
                + " WHERE child_id = " + folderID + ";";

        char bufArr[255]; // ������-�����
        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
            std::cout << sqlite3_column_text(stmt, 0);
        else
            std::cout << "Error: printFolderFromBeginning()";
    }
} // end function printFolderFromBeginning


// ��������� ������� � ������ �����.
bool SmallConsole::cd(const std::string &name)
{
    // ���� ��� ������� "������� �� ���� ������� �����", ��
    if (name == "..")
    {
        if (getCurrentFolderID() != getRootID())
        {
            std::string parentID;
            parentID = getParentID(getCurrentFolderID());
            setCurrentFolderID(parentID);
        }

        return true;
    }

    // �����, ���� ������ ������ -- ��� ����������� '/', ��
    else if (name.substr(0, 1) == "/")
    {
        // ����, ���� ������ ��� �������� ����������� '/'
        size_t position = name.find('/', 1);

        // ���� ����������� '/' ��� ������
        if (position != std::string::npos)
        {
            int n;
            std::string currentFolderName = name.substr(1, position - 1);

            // ���������, ���� ���-����� ����������
            if (isFolderExists(getCurrentFolderID(), currentFolderName))
            {
                std::string targetID = getIdByName(getCurrentFolderID(), currentFolderName);

                if (targetID != "-1")
                    setCurrentFolderID(targetID); // ������ ������� �����

                // ���������� �������� cd(), ����� ���������� ���������� �����
                cd(name.substr(position, std::string::npos));

                return true;
            }
            else
            {
                std::cout << currentFolderName << ": No such directory\n";
                return false;
            }
        }

        else // ���� ������ �� �������� ������ ������������ '/', �� �����
             // ���������� ��������� ��� ���������� ������ �-��� cd()
            cd(name.substr(1, std::string::npos));

        return true;
    }

    else // �����, ��������� � ���� �� ����� �������� ��������.
    {
        // ������ ������ ���������� (��� ����� ���������� ����� �����������
        // ��������� ����������� �� ����� ����������� '/',
        // ������� ����� ��������� �������� ��������� �����).
        if (name == "")
            return true;

        // ����, ���� ������ ��� �������� ����������� '/'
        size_t position = name.find('/', 1);

        // ���� ����������� '/' ��� ������
        if (position != std::string::npos)
        {
            std::string name2 = name;
            name2.insert(0, 1, '/');

            cd(name2);
            return true;
        }

        // ���� ����� � ����� ������ ����������
        else if (isFolderExists(getCurrentFolderID(), name))
        {
            std::string targetID = getIdByName(getCurrentFolderID(), name);

            if (targetID != "-1")
                setCurrentFolderID(targetID); // ������ ������� �����.

            return true;
        }

        else // ����� ������� ��������� �� ������
        {
            std::cout << name << ": No such directory\n";
            return false;
        }
    }
} // end function cd


// ������� ���������� �����.
void SmallConsole::ls(const std::string &folderID)
{
    int result;
    int something;

    query = "SELECT child_name FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + folderID + ";";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    something = result;

    while (result == SQLITE_ROW)
    {
        std::cout << sqlite3_column_text(stmt, 0) << "  ";
        result = sqlite3_step(stmt);
    }

    // ���� ���� ���-�� ��������
    if (something == SQLITE_ROW)
    {
        // �� ������� ��� ������ ������ ��� ��������������.
        std::cout << std::endl;
    }
} // end funciton ls()


// ������ �����
void SmallConsole::mkdir(const std::string &folderName)
{
    // ����, ���� ������ �������� ����������� '/',
    // ������� ������ ������������ � �����.
    size_t position = folderName.find('/');

    // ���� ����������� '/' ��� ������
    if (position != std::string::npos)
    {
        std::cout << "mkdir: name cannot contain this character \"/\"." << std::endl;
        return;
    }

    int result;

    // ������� ��������, ���� ������ ��� �������� ���������� � ������� �����.
    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID()
            + " AND child_name = '" + folderName + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        // ���� �������� ��� ���������
        if (result == 0)
        {
            std::string newChildID = getNewID();
            std::string parentID = getCurrentFolderID();
            std::string childType = "folder";

            query = "INSERT INTO " + getParentsChildsTbName() + " VALUES ("
                    + newChildID + ", " + parentID + ", '"
                    + childType + "', '" + folderName + "');";

            // �������� � �����.
            strcpy(bufArr, query.c_str());
            // ����� ������������ ����������� ������ bufArr[] (������ �������������)
            // � sqlite3_prepare_v2(), ������� ������ �� ����������.
            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);

            if (result != SQLITE_DONE)
            {
                std::cout << "Error: mkdir(): " << sqlite3_errmsg(db) << std::endl;
            }
        }
        else
            std::cout << "mkdir: a folder with this name already exists." << std::endl;
    } // end outer if

} // end function mkdir


// ������ ����.
void SmallConsole::create(const std::string &fileName)
{
    // ����, ���� ������ �������� ����������� '/',
    // ������� ������ ������������ � �����.
    size_t position = fileName.find('/');

    // ���� ����������� '/' ��� ������
    if (position != std::string::npos)
    {
        std::cout << "create: name cannot contain this character \"/\"."
                << std::endl;
        return;
    }

    int result;

    // ������� ��������, ���� ������ ��� �������� ���������� � ������� �����.
    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID()
            + " AND child_name = '" + fileName + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        // ���� �������� ��� ���������
        if (result == 0)
        {
            std::string newChildID = getNewID();
            std::string parentID = getCurrentFolderID();
            std::string childType = "file";

            // ������ ������ � �������� �������.
            query = "INSERT INTO " + getParentsChildsTbName() + " VALUES ("
                    + newChildID + ", " + parentID + ", '"
                    + childType + "', '" + fileName + "');";

            // �������� � �����.
            strcpy(bufArr, query.c_str());

            // ����� ������������ ����������� ������ bufArr[] (������ �������������)
            // � sqlite3_prepare_v2(), ������� ������ �� ����������.
            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);

            if (result != SQLITE_DONE)
            {
                std::cout << "Error: create(): " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // ������ ������ �� ������ �������, ��� �������� ����� �����.
            query = "INSERT INTO " + getFileContentsTbName() + " VALUES ("
                    + newChildID + ", '');";

            // �������� � �����.
            strcpy(bufArr, query.c_str());

            // ����� ������������ ����������� ������ bufArr[] (������ �������������)
            // � sqlite3_prepare_v2(), ������� ������ �� ����������.
            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);

            if (result != SQLITE_DONE)
            {
                std::cout << "Error: create(): " << sqlite3_errmsg(db) << std::endl;
                return;
            }
        }
        else
            std::cout << "create: a file with this name already exists." << std::endl;
    } // end outer if
} // end function create


// ������� ����� ��� ����.
void SmallConsole::del(const std::string &parentID, const std::string &name)
{
    int result;
    std::string childID;
    std::string childType;
    std::stringstream sstm;

    // ������� �� ����� ������� ID, ����� ���������� ��� -- ���� ��� �����.
    query = "SELECT child_id, child_type FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID + " AND child_name = '"
            + name + "';";

    char bufArr[255]; // ������-�����

    // �������� � �����.
    strcpy(bufArr, query.c_str());

    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        // ����������� ������ � ��� std::string.
        sstm << sqlite3_column_int(stmt, 0);
        childID = sstm.str();

        sstm.str(std::string()); // ������� sstm.
        sstm.clear();

        sstm << sqlite3_column_text(stmt, 1);
        childType = sstm.str();

        if (childType == "folder")
        {
            deleteFolder(childID);
        }
        else if (childType == "file")
        {
            deleteFile(childID);
        }
        else
        {
            std::cout << "Error: del(): the type of object is not defined."
                    << std::endl;
        }
    }
    else
    {
         std::cout << "del: unable to delete \"" << name << "\" - no such file or directory." << std::endl;
    }
}  // end function del


void SmallConsole::deleteFolder(std::string &folderID)
{
    int result;

    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName() + " WHERE parent_id = "
            + folderID + ";";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
        // ��������� ���-�� �����
        result = sqlite3_column_int(stmt, 0);

    // ���� ����� �������� �����
    if (result > 0)
    {
        // ���������� ��� �������� ����� ������ � ��������������
        // ����������� ������ � std::string.
        std::stringstream sstm;

        query = "SELECT child_name FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + folderID + ";";

        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        while (result == SQLITE_ROW)
        {
            // ����������� ����� ������ ��������� ������� sstm.
            sstm.str(std::string());
            sstm.clear();

            // ��������� � sstm ��� �����.
            sstm << sqlite3_column_text(stmt, 0);

            del(folderID, sstm.str()); // ����������� ����� del()

            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);
        }
    } // end if

    query = "DELETE FROM " + getParentsChildsTbName() + " WHERE child_id = "
            + folderID + ";";

    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFolder()" << std::endl;
}


void SmallConsole::deleteFile(std::string &fileID)
{
    int result;

    // ID ��� ������ � ���� �� ����� ��������� � �������� parents_childs � file_contents.
    // ������� ������ �� ������ �������.
    query = "DELETE FROM " + getParentsChildsTbName() + " WHERE child_id = "
            + fileID + ";";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFile(): from " << getParentsChildsTbName()
                << std::endl;


    // ������� ������ �� ������ �������.
    query = "DELETE FROM " + getFileContentsTbName() + " WHERE file_id = "
            + fileID + ";";

    // �������� � �����.
    strcpy(bufArr, query.c_str());
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFile(): from " << getFileContentsTbName()
                << std::endl;
} // end funtion deleteFile


// ���������� (���������) ����� � ����.
void SmallConsole::write(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // ���� ������ ���� ����������
    if (fileID != "-1")
    {
        std::string finalBuffer("");

        // �������� ������� ������ ��������.
        query = "SELECT file_buffer FROM " + getFileContentsTbName()
                + " WHERE file_id = " + fileID + ";";

        char bufArr[255]; // ������-�����

        // �������� � �����.
        strcpy(bufArr, query.c_str());

        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {
            std::stringstream sstm;
            sstm << sqlite3_column_text(stmt, 0);

            // ��������� ���������� ����� � �����.
            finalBuffer = sstm.str();
        }
        else
        {
            std::cout << "Error: write" << std::endl;
            return;
        }


        std::cout << "(Enter ctrl+D to end input)" << std::endl;

        std::string wordBuffer("");

        // ���������� �������� ����� � wordBuffer
        getline(std::cin, wordBuffer);

        // ���� ��������� ������ � wordBuffer �� ����� <ctrl+D>.
        while (wordBuffer[wordBuffer.size()-1] != 4)
        {
            wordBuffer.push_back('\n');
            // ��������� � ������ �����, ��� ��� getline �������������� ������ ���.
            finalBuffer += wordBuffer;
            getline(std::cin, wordBuffer);
        }

        query = "UPDATE " + getFileContentsTbName() + " SET file_buffer = '"
                + finalBuffer + "' WHERE file_id = " + fileID + ";";

        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error: write" << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "write: unable to write to \"" << fileName << "\" - no such file."
                << std::endl;
    }
}  // end function write


// �������������� ����� � �����.
void SmallConsole::rewrite(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // ���� ������ ���� ����������
    if (fileID != "-1")
    {
        std::cout << "(Enter ctrl+D to end input)" << std::endl;

        std::string wordBuffer("");
        std::string finalBuffer("");

        // ���������� �������� ����� � wordBuffer
        getline(std::cin, wordBuffer);

        // ���� ��������� ������ � wordBuffer �� ����� <ctrl+D>.
        while (wordBuffer[wordBuffer.size()-1] != 4)
        {
            wordBuffer.push_back('\n');
            // ��������� � ������ �����, ��� ��� getline �������������� ������ ���.
            finalBuffer += wordBuffer;
            getline(std::cin, wordBuffer);
        }

        query = "UPDATE " + getFileContentsTbName() + " SET file_buffer = '"
                + finalBuffer + "' WHERE file_id = " + fileID + ";";

        char bufArr[255]; // ������-�����

        // �������� � �����.
        strcpy(bufArr, query.c_str());

        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error: write" << std::endl;
        }
    }
    else
    {
        std::cout << "write: unable to write to \"" << fileName << "\" - no such file."
                << std::endl;
    }
}  // end function rewrite


// ������� ���������� �����.
void SmallConsole::read(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // ���� ������ ���� ����������
    if (fileID != "-1")
    {
        query = "SELECT file_buffer FROM " + getFileContentsTbName()
                + " WHERE file_id = " + fileID + ";";

        char bufArr[255]; // ������-�����

        // �������� � �����.
        strcpy(bufArr, query.c_str());
        // ����� ������������ ����������� ������ bufArr[] (������ �������������)
        // � sqlite3_prepare_v2(), ������� ������ �� ����������.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {
            std::cout << sqlite3_column_text(stmt, 0);
        }
        else
            std::cout << "Error: read" << std::endl;
    }
    else
    {
        std::cout << "read: unable to read \"" << fileName << "\" - no such file."
                << std::endl;
    }
}  // end function read


// ������������ � ������� ���-�� ��������� � ������� �����.
void SmallConsole::cat()
{
    int result;

    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID() + ";";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        std::cout << sqlite3_column_int(stmt, 0)
                << " element(s) in current folder.\n" << std::endl;
    }
    else
        std::cout << "Error: cat()" << std::endl;
} // end function cat


/// setters

void SmallConsole::setDbName(const std::string &dbName)
{
    dbName_ = dbName;
}


void SmallConsole::setParentsChildsTbName(const std::string &tbName)
{
    parentsChildsTbName_ = tbName;
}


void SmallConsole::setFileContentsTbName(const std::string &tbName)
{
    fileContentsTbName_ = tbName;
}


void SmallConsole::setRootID(const std::string &rootID)
{
    rootID_ = rootID;
}


void SmallConsole::setCurrentFolderID(const std::string &folderID)
{
    currentFolderID_ = folderID;
}


void SmallConsole::setRootName(const std::string &rootName)
{
    rootName_ = rootName;
}


/// getters

std::string SmallConsole::getDbName() const
{
    return dbName_;
}


std::string SmallConsole::getParentsChildsTbName() const
{
    return parentsChildsTbName_;
}


std::string SmallConsole::getFileContentsTbName() const
{
    return fileContentsTbName_;
}


std::string SmallConsole::getRootName() const
{
    return rootName_;
}


std::string SmallConsole::getRootID() const
{
    return rootID_;
}


std::string SmallConsole::getCurrentFolderID() const
{
    return currentFolderID_;
}


// �����! � ��������� �������� ��� ��������,
// ������� ������ �������� ��� ���� ����� getParentID().
std::string SmallConsole::getParentID(const std::string &child_id)
{
    int n;

    query = "SELECT parent_id FROM " + getParentsChildsTbName()
            + " WHERE child_id = " + child_id + ";";

    char bufArr[255]; // ������-�����

    // �������� � �����.
    strcpy(bufArr, query.c_str());

    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    n = sqlite3_step(stmt);

    if (n == SQLITE_ROW)
    {
        n = sqlite3_column_int(stmt, 0);

        std::stringstream sstm;
        sstm << n;  // ��������������� ��������� � std::string

        return sstm.str();
    }
    else
    {
        std::cout << "Error: getParentID()" << std::endl;
        return "-1";  // ����� ������������ ��������.
    }
} // end function getParentID


std::string SmallConsole::getIdByName(const std::string &parentID, const std::string &name)
{
    int result;

    query = "SELECT child_id FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_name = '"+ name + "';";

    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        std::stringstream sstm;
        sstm << result;  // ��������������� ��������� � std::string

        return sstm.str();
    }
    else
    {
        return "-1";
    }
}


// ���������� ���������� ����� ID.
std::string SmallConsole::getNewID()
{
    int n = 0;
    std::stringstream sstm; // ���������� ��� �������������� ���������� � std::string

    // ���������� std::string query, ����� ��������� ������.
    query = "SELECT MAX(child_id) FROM " + getParentsChildsTbName() + ";";
    char bufArr[255]; // ������-�����
    // �������� � �����.
    strcpy(bufArr, query.c_str());
    // ����� ������������ ����������� ������ bufArr[] (������ �������������)
    // � sqlite3_prepare_v2(), ������� ������ �� ����������.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);

    n = sqlite3_step(stmt);

    // ���� � ������� ���� ���� �� ���� child_id
    if (n == SQLITE_ROW)
    {
        n = sqlite3_column_int(stmt, 0);

        sstm << ++n;  // �������������� ����� � ��������������� ��� � ������.

        return sstm.str();
    }

    // �����, � ������� ��� ��� �� ������ child_id.
    return "1";  // ����� ���� ������ child_id.
} // end function getNewID


// ������� ������ ��������� ������.
void SmallConsole::printHelp()
{
    std::cout << "\nAvailable commands:" << std::endl;
    std::cout << "--------------------\n" << std::endl;
    std::cout << " help - displays the available commands." << std::endl;
    std::cout << " exit - exit the SmallConsole." << std::endl;
    std::cout << " cd - change directory." << std::endl;
    std::cout << " cd .. - go to previous directory." << std::endl;
    std::cout << " ls - list content of current or other folder." << std::endl;
    std::cout << " cat - count and print number of elements in current directory." << std::endl;
    std::cout << " pwd - print working directory." << std::endl;
    std::cout << " mkdir - create directory." << std::endl;
    std::cout << " create - create file." << std::endl;
    std::cout << " del - delete file or directory (with or without content)." << std::endl;
    std::cout << " write - append text to a file." << std::endl;
    std::cout << " rewrite - rewrite text in file." << std::endl;
    std::cout << " read - print content of the file.\n\n" << std::endl;
}

