// behaviour test for 0x009E6220 _STL::_Construct(pair<const CCharString,CShaderParserSymbol>*, const pair&): placement copy, nothing for a null slot
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

class CCharString {                                      // stand-in: a shared text pointer, copies counted
public:
    const char* PData;
    CCharString(const char* text) : PData(text) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_stringCopies; }   // stand-in for 0x0099EC30
    ~CCharString() { }
};

class CShaderParserSymbol {
public:
    enum EType { TYPE_NONE, TYPE_MACRO, TYPE_DYNAMIC_CONST };
    CCharString Name;
    CCharString Value;
    EType       Type;
    CShaderParserSymbol(const CCharString& name, const CCharString& value, EType type) : Name(name), Value(value), Type(type) { }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
}

typedef _STL::pair<const CCharString, CShaderParserSymbol> TValue;

int main()
{
    const char* key = "FOG";
    const char* name = "FOG_ENABLE";
    const char* value = "1";
    TValue source(CCharString(key), CShaderParserSymbol(CCharString(name), CCharString(value), CShaderParserSymbol::TYPE_MACRO));
    union { double align; unsigned char bytes[sizeof(TValue)]; } slot;
    g_stringCopies = 0;
    _STL::_Construct((TValue*)slot.bytes, source);
    TValue* built = (TValue*)slot.bytes;
    if (g_stringCopies != 3) { std::printf("009e6220_TEST FAIL\n"); return 1; }
    if (built->first.PData != key || built->second.Name.PData != name || built->second.Value.PData != value
        || built->second.Type != CShaderParserSymbol::TYPE_MACRO) { std::printf("009e6220_TEST FAIL\n"); return 1; }
    built->~TValue();
    std::printf("009e6220_TEST PASS\n");
    return 0;
}