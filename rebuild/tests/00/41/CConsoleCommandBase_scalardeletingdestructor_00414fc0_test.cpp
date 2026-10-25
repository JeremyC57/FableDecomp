// Behaviour test for CConsoleCommandBase::`scalar deleting destructor' (0x00414FC0): deleting through the base pointer
// frees the parameter-type buffer and destroys the Name string once.
#include <cstdio>
#include <stdlib.h>

static int g_names = 0;
class CCharString {
public:
    void* PData;
    CCharString() : PData(0) { ++g_names; }
    ~CCharString() { --g_names; }
};
enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_INT };
template <class T> class CMallocVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CMallocVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CConsoleCommandParameters {
public:
    CMallocVector<EGenericVarType> ParamTypes;
};
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase() {}
    CCharString Name;
};
class CConsoleCommandBase : public CConsoleInputBase {
public:
    virtual ~CConsoleCommandBase();
    CConsoleCommandParameters ParamTypes;
};
CConsoleCommandBase::~CConsoleCommandBase()
{
}

int main()
{
    CConsoleCommandBase* c = new CConsoleCommandBase;
    c->ParamTypes.ParamTypes._M_start = (EGenericVarType*)malloc(2 * sizeof(EGenericVarType));
    c->ParamTypes.ParamTypes._M_finish = c->ParamTypes.ParamTypes._M_start + 1;
    c->ParamTypes.ParamTypes._M_start[0] = GENERIC_VAR_TYPE_INT;
    if (g_names != 1) { std::printf("00414fc0_TEST FAIL\n"); return 1; }
    CConsoleInputBase* base = c;
    delete base;
    if (g_names != 0) { std::printf("00414fc0_TEST FAIL\n"); return 1; }
    CConsoleCommandBase* empty = new CConsoleCommandBase;   // null buffer: no free
    delete empty;
    if (g_names != 0) { std::printf("00414fc0_TEST FAIL\n"); return 1; }
    std::printf("00414fc0_TEST PASS\n");
    return 0;
}