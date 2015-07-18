#pragma once

struct Obj 
{
    int a;
    int b;

    inline int get_a()
    { return a; }

    void do_bar();
    static void do_foo();

    void explode();

    Obj(int, int);
    ~Obj();
};
