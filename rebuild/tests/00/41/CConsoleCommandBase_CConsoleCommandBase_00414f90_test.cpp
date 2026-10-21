// test for 00414f90 ~CConsoleCommandBase
#include <stdio.h>
#include <string.h>
#include <new>
#include <stdlib.h>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
enum EGenericVarType { GENERIC_VAR_TYPE_NONE };
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CConsoleCommandParameters {
public:
    CMallocVector<EGenericVarType> ParamTypes;           // +0x00
};
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase() {}
    CCharString Name;                                    // +0x04
};
class CConsoleCommandBase : public CConsoleInputBase {
public:
    virtual ~CConsoleCommandBase();
    CConsoleCommandParameters ParamTypes;                // +0x08
};
CConsoleCommandBase::~CConsoleCommandBase()
{
}

static void* g_name; static int g_names;
CCharString::~CCharString() { g_name = this; ++g_names; }
int main()
{
    static double buf[4];
    CConsoleCommandBase* c = new (buf) CConsoleCommandBase;
    c->ParamTypes.ParamTypes._M_start = (EGenericVarType*)malloc(16);
    c->~CConsoleCommandBase();
    int ok = g_names == 1 && g_name == (char*)buf + 4 && sizeof(CConsoleCommandBase) == 0x14;
    c = new (buf) CConsoleCommandBase; c->ParamTypes.ParamTypes._M_start = 0;
    c->~CConsoleCommandBase();
    ok &= g_names == 2;
    printf(ok ? "00414f90_TEST PASS\n" : "00414f90_TEST FAIL\n");
    return ok ? 0 : 1;
}