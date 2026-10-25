// _STL::_Construct(pair<const CCharString,CShaderParserSymbol>* p, const pair<const CCharString,CShaderParserSymbol>& val) -- STLport: placement-copy one shader-symbol map value (key string, then the symbol's two strings and its type)
// Leaf: ??$_Construct@U?$pair@$$CBVCCharString@@VCShaderParserSymbol@@@_STL@@U12@@_STL@@YIXPAU?$pair@$$CBVCCharString@@VCShaderParserSymbol@@@0@ABU10@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6220, 51 bytes, `__fastcall` (p in ecx, &val in edx), `ret`.
// Identity: the element of the CShaderPreParser symbol map (FableWin.pdb ?FindSymbol@CShaderPreParser@@AAEAAVCShaderParserSymbol@@
//   ABVCCharString@@@Z; map<CCharString, CShaderParserSymbol>, whose node values are pair<const CCharString,
//   CShaderParserSymbol>). Label correction: the catalog's "_Cons_val<allocator<pair<...>>,...>" is the propagated
//   Dinkumware spelling; retail STL is STLport 4.x, where the node value is built by _Construct.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (out of line, x3).
// Layout: CShaderParserSymbol (12 bytes in retail; Ego_r.pdb sizeof 12 with the first word unnamed, Value +0x04,
//   Type +0x08): two CCharStrings and the EType -- the PDB constructor ??0CShaderParserSymbol@@QAE@ABVCCharString@@0W4EType@0@@Z
//   takes the two strings and the type; the first one's member name is not in the PDB (descriptive "Name").
//   CCharString is the 4-byte TLC counted string (one data pointer).
// Shape: STLport `new (__p) _T1(__val)` -- the placement new is null-tested (`test edi,edi ; je`); the pair's copy
//   constructor and CShaderParserSymbol's implicit one are inline (three string copies, then the type dword). The /Gr
//   TU makes the template `__fastcall`.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }   // placement new (STLport <new>)

class CCharString {                                      // 4 bytes
public:
    void* PData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();
};

class CShaderParserSymbol {                              // 12 bytes
public:
    enum EType { TYPE_NONE };
    CCharString Name;                                    // +0x00
    CCharString Value;                                   // +0x04
    EType       Type;                                    // +0x08
};

namespace _STL {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template void __fastcall _Construct<pair<const CCharString, CShaderParserSymbol>, pair<const CCharString, CShaderParserSymbol> >(
    pair<const CCharString, CShaderParserSymbol>*, const pair<const CCharString, CShaderParserSymbol>&);

}