// Entity.h
//  ласс, от которого наследуют клсассы Folder и File
#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#define DLLEXPORT __declspec(dllexport)

// предварительное объ€вление класса Folder,
// чтобы мы могли создать указатель Folder *m_parent
class Folder;


// абстрактный базовый класс дл€ Folder и File
class DLLEXPORT Entity
{
public:
    Entity(); // конструктор по умолчанию
    Entity(const std::string &name, Folder * const parent); // конструктор с параметрами
    virtual ~Entity() {} // виртуальный деструктор

    // set и get функции
    void setParent(Folder * const parent);
    Folder* getParent() const;

    void setName(const std::string &name);
    std::string getName() const;

    // чиста€ виртуальна€ функци€, котора€ удал€ет либо папку, либо файл,
    // в зависимости от типа вызвавшего еЄ объекта
    virtual void deleteSelf() = 0;
private:
    std::string m_name; // им€ файла или папки
    Folder *m_parent; // переменна€, содержаща€ адрес родител€
}; // end class Entity

#endif // ENTITY_H
