// std::_Construct<CWideString,CWideString>(CWideString* p, const CWideString& value) -- placement-copy-constructs a CWideString (STLport, null-checked placement new)
// Leaf: _Construct
// Retail: Fable.exe @ 0x0040DD00, 11 bytes, LTCG register convention modelled as `__fastcall` (p in ecx, value in edx).
// Callee: CWideString::CWideString(const CWideString&) 0x0099B720. The `test ecx,ecx ; je` is VC7.1's null test of
// the throw()-declared placement operator new's result. Callers: three CWideString container fills at 0x00E7FE84..
// (catalog label "_Cons_val<pair<CWideString const,float>>" names a sibling instantiation).
inline void* operator new(unsigned int, void* where) throw() { return where; }

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);               // 0x0099B720
};

namespace std {
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
template void __fastcall _Construct<CWideString, CWideString>(CWideString* p, const CWideString& value);
}