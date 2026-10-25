// Behaviour test for 0x009EA150 CGenericVar::CGenericVar().
#include <cstdio>
#include <cstring>
#include <new>

class CCharString {
public:
    const char* PData;
    CCharString() : PData("") {}
};
enum EGenericVarType { GENERIC_VAR_TYPE_NONE = 0, GENERIC_VAR_TYPE_SLONG = 1, GENERIC_VAR_TYPE_STRING = 5 };
class CGenericVar {
public:
    EGenericVarType Type;
    CCharString     ValString;
    long            ValSLONG;
    CGenericVar() { Type = GENERIC_VAR_TYPE_NONE; }
    virtual ~CGenericVar() {}
};

int main()
{
    static double storage[(sizeof(CGenericVar) + 7) / 8 + 1];
    unsigned char* bytes = (unsigned char*)storage;
    for (unsigned int i = 0; i < sizeof(storage); ++i) bytes[i] = 0xCD;
    CGenericVar* v = new (storage) CGenericVar;
    if (!(v->Type == GENERIC_VAR_TYPE_NONE && std::strcmp(v->ValString.PData, "") == 0)) {
        std::printf("009ea150_TEST FAIL\n");
        return 1;
    }
    std::printf("009ea150_TEST PASS\n");
    return 0;
}