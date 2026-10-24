// Behaviour test for 0x009EA6C0 CConsoleCommandParameters::GetCorrectlyTypedParameter: the result carries the type of
// parameter <index> and a default (empty) string.
#include <cstdio>
#include <cstring>

static int gLive = 0;

class CCharString
{
public:
    const char* PStringData;
    CCharString() : PStringData("") { ++gLive; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++gLive; }
    ~CCharString() { --gLive; }
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_SLONG, GENERIC_VAR_TYPE_ULONG, GENERIC_VAR_TYPE_FLOAT,
                       GENERIC_VAR_TYPE_BOOL, GENERIC_VAR_TYPE_STRING };

class CGenericVar
{
public:
    EGenericVarType Type;
    CCharString     ValString;
    long            ValSLONG;
    unsigned long   ValULONG;
    bool            ValBool;
    float           ValFloat;

    CGenericVar() { Type = GENERIC_VAR_TYPE_NONE; }
    virtual ~CGenericVar() {}
};

template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int n) const { return *(_M_start + n); }
};

class CConsoleCommandParameters
{
public:
    vector<EGenericVarType> ParamTypes;
    CGenericVar GetCorrectlyTypedParameter(long index) const;
};

CGenericVar CConsoleCommandParameters::GetCorrectlyTypedParameter(long index) const
{
    CGenericVar var;
    var.Type = ParamTypes[index];
    return var;
}

int main()
{
    EGenericVarType types[3] = { GENERIC_VAR_TYPE_BOOL, GENERIC_VAR_TYPE_STRING, GENERIC_VAR_TYPE_FLOAT };
    CConsoleCommandParameters params;
    params.ParamTypes._M_start = types;
    params.ParamTypes._M_finish = types + 3;
    params.ParamTypes._M_end_of_storage = types + 3;
    bool ok = true;
    for (long i = 0; i < 3; ++i)
    {
        CGenericVar v = params.GetCorrectlyTypedParameter(i);
        ok = ok && v.Type == types[i] && std::strcmp(v.ValString.PStringData, "") == 0;
    }
    ok = ok && gLive == 0;
    if (!ok) { std::printf("009ea6c0_TEST FAIL\n"); return 1; }
    std::printf("009ea6c0_TEST PASS\n");
    return 0;
}