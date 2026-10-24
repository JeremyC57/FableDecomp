// Behaviour test for 0x00527320 CThingObject::Alloc: the factory asks CThing's operator new for
// sizeof(CThingObject) with the bool argument false, runs the constructor on the block and returns it;
// a null block skips the constructor and returns 0.
#include <cstdio>
#include <cstddef>
#ifndef _MSC_VER
#define __fastcall
#endif
static size_t g_size;
static int g_flag = -1;
static int g_ctor;
static bool g_fail;
static void* g_block;
static void* g_this;
static unsigned char g_pool[0x400];

class CClass {
public:
    virtual void v00() {}
};
class CThing : public CClass {
protected:
    static void* __fastcall operator new(size_t size, bool flag) throw()
    {
        g_size = size; g_flag = flag ? 1 : 0;
        g_block = g_fail ? 0 : (void*)g_pool;
        return g_block;
    }
public:
    unsigned char _body[0xC0 - 4];
};
class CThingObject : public CThing {
public:
    unsigned char _body2[0xE0 - 0xC0];
    CThingObject() { ++g_ctor; g_this = this; }
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingObject::Alloc()
{
    return new (false) CThingObject;
}

int main()
{
    CClass* p = CThingObject::Alloc();
    if (g_size != sizeof(CThingObject) || g_flag != 0 || g_ctor != 1 || (void*)p != (void*)g_pool || g_this != (void*)g_pool) {
        std::printf("00527320_TEST FAIL\n"); return 1;
    }
    g_fail = true; g_ctor = 0; g_flag = -1;
    p = CThingObject::Alloc();
    if (p != 0 || g_ctor != 0 || g_flag != 0) { std::printf("00527320_TEST FAIL\n"); return 1; }
    std::printf("00527320_TEST PASS\n");
    return 0;
}