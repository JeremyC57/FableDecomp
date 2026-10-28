// _global::__pop_heap(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result, PairCharStringScriptInfo value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for the script registry's (name, script info) heap: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB6B00, length 171 bytes, `ret 24h` (result, the 0x18-byte value, the
//   comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is pair<CCharString, CScriptInfo> (0x18 bytes, `imul 2AAAAAABh ; sar edx,2`); the callee 0x00CB68C0
//   carries the manifest name `_Adjust_heap<pair<CCharString,CScriptInfo>*,...,CKeyPairCompareLess<CCharString,CScriptInfo>>`
//   and the landed __copy_backward 0x00CB62F0 uses the same element model. The row's `_Pop_heap_0` label is the aux front end's
//   name; this body is the out-of-line core. Callers 0x00CB6CA5, 0x00CB6E15, 0x00CB6F7E, 0x00CB71B8.
// Callees: CCharString::operator= @ 0x0099EFB0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
//   __adjust_heap @ 0x00CB68C0, CCharString::~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CScriptInfo (0x14): AllocFunc +0, MasterScriptDataAlloc +4, CCharString Name +8, long ID +0xc,
//   bool MasterScript +0x10; in the pair it sits at +4.
// Shape: the pair's IMPLICIT assignment is inlined (string operator=, two pointers, the name's operator=, ID, the bool through
//   cl), the by-value copy for __adjust_heap is built in the argument area by STLport's explicit pair copy constructor and
//   CScriptInfo's implicit one (two string copy constructors), and the by-value `value` parameter is destroyed by this callee
//   at the end (second.Name, then first).
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

class CScriptInfo { public:                                    // Ego_r.pdb, 0x14 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

struct PairCharStringScriptInfo {                       // std::pair<CCharString, CScriptInfo>, 0x18 bytes
    CCharString first;                                  // +0x00
    CScriptInfo second;                                 // +0x04
    PairCharStringScriptInfo(const PairCharStringScriptInfo& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<CCharString, CScriptInfo>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairCharStringScriptInfo* first, long holeIndex, long len, PairCharStringScriptInfo value,
                              CKeyPairCompareLess comp);   // 0x00CB68C0

void __fastcall __pop_heap(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result,
                           PairCharStringScriptInfo value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}