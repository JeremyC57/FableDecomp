// _global::__pop_heap(SRegisteredTCPair* first, SRegisteredTCPair* last, SRegisteredTCPair* result, SRegisteredTCPair value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core: `*result = *first`, then __adjust_heap(first, 0, last - first, value, comp)
// Leaf: __pop_heap
// Retail: Fable.exe @ 0x004E1D20, 72 bytes (`ret 1Ch`: result, the 16-byte value, the comparator and
//         the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Callees: pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (manifest
//          "pair<CCharString,CCharString>" propagated), its copy constructor 0x004D788D (builds the
//          by-value argument in the argument area), __adjust_heap 0x004DFDBB (manifest label),
//          CCharString::~CCharString 0x0099EAE0 (the by-value parameter destroyed by this callee).
// Label: the manifest's "CTCThingOwner::SetOwnedObjectsSerialisationSectionName" is propagated over this
// heap helper of the registered-TC table sort (the landed __pop_heap_aux 0x0059474E is the sibling
// front end for another element type).
// Element: pair<CCharString, CTCBase::CRegisteredTCInfo>, 16 bytes; the info's CCharString sits at
// +0x0C (value destroyed as `lea ecx,[ebp+18h]` then `lea ecx,[ebp+0Ch]`, reverse member order).
// Shape: a frame-pointer body that still uses esi/edi and pushes the comparator straight from its slot
//        (`push dword ptr [ebp+1Ch]`) = `optimize("y",off)` + `("s",on)`; arguments right to left:
//        comp, the value copy (`sub esp,10h ; mov ecx,esp`), len = (last - first) / 16, then
//        holeIndex 0 in edx and first in ecx.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* PStringData;
    ~CCharString();                                             // 0x0099EAE0
};

struct CRegisteredTCInfo {                                      // CTCBase::CRegisteredTCInfo, 12 bytes
    long m_type;
    long m_flags;
    CCharString m_name;                                         // +0x08
};

struct SRegisteredTCPair {                                      // pair<CCharString, CRegisteredTCInfo>
    CCharString first;                                          // +0x00
    CRegisteredTCInfo second;                                   // +0x04
    SRegisteredTCPair(const SRegisteredTCPair& other);          // 0x004D788D
    SRegisteredTCPair& operator=(const SRegisteredTCPair& other);   // 0x004DA56A
};

struct CKeyPairCompareLess {                                    // the empty comparator, passed by value
    CKeyPairCompareLess() {}
};

void __fastcall __adjust_heap(SRegisteredTCPair* first, long holeIndex, long len,
                              SRegisteredTCPair value, CKeyPairCompareLess comp);   // 0x004DFDBB

void __fastcall __pop_heap(SRegisteredTCPair* first, SRegisteredTCPair* last, SRegisteredTCPair* result,
                           SRegisteredTCPair value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, last - first, value, comp);
}