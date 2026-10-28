// std::sort_heap(SScriptInfoPair* first, SScriptInfoPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the script registry's (CCharString, CScriptInfo) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB6F10, length 155 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...>` is propagated
// over the byte-identical skeleton. Stride 0x18 (0x2AAAAAAB / sar 2); the callee 0x00CB6B00 (manifest `_Pop_heap_0<pair<
// CCharString,CScriptInfo>*,...>`) is the SGI `__pop_heap` of that pair (`ret 24h`), feeding the landed `_Adjust_heap<
// pair<CCharString,CScriptInfo>*,...>` @ 0x00CB68C0. Ego_r.pdb CScriptInfo (20 bytes): AllocFunc, MasterScriptDataAlloc,
// CCharString Name, long ID, bool MasterScript.
// Callees: __pop_heap @ 0x00CB6B00 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the key string, then
// CScriptInfo's implicit copy off the slot base (two function pointers, the Name string, ID, the MasterScript byte).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo { public:                                    // Ego_r.pdb, 20 bytes
    CScriptBase* (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                              // +0x04
    CCharString Name;                                   // +0x08
    long        ID;                                     // +0x0c
    bool        MasterScript;                           // +0x10
};
struct SScriptInfoPair {                                // std::pair<CCharString, CScriptInfo> (0x18 bytes)
    CCharString first;                                  // +0x00
    CScriptInfo second;                                 // +0x04
};
struct CKeyPairCompareLess { };                         // CKeyPairCompareLess<CCharString, CScriptInfo>: empty, by value

void __fastcall __pop_heap(SScriptInfoPair* first, SScriptInfoPair* last, SScriptInfoPair* result, SScriptInfoPair value,
                           CKeyPairCompareLess comp, int*);   // 0x00CB6B00

inline void __pop_heap_aux(SScriptInfoPair* first, SScriptInfoPair* last, SScriptInfoPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SScriptInfoPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SScriptInfoPair* first, SScriptInfoPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SScriptInfoPair*)0, comp);
}

void __fastcall sort_heap(SScriptInfoPair* first, SScriptInfoPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}