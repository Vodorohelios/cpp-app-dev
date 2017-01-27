// Console.cpp


#include "Console.h"
#include "Folder.h"
#include "File.h"
#include <iostream>
#include <string>
#include <algorithm> // для transform()


// конструктор
Console::Console() : m_root("/", 0)
{
    // по умолчанию текущей папкой является корень
    setCurrentFolder(&m_root);
}

// запуск цикла программы
void Console::run()
{
    std::string input; // переменная для хранения ввода пользователя

    // Вывод на экран приветствия
    std::cout << "Welcome to Console [Version 3.0]" << std::endl;
    std::cout << "Type \"help\" for instructions.\n" << std::endl;

    while (true)
    {
        // вывести командную строку с текущим положением пользователя в иерархии папок
        prompt();

        // получить ввод пользователя, сохранив его в переменной input
        std::getline(std::cin, input);

        // преобразуем все буквы верхнего регистра в нижние
        std::transform(input.begin(), input.end(), input.begin(), tolower);

        /* MENU */
        // в зависимости от того, что пользователь ввёл, выполнить одну из следующих команд:

        // выход из консоли
        if (input == "exit")
        {
            std::cout << "\n Termination..." << std::endl;
            break;
        }

        // переход в другую папку
        else if (input.substr(0, 3) == "cd ")
        {
            if (input.size() > 3)
            {
                // если был введён запрос войти в корневой каталог
                if (input.size() == 4 && input.substr(3, 1) == "/")
                    m_currentFolder = &m_root;

                // иначе, если был ввёден абсолютный путь (начинающийся на '/')
                else if (input.substr(3, 1) == "/")
                {
                    m_currentFolder = &m_root; // переходим в корень, чтобы из него последовательно войти в нужные папки
                    // передаём подстроку, не содержащую в начале "cd "
                    cd(input.substr(3, std::string::npos)); // std::npos это статичная член-константа, которая означает "до конца строки"
                }

                else // иначе, если был введён относительный путь
                    cd(input.substr(3, std::string::npos));

                std::cout << std::endl;
            }
        }

        // ввод "cd" без аргумента
        else if (input.substr(0, 2) == "cd")
        {
            // ничего не делаем
        }

        // вывести содержимое текущей папки
        else if (input == "ls")
        {
            ls(m_currentFolder);
        }

        // вывести содержимое указанной папки
        else if (input.substr(0, 3) == "ls ")
        {
            if (input.size() > 3)
            {
                // извлечём в folderName имя папки
                std::string folderName = input.substr(3, std::string::npos);

                // найдем указатель на папку, содержащуюся в текущем каталоге
                Entity *childPtr = m_currentFolder->findChild(folderName);
                // преобразуем указатель в тип Folder, чтобы вызвать соответствующую функцию
                Folder *folderPtr = dynamic_cast<Folder *>(childPtr);

                if (folderPtr != 0) // если folderPtr содержит указатель на существующую папку
                    ls(folderPtr);
                else
                    std::cout << "No such directory\n" << std::endl;
            }
            else // если аргумент не указан, то выводим содержимое текущей папки
                ls(m_currentFolder);
        }

        // подсчитать и вывести кол-во элементов в текущей директории
        else if (input == "cat")
        {
            cat();
        }

        // вывести текущую директорию
        else if (input == "pwd")
        {
            printFolderFromBeginning(m_currentFolder);
            std::cout << std::endl << std::endl;
        }

        // создать папку
        else if (input.substr(0, 6) == "mkdir ")
        {
            if (input.size() > 6)
            {
                mkdir(input.substr(6, std::string::npos));
                std::cout << std::endl;
            }
        }

        // создать файл
        else if (input.substr(0, 7) == "create ")
        {
            if (input.size() > 7)
            {
                create(input.substr(7, std::string::npos));
                std::cout << std::endl;
            }
        }

        // удалить папку или файл
        else if (input.substr(0, 4) == "del ")
        {
            if (input.size() > 4)
            {
                del(input.substr(4, std::string::npos));
                std::cout << std::endl;
            }
        }

        // записать (добавить) текст в файл
        else if (input.substr(0, 6) == "write ")
        {
            if (input.size() > 6)
            {
                // ищем, если имя файла содержится в текущем каталоге
                Entity *ptr = m_currentFolder->findChild(input.substr(6, std::string::npos));
                // преобразуем тип для использования функции write()
                File *filePtr = dynamic_cast<File *>(ptr);

                // если файл существует
                if (filePtr != 0)
                {
                    filePtr->write(filePtr); // вписываем в файл текст
                    std::cout << std::endl;
                }
            }
        }

        // переписать текст в файле
        else if (input.substr(0, 8) == "rewrite ")
        {
            if (input.size() > 8)
            {
                // ищем, если имя файла содержится в текущем каталоге
                Entity *ptr = m_currentFolder->findChild(input.substr(8, std::string::npos));
                // преобразуем тип для использования функции write()
                File *filePtr = dynamic_cast<File *>(ptr);

                // если файл существует
                if (filePtr != 0)
                {
                    filePtr->rewrite(filePtr); // вписываем в файл текст
                    std::cout << std::endl;
                }
            }
        }

        // вывести содержимое файла
        else if (input.substr(0, 5) == "read ")
        {
            if (input.size() > 5)
            {
                // ищем, если имя файла содержится в текущем каталоге
                Entity *ptr = m_currentFolder->findChild(input.substr(5, std::string::npos));
                // преобразуем тип для использования функции read()
                File *filePtr = dynamic_cast<File *>(ptr);

                // если файл существует
                if (filePtr != 0)
                {
                    filePtr->read(filePtr); // выводим содержимое файла
                }
            }
        }

        // если просто нажали enter без команды
        else if (input == "")
        {
            // ничего не делаем
        }

        // вывести инструкцию
        else if (input == "help")
        {
            // вывести список доступных команд
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

        // если ввели неизвестную команду
        else
        {
            std::cout << "Unknown command. Type \"help\" for assistance\n" << std::endl;
        }
    }
}

// вывести командную строку с текущим положением пользователя в иерархии папок
void Console::prompt()
{
    // вывести весь путь оcd(input.substr(3, std::string::npos)); // std::npos это статичная член-константа, которая означает "до конца строки"т корня до текущего каталога
    printFolderFromBeginning(m_currentFolder);

    std::cout << ">";
}

// вывести весь путь от корня до текущего каталога
void Console::printFolderFromBeginning(Folder *currentFolder)
{
    Folder *parent = currentFolder->getParent();

    if (parent != 0)
    {
        printFolderFromBeginning(parent);
    }

    // если это не корневой каталог, и родитель этой папки не является корневым каталогом
    if (parent != 0 && parent->getName() != "/")
        std::cout << "/"; // визуально разделим отдельные папки символом '/'

    std::cout << currentFolder->getName();
}

// перейти в другую папку
bool Console::cd(const std::string &name)
{
    // если это команда перейти на один уровень назад, то
    if (name == "..")
    {
        setCurrentFolder(m_currentFolder->getParent());
        return true;
    }

    // иначе, если первый символ -- это разделитель '/', то
    else if (name.substr(0, 1) == "/")
    {
        bool status;
        // ищем, если строка ещё содержит разделитель '/'
        size_t position = name.find('/', 1);

        // если разделитель '/' был найден
        if (position != std::string::npos)
        {
            // вызовем рекурсивно cd() с названием папки, чтобы продвинуться на один уровень
            status = cd (name.substr(1, position - 1));

            // если папка не существует
            if (status == false)
                return false; // то мы не продолжаем

            else // иначе, рекурсивно вызываем cd(), чтобы обработать оставшиеся папки
                cd(name.substr(position, std::string::npos));
        }

        else // если строка не содержит больше разделителей '/', то задаём корректные аргументы для последнего вызова ф-ции cd()
            cd(name.substr(1, std::string::npos));

        return true;
    }

    else // иначе, переходим в одну из папок текущего каталога
    {
        // пустую строку игнорируем (она получается после рекурсивной обработки оставшегося на конце разделителя '/'
        if (name == "")
            return true;

        size_t strSize = name.size();
        std::string name2;

        // если строка на конце содержит '/'
        if (name.substr(strSize - 1, 1) == "/")
            name2 = name.substr(0, strSize - 1); // то мы его удаляем
        else
            name2 = name; // иначе оставляем как есть

        Entity *ptr = m_currentFolder->findChild(name2);

        // если объект с таким именем существует
        if (ptr != 0)
        {
            // преобразуем в тип Folder, чтобы использовать специфическую ф-цию setCurrentFolder()
            Folder *folder = dynamic_cast<Folder *>(ptr);

            // если folder содержит указатель на объект, а не нуль, то это папка, а не файл, и можно применить ф-цию setCurrentFolder()
            if (folder != 0)
            {
                setCurrentFolder(folder); // меняем текущую папку
                return true;
            }

            else // иначе выводим сообщение об ошибке
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

// вывести содержимое папки
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

// создать папку
void Console::mkdir(std::string &name)
{
    Folder *ptr = new Folder(name, 0); // временно укажем в качестве родителя нулевой указатель
    m_currentFolder->addChild(ptr); // функция addChild сама проинициализирует родителя новой папки
}

// создать папку (версия с константным параметром)
void Console::mkdir(const std::string &name)
{
    Folder *ptr = new Folder(name, 0); // временно укажем в качестве родителя нулевой указатель
    m_currentFolder->addChild(ptr); // функция addChild сама проинициализирует родителя новой папки
}

// создать файл
void Console::create(std::string &name)
{
    File *ptr = new File(name, 0); // временно укажем в качестве родителя нулевой указатель
    m_currentFolder->addChild(ptr); // функция addChild сама проинициализирует родителя нового файла
}

// создать файл (версия с константным параметром)
void Console::create(const std::string &name)
{
    File *ptr = new File(name, 0); // временно укажем в качестве родителя нулевой указатель
    m_currentFolder->addChild(ptr); // функция addChild сама проинициализирует родителя нового файла
}

// удаляем папку или файл (с помощью полиморфизма -- через виртуальную функцию Entity::deleteSelf())
void Console::del(std::string &name)
{
    // сначала по имени находим адрес объекта (файла или папки), сохраняя его в указателе ptr
    Entity *ptr = m_currentFolder->findChild(name);

    // и, если имя было найдено в текущей папке,
    if (ptr != 0)
        ptr->deleteSelf(); // то удаляем объект (папку или файл)
}

// (версия с константным параметром) удаляем папку или файл (с помощью полиморфизма -- через виртуальную функцию Entity::deleteSelf())
void Console::del(const std::string &name)
{
    // сначала по имени находим адрес объекта (файла или папки), сохраняя его в указателе ptr
    Entity *ptr = m_currentFolder->findChild(name);

    // и проверяем, если имя было найдено в текущей папке,
    if (ptr != 0)
        ptr->deleteSelf(); // то удаляем объект (папку или файл)
    else
        std::cout << "del: unable to delete - no such file or directory" << std::endl;
}

// подсчитать и вывести кол-во элементов в текущей директории
void Console::cat()
{
    std::cout << m_currentFolder->getNumberOfChilds()
    << " elements in current folder\n\n";
}

// назначить текущую папку
void Console::setCurrentFolder(Folder *folder)
{
    if (folder != 0)
        m_currentFolder = folder;
}

// получить адрес текущей папки
Folder* Console::getCurrentFolder() const
{
    return m_currentFolder;
}
