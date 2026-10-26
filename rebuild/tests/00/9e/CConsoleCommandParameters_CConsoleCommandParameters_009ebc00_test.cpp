#include <cstdio>
#include <cstdlib>

// Host model of CConsoleCommandParameters(EGenericVarType, EGenericVarType, EGenericVarType): an empty STLport vector of parameter types,
// then AddParameterType (push_back) of each argument in order, growing through _M_insert_overflow (old + max(old, 1)).
enum EGenericVarType { GENERIC_VAR_TYPE_NONE, GENERIC_VAR_TYPE_INT, GENERIC_VAR_TYPE_FLOAT, GENERIC_VAR_TYPE_STRING };

struct CTypeVector {
    EGenericVarType* Start; EGenericVarType* Finish; EGenericVarType* End;
    CTypeVector() : Start(0), Finish(0), End(0) { }
    ~CTypeVector() { if (Start) std::free(Start); }
    void insert_overflow(const EGenericVarType& x)
    {
        unsigned int old = (unsigned int)(Finish - Start);
        unsigned int len = old + (old > 1 ? old : 1);
        EGenericVarType* ns = (EGenericVarType*)std::malloc(len * sizeof(EGenericVarType));
        EGenericVarType* nf = ns;
        for (EGenericVarType* p = Start; p != Finish; ++p, ++nf) *nf = *p;
        *nf++ = x;
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
    void push_back(const EGenericVarType& x)
    {
        if (Finish != End) { *Finish = x; ++Finish; }
        else insert_overflow(x);
    }
};

class CConsoleCommandParameters { public:
    CTypeVector ParamTypes;
    CConsoleCommandParameters(EGenericVarType type1, EGenericVarType type2, EGenericVarType type3)
    {
        AddParameterType(type1);
        AddParameterType(type2);
        AddParameterType(type3);
    }
    void AddParameterType(const EGenericVarType& type) { ParamTypes.push_back(type); }
};

int main()
{
    CConsoleCommandParameters p(GENERIC_VAR_TYPE_STRING, GENERIC_VAR_TYPE_INT, GENERIC_VAR_TYPE_FLOAT);
    if (p.ParamTypes.Finish - p.ParamTypes.Start != 3) { std::printf("009ebc00_TEST FAIL\n"); return 1; }
    if (p.ParamTypes.Start[0] != GENERIC_VAR_TYPE_STRING || p.ParamTypes.Start[1] != GENERIC_VAR_TYPE_INT ||
        p.ParamTypes.Start[2] != GENERIC_VAR_TYPE_FLOAT) {
        std::printf("009ebc00_TEST FAIL\n");
        return 1;
    }
    if (p.ParamTypes.End - p.ParamTypes.Start != 4) { std::printf("009ebc00_TEST FAIL\n"); return 1; }   // 0 -> 1 -> 2 -> 4 slots
    std::printf("009ebc00_TEST PASS\n");
    return 0;
}