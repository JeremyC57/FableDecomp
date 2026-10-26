// CTCContainer::ScalarDeletingDestructor(unsigned int) -- `scalar deleting destructor': destroys the ContainerItems vector (each pair's CCharString, then the storage freed), the CTCBase destructor, then operator delete when bit 0 of the flags is set
// Leaf: ??_GCTCContainer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074FDE0, length 78 bytes (+ int3 padding). Vtable 0x012693AC
// (CTCContainer) slot 0.
// Callees: CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14, CTCBase::~CTCBase @ 0x00686830
// (stores the CTCBase vtable; labelled "~CChunkedFileChunk", propagated), operator delete @ 0x00BFE9BC.
// Layout (retail): CTCBase 0xc, ContainerItems +0xc = STLport vector<pair<long, CCharString> > {start,
// finish, end of storage} (12 bytes; the PDB's is 16), the 8-byte pair's string at +4.
// Shape: no vtable store of its own (see the model note below); STLport `_Destroy(start, finish)` is a rotated loop over the pair strings
// with both bounds read once (esi/edi), `~_Vector_base` frees the storage behind a null test, the base
// destructor is called non-virtually and the deleting tail tests bit 0 of the flags. The destructor is
// `__forceinline` so its body lands in the `??_G`.
#include <stdlib.h>
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CThing;

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                         // 0x0099EAE0
};

struct SContainerItem {                                     // pair<long, CCharString>: 8 bytes
    long        first;
    CCharString second;
};

struct CRetailVector_ContainerItem {                        // STLport vector<pair<long, CCharString> >
    SContainerItem* Start;
    SContainerItem* Finish;
    SContainerItem* EndOfStorage;
    CRetailVector_ContainerItem() : Start(0), Finish(0), EndOfStorage(0) { }
    static __forceinline void Destroy(SContainerItem* first, SContainerItem* last)   // STLport _Destroy
    {
        for (; first != last; ++first)
            first->~SContainerItem();
    }
    __forceinline ~CRetailVector_ContainerItem()
    {
        Destroy(Start, Finish);
        if (Start)
            free(Start);
    }
};

class CTCBase { public:                                            // 0xc bytes; its destructor stores the CTCBase vtable
    virtual void v00();
    CThing*       ParentThing;                              // +0x04
    unsigned char _pad08[4];                                // +0x08
    virtual ~CTCBase();                                             // 0x00686830
};

// The retail destructor stores no vtable of its own: modelled (as GOTCHAS 24m describes for a base chain
// whose destructor is out of line) with the CTCBase part as the FIRST MEMBER of a non-polymorphic class,
// whose `delete` emits the non-virtual scalar deleting destructor `??_GCTCContainer@@QAEPAXI@Z`.
class CTCContainer { public:                                       // vtable 0x012693AC (owned by the CTCBase part)
    CTCBase                     Base;                       // +0x00
    CRetailVector_ContainerItem ContainerItems;             // +0x0c
    __forceinline ~CTCContainer() { }
};

void DestroyContainer(CTCContainer* container)
{
    delete container;
}