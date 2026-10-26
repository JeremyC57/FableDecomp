// Behaviour test for 0x009EE100 UninitFillN_CGenericVar_009ee100: copy-construct n CGenericVar values (vtable, type, string, scalars).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pStringData;
    CCharString() : m_pStringData("") { }
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_STRING };
class CGenericVar {
public:
    EGenericVarType Type;
    CCharString     ValString;
    long            ValSLONG;
    unsigned long   ValULONG;
    bool            ValBool;
    float           ValFloat;
    CGenericVar() : Type(GENERIC_VAR_TYPE_NONE), ValSLONG(0), ValULONG(0), ValBool(false), ValFloat(0.0f) { }
    virtual ~CGenericVar() { }
    virtual int Kind() const { return 7; }
};

static CGenericVar* UninitFillN(CGenericVar* first, unsigned int n, const CGenericVar& x, const __false_type&)
{
    CGenericVar* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CGenericVar(x);
    return cur;
}

int main()
{
    static const char* text = "value";
    CGenericVar x;
    x.Type = GENERIC_VAR_TYPE_STRING; x.ValString.m_pStringData = text; x.ValSLONG = -3; x.ValULONG = 9u; x.ValBool = true; x.ValFloat = 1.25f;
    static double storage[3 * sizeof(CGenericVar) / sizeof(double) + 1];
    CGenericVar* dst = (CGenericVar*)storage;
    __false_type tag;
    CGenericVar* r = UninitFillN(dst, 3, x, tag);
    bool ok = r == dst + 3 && g_stringCopies == 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].Type != GENERIC_VAR_TYPE_STRING || dst[k].ValString.m_pStringData != text || dst[k].ValSLONG != -3 || dst[k].ValULONG != 9u
            || !dst[k].ValBool || dst[k].ValFloat != 1.25f || dst[k].Kind() != 7) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || g_stringCopies != 3) ok = false;
    if (!ok) { std::printf("009ee100_TEST FAIL\n"); return 1; }
    std::printf("009ee100_TEST PASS\n");
    return 0;
}