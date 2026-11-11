#ifndef FABLE_ENGINE_C3DANIMATIONBUILDER_H
#define FABLE_ENGINE_C3DANIMATIONBUILDER_H
// New header (chunk 2005, row 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation). C3DAnimationBuilder: Ego_r.pdb vptr +0 (virtual
// ??1C3DAnimationBuilder@@UAE@XZ; ??_G at 0x00AB3550), AddedBonePrefixes CArray<CCharString> +4 (retail 12-byte vector: sizeof 0x10).
// Nested C3DAnimationBuilder::CBoneInfo (Ego_r 0x28, retail 0x24 with the 12-byte CArray; the landed same-TU 0x00AB1F30 model):
// Name +0, Index +4, OriginalNoChildren +8, Children +0xC, AnimSource +0x18, ExtensionChild +0x20, ExtensionParent +0x21.
// Methods: the Ego_r.pdb spellings (private AAE / public QAE); retail addresses in the comments.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class C3DMesh2;
class C3DAnimationSequence;
class CSimpleMemoryPool;
class C3DAnimFileSequenceChunk;
namespace N3DAnimationBuilder { class COutputAnimation; }
#ifdef FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
// Retail opt-in (chunk 2014, row 0x00A50510): the ctor 0x00AB1660 calls ??0CBaseClassNonCopyable 0x0099A2F0 before its own vtable store
// (0x0129E9C8), and InitialiseAnimationSequences expands the destructor of its local builder as the AddedBonePrefixes vector's
// destruction (~CCharString loop, free) then `call ??1CBaseClassNonCopyable 0x0099A300`, no vtable store (0x00A50B95..0x00A50BD0):
// the class derives from CBaseClassNonCopyable and its destructor is the compiler-generated one (FableWin emits
// ??1C3DAnimationBuilder@@UAE@XZ 0x03226670 right after InitialiseAnimationSequences).
#include "engine/CBaseClassNonCopyable.h"
class C3DAnimationBuilder : public CBaseClassNonCopyable {
public:
#else
class C3DAnimationBuilder {
public:
#endif
    class CBoneInfo {
    public:
        CBoneInfo() : ExtensionChild(false), ExtensionParent(false) { }   // ??0CBoneInfo@C3DAnimationBuilder@@QAE@XZ
        ~CBoneInfo();                                                       // 0x00AB34C0 ??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ
        long FindChildIndex(const CCharString& name) const;                 // 0x00AB15F0
        CCharString                         Name;                   // +0x00
        unsigned long                       Index;                  // +0x04
        unsigned long                       OriginalNoChildren;     // +0x08
        CArray<CCountedPointer<CBoneInfo> > Children;               // +0x0c
        CCountedPointer<CBoneInfo>          AnimSource;             // +0x18
        bool                                ExtensionChild;         // +0x20
        bool                                ExtensionParent;        // +0x21
    };
    C3DAnimationBuilder();                                                  // 0x00AB1660 ??0C3DAnimationBuilder@@QAE@XZ
#ifndef FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
    virtual ~C3DAnimationBuilder();                                         // ??1C3DAnimationBuilder@@UAE@XZ (??_G 0x00AB3550)
#endif
    unsigned long BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation& output, const C3DAnimationSequence* sequences,
                                     unsigned long sequence_count, const C3DMesh2& mesh, bool identity_generated_bones,
                                     CSimpleMemoryPool& memory_pool);       // 0x00AB2520
    unsigned long GetIntersection(CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& a,
                                  CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& b);   // 0x00AB2160
private:
    unsigned long AssignAnimation(CBoneInfo& bone, const CCountedPointer<CBoneInfo>& source, bool root);   // 0x00AB1740
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& sequences,
                                                 CArray<CCountedPointer<CBoneInfo> >& bones);       // 0x00AB1F30
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const C3DMesh2& mesh, CArray<CCountedPointer<CBoneInfo> >& bones);   // 0x00AB1CC0
    CCountedPointer<CBoneInfo> BuildBoneInfoTree(const C3DAnimationSequence* sequences, unsigned long sequence_count); // 0x00AB1990
    CArray<CCharString> AddedBonePrefixes;                                  // +0x04
};
#endif
