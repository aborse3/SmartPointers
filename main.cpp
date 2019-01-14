#include "SharedPtr.hpp"
#include <iostream>

using namespace std;

class Person
{
    int age;
    char* pName;

    public:
        Person(): pName(0),age(0)
        {
          cout<<"no param person ctor"<<endl;
        }
        Person(char* pName, int age): pName(pName), age(age)
        {
          cout<<"param person ctor"<<endl;
        }
      protected:
        ~Person()
        {
          cout<<"person destructor"<<endl;
        }
      public:
        void Display()
        {
            printf("Name = %s Age = %d \n", pName, age);
        }
        void Shout()
        {
            printf("Ooooooooooooooooo");
        }
};

class derived : public Person{
    int age1;
  public:
    derived(){
      cout<<"derived constructor";
    }
  ~derived(){
    cout<<"derived destructor";
  }
};

int main()
{
    //cs540::SharedPtr<Person> p1(new derived);
    derived *p1= new derived;
    delete p1;
    /*cs540::SharedPtr<Person> p(new Person("Scott", 25));
    p->Display();
    {
        cs540::SharedPtr<Person> q;
        q = p;
        q->Display();
        // Destructor of Q will be called here..
    }
    p->Display();*/
}
