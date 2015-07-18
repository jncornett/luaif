#include <iostream>
#include <string>

#include <luajit-2.0/lua.hpp>
#include "lua.h"
#include "lib.h"
#include "obj.h"

using namespace std;

class LogGroup
{
public:
    LogGroup(string name)
    {
        indent();
        cout << name << ":";
        end();
        level++;
    }

    ~LogGroup()
    { level--; }

    void indent()
    {
        for ( int i = 0; i < level; i++ )
            cout << "  ";

        if ( level > 0 )
            cout << "- ";
    }

    void end()
    { cout << endl; }

    void field(string name)
    { 
        indent();
        cout << name << ": ";
    }

private:
    static int level;
};

int LogGroup::level = 0;


void dump_stack(lua_State* L)
{
    const char* s = "";
    LogGroup log("Dump stack");
    int top = lua_gettop(L);
    log.field("top"); cout << top; log.end();
    if ( top > 0 )
    {
        LogGroup items("Items");
        for ( int i = top; i > 0; i-- )
        {
            auto t = lua_type(L, i);
            LogGroup item(to_string(i));
            item.field("type"); cout << lua_typename(L, t); item.end();
            switch ( t )
            {
                case LUA_TNUMBER:
                    item.field("value"); cout << lua_tonumber(L, i); item.end();
                    break;

                case LUA_TBOOLEAN:
                    item.field("value");
                    if ( lua_toboolean(L, i) )
                        cout << true;
                    else
                        cout << false;

                    item.end();
                    break;

                case LUA_TSTRING:
                    size_t len;
                    s = lua_tolstring(L, i, &len);
                    item.field("value"); cout << string(s, len); item.end();
                    break;

                case LUA_TUSERDATA:
                    item.field("value"); cout << lua_touserdata(L, i); item.end();
                    break;

                case LUA_TLIGHTUSERDATA:
                    item.field("value"); cout << lua_topointer(L, i); item.end();
                    break;

                default:
                    break;
            }
        }
    }
}

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    Lua::Type<Obj>::install(L);
    luaL_dostring(L, "Obj.new():explode()");
    lua_settop(L, 0);
    luaL_dostring(L, "print(Obj.new())");
    lua_close(L);
    return 0;
}
