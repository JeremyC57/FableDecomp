// DestructArray(CAnimatedBlock* blocks, unsigned long count) -- run the destructor of each of a primitive's `count` animated blocks in place (the block memory is not freed)
// Leaf: DestructArray
// Retail: Fable.exe @ 0x00A96810, 47 bytes, plain `ret`: __fastcall (blocks ecx, count edx). Only call site
// 0x00A9024D in C3DPrimitive2::FreeData (landed 0x00A901E0, which declares it `DestroyAnimatedBlocks`), passing
// AnimatedBlocks (+0x4C) and AnimatedBlockCount (+0x48); FreeData then just nulls the pointer.
// The catalog's `_Destroy_range<allocator<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > > >` is
// propagated: the body steps 0x30 bytes (sizeof CAnimatedBlock) and takes a count, not a last pointer.
// Types (Ego_r.pdb): CAnimatedBlock (48 bytes): BoneConstantsFixupOffset +0xC, VertexCount +0x10, BonesPerVertex +0x14,
//   PalettedFlag +0x16, GroupCount +0x17, Groups[18] +0x18, Data CIVCountedPointer<N3DPrimitive::CAnimatedBlock::
//   CDeviceIndependent> +0x2C (the first 12 bytes are unnamed in the PDB).
// Callees: the pointee's virtual destroy (vtable slot 1, `call [eax+4]`) once its reference count (+4) hits zero.
// Shape: the null-pointer guard then the `0 < count` entry test (unsigned: `test edx,edx ; jbe`); the element
// destructor is the implicit ~CAnimatedBlock = the CIVCountedPointer release (local copy, `dec [ecx+4] ; jne`, the
// member zeroed only when it was set); VC7.1 rewrites the index loop into a `dec edi ; jne` countdown over the Data
// member's address (`lea esi,[ecx+2Ch]`).
// Local base model, not engine/CIVCountedPointeeBase.h: the generated header flattens the vptr into a plain
// `void* __vftable`, which cannot make the slot-1 virtual destroy call the release runs.
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
    T* Object;                                  // +0x00
    __forceinline ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->m_count == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

namespace N3DPrimitive {
struct CAnimatedBlockDeviceIndependent : CIVCountedPointeeBase { };   // N3DPrimitive::CAnimatedBlock::CDeviceIndependent
}

struct CAnimatedBlock {                         // 48 bytes
    unsigned long  _unnamed00[3];               // +0x00 (unnamed in the PDB)
    unsigned long  BoneConstantsFixupOffset;    // +0x0C
    unsigned long  VertexCount;                 // +0x10
    unsigned short BonesPerVertex;              // +0x14
    bool           PalettedFlag;                // +0x16
    unsigned char  GroupCount;                  // +0x17
    unsigned char  Groups[18];                  // +0x18
    CIVCountedPointer<N3DPrimitive::CAnimatedBlockDeviceIndependent> Data;   // +0x2C
};

template <class T>
void __fastcall DestructArray(T* p, unsigned long n)
{
    if (p) {
        for (unsigned long i = 0; i < n; ++i)
            p[i].~T();
    }
}
template void __fastcall DestructArray(CAnimatedBlock*, unsigned long);