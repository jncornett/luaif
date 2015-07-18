#pragma once

#include <string>
#include <luajit-2.0/lua.hpp>

namespace Lua
{
#define SELF_UPVALUEINDEX   1

template<typename T>
T** new_userdata(lua_State* L, const char* name)
{ return reinterpret_cast<T**>(lua_newuserdata(L, sizeof(T*))); }

template<typename T>
T** check_userdata(lua_State* L, int n, const char* name)
{ return static_cast<T**>(const_cast<void*>(luaL_checkudata(L, n, name))); }

template<typename T>
class Type
{
public:
    using type = T;
    static const char* name;

    static void install(lua_State*);
    static inline T* get(lua_State* L, int n = 1)
    { return *check_userdata<T>(L, n, name); }

private:
    static T** create_userdata(lua_State*);

    static int l_ctor(lua_State* L);
    static int l_dtor(lua_State* L);
    static int l_str(lua_State* L);

    static const luaL_reg methods[];
    static const luaL_reg metamethods[];
};

template<typename T>
void Type<T>::install(lua_State* L)
{
    const luaL_reg method_overrides[] =
    {
        { "new", l_ctor },
        { nullptr, nullptr }
    };

    const luaL_reg metamethod_overrides[] =
    {
        { "__gc", l_dtor },
        { "__tostring", l_str },
        { nullptr, nullptr }
    };

    int top, mt, t;

    top = lua_gettop(L);

    // register user methods
    luaL_register(L, name, methods);
    t = lua_gettop(L);

    // register method overrides
    luaL_register(L, nullptr, method_overrides);

    // register metamethods
    luaL_newmetatable(L, name);
    mt = lua_gettop(L);
    luaL_register(L, nullptr, metamethods);

    // register metamethod overrides
    luaL_register(L, nullptr, metamethod_overrides);

    // meta.__index = table (set method lookup table)
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, t);
    lua_rawset(L, mt);

    // meta.__meta = table (hide the metatable)
    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, t);
    lua_rawset(L, mt);

    lua_settop(L, top);
}

template<typename T>
T** Type<T>::create_userdata(lua_State* L)
{
    auto t = new_userdata<T>(L, name);
    int ud = lua_gettop(L);

    luaL_getmetatable(L, name);
    lua_setmetatable(L, ud);

    return t;
}

template<typename T>
class Instance
{
public:
    using type = T;

    static void install(lua_State*, T* instance);
    static T* get(lua_State*);

private:
    static const luaL_reg methods[];
    static const char* name;
};

template<typename T>
void Instance<T>::install(lua_State* L, T* instance)
{
    const luaL_reg empty_reg[] = { { nullptr, nullptr } };
    int top, t, i;
    top = lua_gettop(L);

    // create methods table with empty registration table
    luaL_register(L, name, empty_reg);
    t = lua_gettop(L);

    // push pointer to instance
    // using userdata instead of lightuserdata allows us to do type checking
    // when retrieving the value
 
    auto ig = new_userdata<T>(L, name);
    *ig = instance;
    i = lua_gettop(L);

    const luaL_reg *entry = methods;
    while ( entry->func )
    {
        // make a copy of instance userdata
        lua_pushvalue(L, i);
        
        // push method
        lua_pushcclosure(L, entry->func, 1);

        // add to methods table
        lua_setfield(L, t, entry->name);

        ++entry;
    }

    // Reset the stack
    lua_settop(L, top);
}

template<typename T>
T* Instance<T>::get(lua_State* L)
{
    int up = lua_upvalueindex(SELF_UPVALUEINDEX);
    return *check_userdata<T>(L, up, name);
}

}
