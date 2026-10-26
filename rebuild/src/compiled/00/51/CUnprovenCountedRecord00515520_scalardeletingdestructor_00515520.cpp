// CUnprovenCountedRecord_00515520::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor of a record whose only non-trivial member is the CCountedPointer at +4: release it (destroy the pointee and free the count block when the count reaches zero), clear both words, and free the record when bit 0 of the flags is set
// Leaf: ??_GCUnprovenCountedRecord_00515520@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00515520, 74 bytes (+ int3 padding to 80), thiscall `ret 4`, returns this.
// Identity (descriptive): the deleting destructor of a non-polymorphic 12-byte record {4 trivially destructible bytes,
//   CCountedPointer<T> +4}; no call, jump or data reference reaches this address (VC7.1 emits ??_G for every explicit
//   `p->~T()` -- e.g. STLport `_Destroy(&node->_M_value_field)` / `__destroy_aux` over pair<K8, CCountedPointer<T> >
//   values -- and for every `delete p`, inlines it at the site, and the retail link kept the COMDAT), so neither the
//   record nor T is provable: both are named CUnproven..._00515520. FableWin.pdb has ??_G for some twenty pair<4-byte key, CCountedPointer<T> > types of this shape (CFaction, CRegionConnectionNode, CContainmentVolume, CNavConnectorNode, NParticleEngine::CParticleEmitter, CVertexBufferWin32, CTexture, ...); Ego_r.pdb folds them as the manifest's pair<unsigned long, CCountedPointer<CFaction> >.
//   Neighbours: the CThingBuilding sort / CMapInfo COMDATs (0x005154C0, 0x00515570).
// Label: the manifest's "_Dest_val<std::allocator<std::pair<unsigned_long,CCountedPointer<CFaction>_>_>,std::pair<unsigned_long,CCountedPointer<CFaction>_>_>" is propagated (family H067: six bodies,
//   the counted pointer at +4 or +8; retail has no identical-code folding, so each address is its own class's COMDAT).
// Callees: the count block's deleter (`call [info+4]` with the object in ecx), operator delete 0x00BFE9BC (the count
//   block, then the record).
void operator delete(void* block);                      // 0x00BFE9BC

struct CCPPointerInfo {                                 // the shared count block
    long RefCount;                                      // +0x0
    void (__fastcall *DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};

template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CUnprovenPointee_00515520;                                   // descriptive: the counted pointee
class CUnprovenCountedRecord_00515520 {                             // descriptive: 12 bytes
public:
    unsigned long Head[1];                              // +0x0: 4 trivially destructible bytes (the key)
    CCountedPointer<CUnprovenPointee_00515520> Pointer;     // +0x4
};

// The explicit destructor call (an STLport _Destroy of the record) that emits the COMDAT.
void DestroyRecord_00515520(CUnprovenCountedRecord_00515520* p) { p->~CUnprovenCountedRecord_00515520(); }