// Behaviour test for 0x00478706 CAbilityTrackDef::`scalar deleting destructor': the virtual destructor runs once, and the storage is
// freed only on the deleting path (delete through a base pointer); a destroyed automatic object is not freed.
#include <stdio.h>
#include <stdlib.h>
#include <new>
static long g_baseDtor = 0, g_dtor = 0, g_frees = 0;
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase() { ++g_baseDtor; }
};
class CAbilityTrackDef : public CParentDefClassBase {
public:
    long Payload;
    CAbilityTrackDef() : Payload(5) { }
    virtual ~CAbilityTrackDef() { ++g_dtor; }
    static void* operator new(size_t size) { return malloc(size); }
    static void operator delete(void* p) { ++g_frees; free(p); }
};
int main()
{
    CParentDefClassBase* p = new CAbilityTrackDef;
    delete p;
    bool ok = g_dtor == 1 && g_baseDtor == 1 && g_frees == 1;
    {
        CAbilityTrackDef local;
    }
    ok = ok && g_dtor == 2 && g_baseDtor == 2 && g_frees == 1;
    if (!ok) { printf("00478706_TEST FAIL\n"); return 1; }
    printf("00478706_TEST PASS\n");
    return 0;
}
