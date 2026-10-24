// behaviour test for 009edfe0: placement-copy-construct a polymorphic CGenericVar into raw storage: the
// vtable is installed and every typed value (string deep-copied) is carried over.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_STRING = 5 };

struct CGenericVar {
    virtual long Kind() const { return 3; }
    EGenericVarType Type;
    CCharString     ValString;
    long            ValSLONG;
    unsigned long   ValULONG;
    bool            ValBool;
    float           ValFloat;
    CGenericVar() : Type(GENERIC_VAR_TYPE_NONE), ValString(""), ValSLONG(0), ValULONG(0), ValBool(false), ValFloat(0) { }
};

static void Cons_val(CGenericVar* ptr, const CGenericVar& value)
{
    new ((void*)ptr) CGenericVar(value);
}

int main()
{
    int ok = 1;
    CGenericVar source;
    void* raw = ::operator new(sizeof(CGenericVar));
    CGenericVar* dest = (CGenericVar*)raw;
    source.Type = GENERIC_VAR_TYPE_STRING;
    source.ValString.~CCharString();
    new (&source.ValString) CCharString("quest_done");
    source.ValSLONG = -12;
    source.ValULONG = 4000000000UL;
    source.ValBool = true;
    source.ValFloat = 0.75f;
    Cons_val(dest, source);
    if (dest->Kind() != 3 || dest->Type != GENERIC_VAR_TYPE_STRING)
        ok = 0;
    if (std::strcmp(dest->ValString.PStringData, "quest_done") != 0 || dest->ValString.PStringData == source.ValString.PStringData)
        ok = 0;
    if (dest->ValSLONG != -12 || dest->ValULONG != 4000000000UL || !dest->ValBool || dest->ValFloat != 0.75f)
        ok = 0;
    dest->~CGenericVar();
    ::operator delete(raw);
    if (!ok) {
        std::printf("009edfe0_TEST FAIL\n");
        return 1;
    }
    std::printf("009edfe0_TEST PASS\n");
    return 0;
}