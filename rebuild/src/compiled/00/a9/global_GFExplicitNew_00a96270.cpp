// GFExplicitNew(N3DPrimitive::CAnimatedBlock* blocks, unsigned long count) -- construct count animated blocks in place in caller-provided storage
// Leaf: ??$GFExplicitNew@VCAnimatedBlock@N3DPrimitive@@@@YIXPAVCAnimatedBlock@N3DPrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A96270, 48 bytes, __fastcall (blocks in ecx, count in edx), plain `ret`.
// Identity: Ego_r.pdb `??$GFExplicitNew@VCAnimatedBlock@N3DPrimitive@@@@YIXPAVCAnimatedBlock@N3DPrimitive@@K@Z`; the
//   C3DPrimitive2 loader calls it at 0x00A91BD8 on AnimatedBlocks (+0x4C, placed after the StaticBlockCount 32-byte
//   static blocks) with AnimatedBlockCount (+0x48), right after the CStaticBlock instantiation 0x00A94150.
// Layout: Ego_r.pdb N3DPrimitive::CAnimatedBlock (48 bytes): CBaseBlock base {PrimitiveCount +0, StartIndex +4, IsStrip +8,
//   DegenerateTriangles +9, ChangeFlags +0xA}, BoneConstantsFixupOffset +0xC .. Groups +0x18, Data +0x2C
//   (CIVCountedPointer<CAnimatedBlock::CDeviceIndependent>). The inline constructors zero the base fields and Data only.
// Shape: placement new per element (the null test of the slot), the count register counted down.
inline void* operator new(unsigned int, void* __p) { return __p; }

template <class T> class CIVCountedPointer {            // retail 4 bytes
public:
    T* Object;                                          // +0x00
    CIVCountedPointer() : Object(0) { }
};

namespace N3DPrimitive {

class CBaseBlock {
public:
    unsigned long PrimitiveCount;                       // +0x00
    unsigned long StartIndex;                           // +0x04
    bool          IsStrip;                              // +0x08
    bool          DegenerateTriangles;                  // +0x09
    unsigned char ChangeFlags;                          // +0x0A
    CBaseBlock() : PrimitiveCount(0), StartIndex(0), IsStrip(false), DegenerateTriangles(false), ChangeFlags(0) { }
};

class CAnimatedBlock : public CBaseBlock {
public:
    class CDeviceIndependent;
    unsigned long  BoneConstantsFixupOffset;            // +0x0C
    unsigned long  VertexCount;                         // +0x10
    unsigned short BonesPerVertex;                      // +0x14
    bool           PalettedFlag;                        // +0x16
    unsigned char  GroupCount;                          // +0x17
    unsigned char  Groups[18];                          // +0x18
    CIVCountedPointer<CDeviceIndependent> Data;         // +0x2C
    CAnimatedBlock() { }
};

}   // namespace N3DPrimitive

template <class T> void __fastcall GFExplicitNew(T* blocks, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&blocks[i]) T;
}

template void __fastcall GFExplicitNew<N3DPrimitive::CAnimatedBlock>(N3DPrimitive::CAnimatedBlock* blocks, unsigned long count);