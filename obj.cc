#include "obj.h"
#include <iostream>

Obj::Obj(int A, int B)
{ a = A; b = B; }

Obj::~Obj()
{ std::cout << "Obj destructor called for " << this << std::endl; }

void Obj::do_bar()
{ std::cout << "Obj " << this << "is doing bar" << std::endl; }

void Obj::do_foo()
{ std::cout << "Obj static doing foo" << std::endl; }

void Obj::explode()
{ std::cout << "PFGGHHHHHHHH" << std::endl; }
