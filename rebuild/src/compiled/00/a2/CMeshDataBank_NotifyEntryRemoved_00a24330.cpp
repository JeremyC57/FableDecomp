// CMeshDataBank::NotifyEntryRemoved(unsigned long) -- an entry left the bank: drop its dynamic-info counted pointer (when in range) and clear its slot in the entry table
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24330, 128 bytes (padding included); vtable 0x0129CE94 slot 14.
// Leaf: ?NotifyEntryRemoved@CMeshDataBank@@MAEXK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Shape: `index < DynamicInfoBuffer.size()` (+0x3b0, 8-byte CCountedPointer<CMeshDataBankEntry>
// elements, unsigned) selects the element through STLport's `*(begin() + n)` and assigns null to it:
// inline release (count re-read after the decrement; deleter +4 on the object +8, block freed by
// operator delete 0x00BFE9BC) then block and object zeroed. Either way MeshesAndAnimations[index]
// (+0x380) = 0; VC duplicates that store into both arms. Retail CArray is 12 bytes (PDB 16), so the
// arrays sit 0x74 + 4n below the PDB offsets.
class CMeshDataBankEntry;

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template <class T> class CCountedPointer { public:
    T*           Data;                              // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer& operator=(T* object)           // used with null
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        Data = object;
        return *this;
    }
};

#include "engine/CArray.h"

class CMeshDataBank { public:
    virtual void v00();
    unsigned char _pad004[0x380 - 4];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;                 // +0x380
    unsigned char _pad38c[0x3b0 - 0x38c];                             // MeshInfoBuffer, PhysicsMeshInfoBuffer, AnimationInfoBuffer
    CArray<CCountedPointer<CMeshDataBankEntry> > DynamicInfoBuffer;   // +0x3b0
protected:
    virtual void NotifyEntryRemoved(unsigned long index);   // protected virtual (PDB symbol ...@@MAEXK@Z)
};

void CMeshDataBank::NotifyEntryRemoved(unsigned long index)
{
    if (index < DynamicInfoBuffer.size())
        DynamicInfoBuffer[index] = 0;
    MeshesAndAnimations[index] = 0;
}