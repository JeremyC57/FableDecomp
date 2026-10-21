// std::_Construct<std::pair<CWideString,C3DVector>,std::pair<CWideString,C3DVector> >(std::pair<CWideString,C3DVector>* p, const std::pair<CWideString,C3DVector>& value) -- placement-copy-constructs a {CWideString, C3DVector} pair (null-checked placement new)
// Leaf: _Construct
// Retail: Fable.exe @ 0x0043F400, 37 bytes, LTCG register convention modelled as `__fastcall` (p in ecx, value in edx).
// Callee: CWideString::CWideString(const CWideString&) 0x0099B720 (the first member); the C3DVector is copied
// member by member through its user copy constructor inside the pair's implicit copy constructor. Catalog label "_Cons_val<pair<CCharString,C3DVector>>":
// the string copy ctor called is the CWideString one (as in 0x0040DD00).
inline void* operator new(unsigned int, void* where) throw() { return where; }

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);               // 0x0099B720
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}   // memberwise user copy ctor (engine C3DVector)
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
template void __fastcall _Construct<pair<CWideString, C3DVector>, pair<CWideString, C3DVector> >(pair<CWideString, C3DVector>* p, const pair<CWideString, C3DVector>& value);
}