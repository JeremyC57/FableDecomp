// _STL::__insertion_sort(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, CCompareScriptName comp) -- STLport insertion sort over the script registry's 0x18-byte pair<CCharString, CScriptInfo> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB6EA0, 112 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00CB6CE0 __linear_insert(first, last, value, comp) (manifest "ScriptInfo_InsertElement"; only this body
// calls it), 0x0099EC30 CCharString::CCharString(const CCharString&) (x2).
// Element: std::pair<CCharString, CScriptInfo>, Ego_r.pdb CScriptInfo (0x14): AllocFunc +0, MasterScriptDataAlloc +4,
// CCharString Name +8, long ID +0xC, bool MasterScript +0x10 -- the model of the landed __copy_backward 0x00CB62F0.
// Label: catalogued as `_Insertion_sort1<pair<float,CCharString>*, CKeyPairCompareLess<float,CCharString> >`
// (propagated over a different instantiation); the code is STLport's `__insertion_sort`.
// Shape: STLport's explicit pair copy constructor builds the by-value `*i` in the argument area (`sub esp,18h ; mov
// esi,esp`): the key string, then CScriptInfo's implicit memberwise copy (two function pointers, the Name string, ID and
// the bool byte), its sources read through a second induction pointer at Name (`lea edi,[ebx+0Ch]`).
struct CCharString {                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString(const CCharString& other);   // 0x0099EC30
};

struct CScriptBase;
struct CScriptDataBase;
struct CGameScriptInterfaceBase;

struct CScriptInfo {                         // Ego_r.pdb, 0x14 bytes (implicit copy)
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                   // +0x08
    long             ID;                     // +0x0C
    bool             MasterScript;           // +0x10
};

struct PairCharStringScriptInfo {            // STLport std::pair<CCharString, CScriptInfo>, 0x18 bytes
    CCharString first;                       // +0x00 key
    CScriptInfo second;                      // +0x04
    PairCharStringScriptInfo(const PairCharStringScriptInfo& o) : first(o.first), second(o.second) { }
};

struct CCompareScriptName {                  // 4-byte comparator slot passed by value
    void* m_context;
};

void __fastcall __linear_insert(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo value, CCompareScriptName comp);   // 0x00CB6CE0

void __fastcall __insertion_sort(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, CCompareScriptName comp)
{
    if (first == last)
        return;
    for (PairCharStringScriptInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}