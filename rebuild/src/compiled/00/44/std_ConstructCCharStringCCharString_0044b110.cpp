// std::_Construct<CCharString,CCharString>(CCharString* p, const CCharString& value) -- placement-copy-constructs a CCharString (STLport, null-checked placement new)
// Leaf: _Construct
// Retail: Fable.exe @ 0x0044B110, 11 bytes, LTCG register convention modelled as `__fastcall` (p in ecx, value in edx).
// Callee: CCharString::CCharString(const CCharString&) 0x0099EC30. The `test ecx,ecx ; je` is VC7.1's null test of
// the throw()-declared placement operator new's result. Callers: the CCharString container fills at 0x00DAB52C.. (20+ sites)
// (catalog label "_Cons_val<pair<CWideString const,float>>" names a sibling instantiation).
inline void* operator new(unsigned int, void* where) throw() { return where; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);               // 0x0099EC30
};

namespace std {
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
template void __fastcall _Construct<CCharString, CCharString>(CCharString* p, const CCharString& value);
}