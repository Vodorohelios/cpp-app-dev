// SmallConsole.cpp

#include "SmallConsole.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring> // for strcpy()


// Конструктор по умолчанию инициализирует все данные класса.
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


// Конструктор с параметрами принимает от пользователя
// имена для базы данных и двух таблиц.
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


// Инициализирует (открывает существующую или создаёт новую)
// базу данных SQLite. Возвращает 0, если успешно.
int SmallConsole::initDataBase(const std::string &nameOfDb)
{
    int result;

    // Откроем сущуствующую или создадим новую базу данных.
    result = sqlite3_open(nameOfDb.c_str(), &db);

    // Проверим, если получилось открыть базу данных.
    if (result != SQLITE_OK)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    return 0;
}


// Освобождает занятые ресурсы и закрывает базу данных.
void SmallConsole::finalizeDataBase()
{
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


bool SmallConsole::isTableExists(const std::string tbName)
{
    int result; // Создадим локальную переменную.

    // Посчитаем, сколько существует таблиц с указанным именем.
    query = "SELECT COUNT(TYPE) FROM sqlite_master WHERE type='table'"
            " AND name='" + tbName + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the table '"
                << tbName << "'\n";
        return false;
    }

    // Извлечём полученное значение после запроса
    result = sqlite3_column_int(stmt, 0);

    // Если запрос вернул 0 (т.е. ни одной таблицы
    // с указанным именем не существует)
    if (result == 0)
        return false;
    else
        return true;
} // end function isTableExists


bool SmallConsole::isFolderExists(const std::string &parentID,
                                  const std::string &folderName)
{
    int result; // Создадим локальную переменную.

    query = "SELECT COUNT(child_name) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_type = 'folder' AND child_name = '"
            + folderName + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the folder '"
                << folderName << "'\n";
        return false;
    }

    // Извлечём полученное значение после запроса
    result = sqlite3_column_int(stmt, 0);

    // Если запрос вернул 0 (т.е. ни одной папки
    // с указанным именем не существует)
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
    int result; // Создадим локальную переменную.

    query = "SELECT COUNT(child_name) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_type = 'file' AND child_name = '" + fileName + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        std::cout << "Error: there is no result after checking the exixtence of the file '"
                << fileName << "'\n";
        return false;
    }

    // Извлечём полученное значение после запроса
    result = sqlite3_column_int(stmt, 0);

    // Если запрос вернул 0 (т.е. ни одного файла
    // с указанным именем не существует)
    if (result == 0)
        return false;
    else
        return true;
}


// Проверяет на наличие нужных таблиц, и если они отсутствуют, то создаёт их.
int SmallConsole::checkTables()
{
    int result;

    // Проверим, если таблица parentsChiledsTbName_ существует.
    result = isTableExists(getParentsChildsTbName());

    // Если таблицы не существует
    if (result == 0)
    {
        // тогда создадим новую таблицу
        query = "CREATE TABLE " + getParentsChildsTbName() + " ("
            "child_id INTEGER PRIMARY KEY,"
            "parent_id INTEGER,"
            "child_type TEXT,"
            "child_name TEXT );";

        char bufArr[255]; // массив-буфер
        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE)
        {
            std::cout << "Error: can not create table: " << getParentsChildsTbName()
                    << std::endl;
            return 1;
        }
    }


    // Проверим, если таблица fileContentsTbName_ существует.

    result = isTableExists(getFileContentsTbName());

    // Если таблицы не существует
    if (result == 0)
    {
        // тогда создадим новую таблицу
        query = "CREATE TABLE " + getFileContentsTbName() + " ("
            "file_id INTEGER PRIMARY KEY,"
            "file_buffer TEXT );";

        char bufArr[255]; // массив-буфер
        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Запускает цикл программы.
// После выхода из цикла вызывает метод finalizeDataBase().
void SmallConsole::run()
{
    int result;
    // Инициализируем базу данных SQLite.
    result = initDataBase(getDbName());

    // Если инициализация не прошла успешно,
    if (result != 0)
        return;  // тогда выходим из программы.

    // Проверяем наличие нужных таблиц.
    result = checkTables();

    // Если  проверка таблиц не прошла успешно,
    if (result != 0)
        return;  // тогда выходим из программы.

    // Переменная для хранения ввода пользователя.
    std::string input;

    // Выводим на экран приветствие.
    std::cout << "Welcome to SmallConsole [Version 4.0]" << std::endl;
    std::cout << "Type \"help\" for instructions.\n" << std::endl;

    // Основной цикл программы
    while (true)
    {
        // Выводим командную строку с текущим положением
        // пользователя в иерархии папок.
        prompt();

        // Получаем ввод пользователя, сохраняя его в переменной input.
        std::getline(std::cin, input);

        ///* MENU */
        // В зависимости от того, что пользователь ввёл, выполняется одна из следующих команд:

        // Выход из консоли
        if (input == "exit")
        {
            std::cout << "\n Terminated..." << std::endl;
            break;
        }

        // Переход в другую папку
        else if (input.substr(0, 3) == "cd ")
        {
            if (input.size() > 3)
            {
                // Если был введён запрос войти в корневой каталог
                if (input.size() == 4 && input.substr(3, 1) == getRootName())
                    setCurrentFolderID(getRootID());

                // Иначе, если был ввёден абсолютный путь (начинающийся на '/')
                else if (input.substr(3, 1) == getRootName())
                {
                    // Переходим в корень, чтобы из него последовательно
                    // войти в нужные папки.
                    setCurrentFolderID(getRootID());

                    // Передаём подстроку, не содержащую в начале "cd "
                    cd(input.substr(3, std::string::npos)); // std::npos -- "до конца строки"
                }

                else // Иначе, если был введён относительный путь
                    cd(input.substr(3, std::string::npos));

                std::cout << std::endl;
            }
        }

        // Ввод "cd" без аргумента.
        else if (input == "cd")
        {
            // ничего не делаем
        }

        // Вывести содержимое текущей папки.
        else if (input == "ls")
        {
            ls(getCurrentFolderID());
            std::cout << std::endl;
        }

        // Вывести содержимое указанной папки.
        else if (input.substr(0, 3) == "ls ")
        {
            if (input.size() > 3)
            {
                // Извлечём в folderName имя папки.
                std::string folderName = input.substr(3, std::string::npos);

                // По имени найдём ID папки, содержащейся в текущем каталоге.

                // Если папка с таким именем существует
                if (isFolderExists(getCurrentFolderID(), folderName))
                {
                    std::string targetID = getIdByName(getCurrentFolderID(), folderName);

                    if (targetID != "-1")  // Если ID было найдено
                        ls(targetID);

                    std::cout << std::endl;
                }
                else
                    std::cout << "No such directory\n" << std::endl;
            }
            else // если аргумент не указан, то выводим содержимое текущей папки
            {
                ls(getCurrentFolderID());
                std::cout << std::endl;
            }
        }

        // подсчитать и вывести кол-во элементов в текущей директории
        else if (input == "cat")
        {
            cat();
        }

        // Вывести текущую директорию
        else if (input == "pwd")
        {
            printFolderFromBeginning(getCurrentFolderID());
            std::cout << std::endl << std::endl;
        }

        // Создать папку
        else if (input.substr(0, 6) == "mkdir ")
        {
            if (input.size() > 6)
            {
                mkdir(input.substr(6, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Создать файл
        else if (input.substr(0, 7) == "create ")
        {
            if (input.size() > 7)
            {
                create(input.substr(7, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Удалить папку или файл
        else if (input.substr(0, 4) == "del ")
        {
            if (input.size() > 4)
            {
                del(getCurrentFolderID(), input.substr(4, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Записать (добавить) текст в файл
        else if (input.substr(0, 6) == "write ")
        {
            if (input.size() > 6)
            {
                write(input.substr(6, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Ввод "write" без аргумента.
        else if (input == "write")
        {
            // ничего не делаем
        }

        // Перезаписать текст в файле.
        else if (input.substr(0, 8) == "rewrite ")
        {
            if (input.size() > 8)
            {
                rewrite(input.substr(8, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Ввод "rewrite" без аргумента.
        else if (input == "rewrite")
        {
            // ничего не делаем
        }

        // Вывести содержимое файла
        else if (input.substr(0, 5) == "read ")
        {
            if (input.size() > 5)
            {
                read(input.substr(5, std::string::npos));
                std::cout << std::endl;
            }
        }

        // Ввод "read" без аргумента.
        else if (input == "read")
        {
            // ничего не делаем
        }

        // если просто нажали enter без команды
        else if (input == "")
        {
            // ничего не делаем
        }

        // вывести инструкцию
        else if (input == "help")
        {
            printHelp();
        }

        // если ввели неизвестную команду
        else
        {
            std::cout << "Unknown command. Type \"help\" for assistance.\n" << std::endl;
        }
    } // end while

    // Завершаем работу базу данных SQLite.
    finalizeDataBase();
} // end function run


// вывести командную строку с текущим положением пользователя в иерархии папок
void SmallConsole::prompt()
{
    // Вывести весь путь от корня до текущего каталога.
    printFolderFromBeginning(getCurrentFolderID());
    std::cout << ">";
}


// вывести весь путь от корня до текущего каталога
void SmallConsole::printFolderFromBeginning(const std::string &folderID)
{
    int result;

    std::string parentID;

    // Важно! У корневого каталога нет родителя,
    // поэтому нельзя вызывать для него метод getParentID().
    if (folderID != getRootID())
    {
        parentID = getParentID(folderID);
        printFolderFromBeginning(parentID);
    }

    if (folderID == getRootID())
        std::cout << getRootName();

    // Если эта папка не является корневым каталогом
    else
    {
        if (parentID != getRootID())
            std::cout << "/"; // визуально разделим отдельные папки символом '/'


        query = "SELECT child_name FROM " + getParentsChildsTbName()
                + " WHERE child_id = " + folderID + ";";

        char bufArr[255]; // массив-буфер
        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
            std::cout << sqlite3_column_text(stmt, 0);
        else
            std::cout << "Error: printFolderFromBeginning()";
    }
} // end function printFolderFromBeginning


// Совершает переход в другую папку.
bool SmallConsole::cd(const std::string &name)
{
    // Если это команда "перейти на один уровень назад", то
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

    // Иначе, если первый символ -- это разделитель '/', то
    else if (name.substr(0, 1) == "/")
    {
        // Ищем, если строка ещё содержит разделитель '/'
        size_t position = name.find('/', 1);

        // Если разделитель '/' был найден
        if (position != std::string::npos)
        {
            int n;
            std::string currentFolderName = name.substr(1, position - 1);

            // Проверяем, если под-папка существует
            if (isFolderExists(getCurrentFolderID(), currentFolderName))
            {
                std::string targetID = getIdByName(getCurrentFolderID(), currentFolderName);

                if (targetID != "-1")
                    setCurrentFolderID(targetID); // меняем текущую папку

                // Рекурсивно вызываем cd(), чтобы обработать оставшиеся папки
                cd(name.substr(position, std::string::npos));

                return true;
            }
            else
            {
                std::cout << currentFolderName << ": No such directory\n";
                return false;
            }
        }

        else // Если строка не содержит больше разделителей '/', то задаём
             // корректные аргументы для последнего вызова ф-ции cd()
            cd(name.substr(1, std::string::npos));

        return true;
    }

    else // Иначе, переходим в одну из папок текущего каталога.
    {
        // Пустую строку игнорируем (она может получиться после рекурсивной
        // обработки оставшегося на конце разделителя '/',
        // который может завершать название последней папки).
        if (name == "")
            return true;

        // Ищем, если строка ещё содержит разделитель '/'
        size_t position = name.find('/', 1);

        // Если разделитель '/' был найден
        if (position != std::string::npos)
        {
            std::string name2 = name;
            name2.insert(0, 1, '/');

            cd(name2);
            return true;
        }

        // Если папка с таким именем существует
        else if (isFolderExists(getCurrentFolderID(), name))
        {
            std::string targetID = getIdByName(getCurrentFolderID(), name);

            if (targetID != "-1")
                setCurrentFolderID(targetID); // меняем текущую папку.

            return true;
        }

        else // иначе выводим сообщение об ошибке
        {
            std::cout << name << ": No such directory\n";
            return false;
        }
    }
} // end function cd


// Выводит содержимое папки.
void SmallConsole::ls(const std::string &folderID)
{
    int result;
    int something;

    query = "SELECT child_name FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + folderID + ";";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    something = result;

    while (result == SQLITE_ROW)
    {
        std::cout << sqlite3_column_text(stmt, 0) << "  ";
        result = sqlite3_step(stmt);
    }

    // Если было что-то выведено
    if (something == SQLITE_ROW)
    {
        // то добавим ещё разрыв строки для форматирования.
        std::cout << std::endl;
    }
} // end funciton ls()


// Создаёт папку
void SmallConsole::mkdir(const std::string &folderName)
{
    // Ищем, если строка содержит разделитель '/',
    // который нельзя использовать в имени.
    size_t position = folderName.find('/');

    // Если разделитель '/' был найден
    if (position != std::string::npos)
    {
        std::cout << "mkdir: name cannot contain this character \"/\"." << std::endl;
        return;
    }

    int result;

    // Сначала проверим, если данное имя является уникальным в текущей папке.
    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID()
            + " AND child_name = '" + folderName + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        // Если заданное имя уникально
        if (result == 0)
        {
            std::string newChildID = getNewID();
            std::string parentID = getCurrentFolderID();
            std::string childType = "folder";

            query = "INSERT INTO " + getParentsChildsTbName() + " VALUES ("
                    + newChildID + ", " + parentID + ", '"
                    + childType + "', '" + folderName + "');";

            // Копируем в буфер.
            strcpy(bufArr, query.c_str());
            // Когда используется статический массив bufArr[] (вместо динамического)
            // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Создаёт файл.
void SmallConsole::create(const std::string &fileName)
{
    // Ищем, если строка содержит разделитель '/',
    // который нельзя использовать в имени.
    size_t position = fileName.find('/');

    // Если разделитель '/' был найден
    if (position != std::string::npos)
    {
        std::cout << "create: name cannot contain this character \"/\"."
                << std::endl;
        return;
    }

    int result;

    // Сначала проверим, если данное имя является уникальным в текущей папке.
    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID()
            + " AND child_name = '" + fileName + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        // Если заданное имя уникально
        if (result == 0)
        {
            std::string newChildID = getNewID();
            std::string parentID = getCurrentFolderID();
            std::string childType = "file";

            // Создаём запись в основной таблице.
            query = "INSERT INTO " + getParentsChildsTbName() + " VALUES ("
                    + newChildID + ", " + parentID + ", '"
                    + childType + "', '" + fileName + "');";

            // Копируем в буфер.
            strcpy(bufArr, query.c_str());

            // Когда используется статический массив bufArr[] (вместо динамического)
            // в sqlite3_prepare_v2(), никакой ошибки не появляется.
            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);

            if (result != SQLITE_DONE)
            {
                std::cout << "Error: create(): " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // Создаём запись во второй таблице, где хранится буфер файла.
            query = "INSERT INTO " + getFileContentsTbName() + " VALUES ("
                    + newChildID + ", '');";

            // Копируем в буфер.
            strcpy(bufArr, query.c_str());

            // Когда используется статический массив bufArr[] (вместо динамического)
            // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Удаляет папку или файл.
void SmallConsole::del(const std::string &parentID, const std::string &name)
{
    int result;
    std::string childID;
    std::string childType;
    std::stringstream sstm;

    // Сначала по имени находим ID, затем определяем тип -- файл или папка.
    query = "SELECT child_id, child_type FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID + " AND child_name = '"
            + name + "';";

    char bufArr[255]; // массив-буфер

    // Копируем в буфер.
    strcpy(bufArr, query.c_str());

    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        // Преобразуем данные в тип std::string.
        sstm << sqlite3_column_int(stmt, 0);
        childID = sstm.str();

        sstm.str(std::string()); // Очищаем sstm.
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

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
        // Сохраняем кол-во детей
        result = sqlite3_column_int(stmt, 0);

    // Если папка содержит детей
    if (result > 0)
    {
        // Переменная для хранения имени ребёнка и преобразования
        // полученного текста в std::string.
        std::stringstream sstm;

        query = "SELECT child_name FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + folderID + ";";

        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        while (result == SQLITE_ROW)
        {
            // Обязательно перед каждой итерацией очищаем sstm.
            sstm.str(std::string());
            sstm.clear();

            // Извлекаем в sstm имя папки.
            sstm << sqlite3_column_text(stmt, 0);

            del(folderID, sstm.str()); // Рекурсивный вызов del()

            sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
            result = sqlite3_step(stmt);
        }
    } // end if

    query = "DELETE FROM " + getParentsChildsTbName() + " WHERE child_id = "
            + folderID + ";";

    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFolder()" << std::endl;
}


void SmallConsole::deleteFile(std::string &fileID)
{
    int result;

    // ID для одного и того же файла одинаково в таблицах parents_childs и file_contents.
    // Удаляем запись из первой таблицы.
    query = "DELETE FROM " + getParentsChildsTbName() + " WHERE child_id = "
            + fileID + ";";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFile(): from " << getParentsChildsTbName()
                << std::endl;


    // Удаляем запись из второй таблицы.
    query = "DELETE FROM " + getFileContentsTbName() + " WHERE file_id = "
            + fileID + ";";

    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
        std::cout << "Error: deleteFile(): from " << getFileContentsTbName()
                << std::endl;
} // end funtion deleteFile


// Записывает (добавляет) текст в файл.
void SmallConsole::write(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // Если данный файл существует
    if (fileID != "-1")
    {
        std::string finalBuffer("");

        // Извлечём сначала старое значение.
        query = "SELECT file_buffer FROM " + getFileContentsTbName()
                + " WHERE file_id = " + fileID + ";";

        char bufArr[255]; // массив-буфер

        // Копируем в буфер.
        strcpy(bufArr, query.c_str());

        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
        sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW)
        {
            std::stringstream sstm;
            sstm << sqlite3_column_text(stmt, 0);

            // Сохраняем полученный текст в буфер.
            finalBuffer = sstm.str();
        }
        else
        {
            std::cout << "Error: write" << std::endl;
            return;
        }


        std::cout << "(Enter ctrl+D to end input)" << std::endl;

        std::string wordBuffer("");

        // Записываем вводимый текст в wordBuffer
        getline(std::cin, wordBuffer);

        // пока последний символ в wordBuffer не равен <ctrl+D>.
        while (wordBuffer[wordBuffer.size()-1] != 4)
        {
            wordBuffer.push_back('\n');
            // Сохраняем в другой буфер, так как getline перезаписывает каждый раз.
            finalBuffer += wordBuffer;
            getline(std::cin, wordBuffer);
        }

        query = "UPDATE " + getFileContentsTbName() + " SET file_buffer = '"
                + finalBuffer + "' WHERE file_id = " + fileID + ";";

        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Перезаписывает текст в файле.
void SmallConsole::rewrite(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // Если данный файл существует
    if (fileID != "-1")
    {
        std::cout << "(Enter ctrl+D to end input)" << std::endl;

        std::string wordBuffer("");
        std::string finalBuffer("");

        // Записываем вводимый текст в wordBuffer
        getline(std::cin, wordBuffer);

        // пока последний символ в wordBuffer не равен <ctrl+D>.
        while (wordBuffer[wordBuffer.size()-1] != 4)
        {
            wordBuffer.push_back('\n');
            // Сохраняем в другой буфер, так как getline перезаписывает каждый раз.
            finalBuffer += wordBuffer;
            getline(std::cin, wordBuffer);
        }

        query = "UPDATE " + getFileContentsTbName() + " SET file_buffer = '"
                + finalBuffer + "' WHERE file_id = " + fileID + ";";

        char bufArr[255]; // массив-буфер

        // Копируем в буфер.
        strcpy(bufArr, query.c_str());

        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Выводит содержимое файла.
void SmallConsole::read(const std::string &fileName)
{
    int result;
    std::string fileID;

    fileID = getIdByName(getCurrentFolderID(), fileName);

    // Если данный файл существует
    if (fileID != "-1")
    {
        query = "SELECT file_buffer FROM " + getFileContentsTbName()
                + " WHERE file_id = " + fileID + ";";

        char bufArr[255]; // массив-буфер

        // Копируем в буфер.
        strcpy(bufArr, query.c_str());
        // Когда используется статический массив bufArr[] (вместо динамического)
        // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Подсчитывает и выводит кол-во элементов в текущей папке.
void SmallConsole::cat()
{
    int result;

    query = "SELECT COUNT(child_id) FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + getCurrentFolderID() + ";";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
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


// Важно! У корневого каталога нет родителя,
// поэтому нельзя вызывать для него метод getParentID().
std::string SmallConsole::getParentID(const std::string &child_id)
{
    int n;

    query = "SELECT parent_id FROM " + getParentsChildsTbName()
            + " WHERE child_id = " + child_id + ";";

    char bufArr[255]; // массив-буфер

    // Копируем в буфер.
    strcpy(bufArr, query.c_str());

    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    n = sqlite3_step(stmt);

    if (n == SQLITE_ROW)
    {
        n = sqlite3_column_int(stmt, 0);

        std::stringstream sstm;
        sstm << n;  // преобразовываем результат в std::string

        return sstm.str();
    }
    else
    {
        std::cout << "Error: getParentID()" << std::endl;
        return "-1";  // Вернём неподходящее значение.
    }
} // end function getParentID


std::string SmallConsole::getIdByName(const std::string &parentID, const std::string &name)
{
    int result;

    query = "SELECT child_id FROM " + getParentsChildsTbName()
            + " WHERE parent_id = " + parentID
            + " AND child_name = '"+ name + "';";

    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        result = sqlite3_column_int(stmt, 0);

        std::stringstream sstm;
        sstm << result;  // преобразовываем результат в std::string

        return sstm.str();
    }
    else
    {
        return "-1";
    }
}


// Возвращает уникальный номер ID.
std::string SmallConsole::getNewID()
{
    int n = 0;
    std::stringstream sstm; // переменная для преобразования результата в std::string

    // Используем std::string query, чтобы соединить строки.
    query = "SELECT MAX(child_id) FROM " + getParentsChildsTbName() + ";";
    char bufArr[255]; // массив-буфер
    // Копируем в буфер.
    strcpy(bufArr, query.c_str());
    // Когда используется статический массив bufArr[] (вместо динамического)
    // в sqlite3_prepare_v2(), никакой ошибки не появляется.
    sqlite3_prepare_v2(db, bufArr, 255, &stmt, NULL);

    n = sqlite3_step(stmt);

    // Если в таблице есть хотя бы один child_id
    if (n == SQLITE_ROW)
    {
        n = sqlite3_column_int(stmt, 0);

        sstm << ++n;  // Инкрементируем число и преобразовываем его в строку.

        return sstm.str();
    }

    // Иначе, в таблице ещё нет ни одного child_id.
    return "1";  // Вернём самы первый child_id.
} // end function getNewID


// Выводит список доступных команд.
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

