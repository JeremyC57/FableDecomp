// std::fill(CClassTypeInfo* first, CClassTypeInfo* last, const CClassTypeInfo& value) -- assign one class-factory type record to every element of a range
// Leaf: ??$fill@PAVCClassTypeInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37BE0, 74 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 20-byte stride = {4-byte member +0 assigned through 0x00A37940 (a list copy-assign: the STLport list is
//   one node pointer), long +4, bool +8, bool +9, long +0xc, CCharString +0x10 (0x0099EFB0)}: Ego_r.pdb
//   CClassTypeInfo {list<CClass*> TypeList; unsigned long NoAllocated; bool Used; bool Local; CClass* (*AllocFunc)();
//   CCharString Name} (FableWin.pdb has vector<CClassTypeInfo>). The manifest's "pair<CCharString, CCharString>" is
//   propagated. One caller, 0x00A38035.
// Callees: list<CClass*>::operator= @ 0x00A37940, CCharString::operator= @ 0x0099EFB0.
// Shape: STLport fill with CClassTypeInfo's implicit operator= inline, member by member; &val.Name hoisted, the
//   AllocFunc store scheduled after the Name argument push.

class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
template <class T> class CList {                         // STLport list, 4 bytes (node pointer)
public:
    void* _M_node;
    CList& operator=(const CList& other);                // 0x00A37940 for CClass*
};
class CClass;

class CClassTypeInfo {                                   // 20 bytes
public:
    CList<CClass*> TypeList;                             // +0x00
    unsigned long  NoAllocated;                          // +0x04
    bool           Used;                                 // +0x08
    bool           Local;                                // +0x09
    CClass*        (__cdecl* AllocFunc)();               // +0x0c
    CCharString    Name;                                 // +0x10
};

namespace std {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<CClassTypeInfo*, CClassTypeInfo>(CClassTypeInfo*, CClassTypeInfo*, const CClassTypeInfo&);