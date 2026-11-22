// C3DAnimationBuilder::CBoneInfo::FindChildIndex(const CCharString& name) const -- linear search of a skeleton-builder bone's children for
// the child with the given name; returns its index or -1
// Leaf: ?FindChildIndex@CBoneInfo@C3DAnimationBuilder@@QBEJABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB15F0, length 109 bytes (ret 4; caller 0x00AB1942).
// Callee: Compare(const char*, const char*) @ 0x00411570 (__fastcall, 0 when equal) inside the inlined CCharString operator== (same
// data block, or both present with equal lengths and Compare == 0).
// Layout (engine/C3DAnimationBuilder.h, Ego_r.pdb nested CBoneInfo): Name +0x00, Index +0x04, Children +0x0c (a CArray of
// CCountedPointer<CBoneInfo>: first +0x0c, last +0x10 in retail; 8-byte elements {object, count block}).
// Names (agentNF250, 2026-11-22, CODEXP27 L115): was ?FindChildIndex@CBoneInfo@@QAEJABVCCharString@@@Z (a flattened non-const CBoneInfo
//   over row-local CCharString / CArray / CCountedPointer models and a same-TU Compare stand-in). Ego_r.pdb and FableWin.pdb S_PUB32 both
//   spell ?FindChildIndex@CBoneInfo@C3DAnimationBuilder@@QBEJABVCCharString@@@Z (nested in C3DAnimationBuilder, const this); the landed
//   caller 0x00AB1740 (AssignAnimation, retail call 0x00AB1942) already binds it. Now on the shared engine/C3DAnimationBuilder.h with the
//   caller's prelude (its TU-mate: 0x00AB1740 defines the same body as a same-TU strong copy, so the two objects linked together give one
//   LNK2005 on this symbol with identical bytes).
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "engine/C3DAnimationBuilder.h"

long C3DAnimationBuilder::CBoneInfo::FindChildIndex(const CCharString& name) const
{
    for (unsigned long child = 0; child < Children.size(); ++child) {
        if (name == Children[child]->Name)
            return child;
    }
    return -1;
}
