#include "lib.h"
#include "obj.h"
#include <iostream>

#define LNAME "Obj"

using Object = Lua::Type<Obj>;

template<>
const char* Object::name = LNAME;

int explode(lua_State* L)
{
    auto o = Object::get(L);
    o->explode();
    return 0;
}

template<>
const luaL_reg Object::methods[] =
{
    { "explode", explode },
    { nullptr, nullptr }
};

template<>
const luaL_reg Object::metamethods[] =
{
    { nullptr, nullptr }
};

template<>
int Object::l_ctor(lua_State* L)
{
    auto t = create_userdata(L);
    *t = new type(2, 3);
    return 1;
}

template<>
int Object::l_dtor(lua_State* L)
{
    delete get(L);
    return 0;
}

template<>
int Object::l_str(lua_State* L)
{
    lua_pushfstring(L, "%s@%p", name, get(L));
    return 1;
}
