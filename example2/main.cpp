#include <iostream>
#include <thread>
#include <memory>

using namespace std;

class background_task
{
    void do_something() const
    {
        cout << "do something" << endl;
    }
    void do_something_else() const
    {
        cout << "do something else" << endl;
    }
public:
    background_task(){}

    void do_this_thing()
    {
        cout << "do this thing" << endl;
    }

    void operator()() const
    {
        do_something();
        do_something_else();
    }
};

void process_background_task(std::unique_ptr<background_task> apBt)
{
    try
    {
       // (*apBt)();
       //apBt->do_this_thing();
       (*apBt)();
    }
    catch(...)
    {
        cout << "error happened" << endl;
    }
}

int main()
{
    //background_task bt;
    //std::thread t(bt);
    //t.join();

    std::unique_ptr<background_task> p(new background_task);
    p->do_this_thing();
    std::thread t(process_background_task,std::move(p));
    if(p==nullptr)
        cout << "p is nullptr" << endl;
    t.join();
    std::thread u(process_background_task,std::unique_ptr<background_task>(new background_task));
    u.join();
    return 0;
}
