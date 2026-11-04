// CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain) -- queue a sub-primitive for sorted rendering: alpha ones (flag 1) on list 1 keyed by the reversed distance, opaque ones on list 0 / 11 / 12 keyed by the distance; a sub-primitive whose chain already has a head is linked behind it instead
// Leaf: ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z
// Flags: none
// Retail-asm: 0x00B845A6 / 0x00B84648 `fistp dword ptr [esp+28h]` -- the scaled distance stored back to its argument slot, reloaded and
//   stored with a bare dword fistp: the engine's inline-assembly float-to-long GFFloatToLongNear (shared engine/GFFloatToLongNear.h).
//   VC7.1 emits no bare dword fistp from C++ (NOTES.md "C++ attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84580, thiscall `ret 18h`.
// Identity: Ego_r.pdb ?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z
//   (the reloc binding's ...PAX0000PAUSSortKey... spelling is wrong, NAME_FIXES.tsv).
// Callees: STLport vector<CEngineSubPrimitiveRenderer::CSortElement>::_M_insert_overflow (__false_type) 0x00B85EC0 (push_back).
// Layout: the 15 sub-primitive lists at +0x18, 0x18 bytes each (CArray<CSortElement> + render passes + flags, the landed
//   RenderType 0x00B849F0 model); MaxSortDistance +0x10, SortDistanceMultiplier +0x14.
// Shape: opaque sub-primitives go on list 0 / 11 / 12 (ESubPrimitiveType), alpha ones on list 1. Grade: RELOCATION_MATCH (try b).
#include "engine/GFFloatToLongNear.h"
#include "engine/CArray.h"
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase { public:
    void*                    _vptr;
    unsigned long            RenderFlags;           // +0x04
    unsigned char            Alpha;                 // +0x08
    void*                    Parent;                // +0x0c
    CEngineSubPrimitiveBase* NextSubPrimitive;      // +0x10
    CEnginePrimitiveManager* Manager;               // +0x14
};
class CEngineSubPrimitiveChain { public:
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
};
class CEngineSubPrimitiveRenderer {
public:
    class CSortElement { public:
        unsigned long            SortValue;
        CEngineSubPrimitiveBase* Primitive;
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* primitive) : SortValue(sortValue), Primitive(primitive) { }
    };
    class CSubPrimitiveList { public:
        CArray<CSortElement> List;
        long RenderPass, PreRenderPass;
        bool BoundingVolumePass, Stable, Sorted, Enable;
    };
    void*     _vptr;
    unsigned char _pad04[0x0c];
    float     MaxSortDistance;                      // +0x10
    float     SortDistanceMultiplier;               // +0x14
    CSubPrimitiveList SubPrimitiveLists[15];        // +0x18
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha, unsigned long flags,
                         CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain);
};
static inline long ClampKey(long key) { return key < 0 ? 0 : (key > 0x3fffffff ? 0x3fffffff : key); }
void CEngineSubPrimitiveRenderer::AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char alpha,
                                                  unsigned long flags, CEnginePrimitiveManager* manager, CEngineSubPrimitiveChain& chain)
{
    if (flags & 1) {
        distance = (MaxSortDistance - distance) * SortDistanceMultiplier;
        long key = ClampKey(GFFloatToLongNear(distance));
        if (chain.LastAlpha) {
            chain.LastAlpha->NextSubPrimitive = subPrimitive;
            chain.LastAlpha = subPrimitive;
        }
        else {
            SubPrimitiveLists[1].List.push_back(CSortElement(key, subPrimitive));
            chain.LastAlpha = subPrimitive;
        }
    }
    else {
        long list = 0;
        if (flags & 0x80)
            list = 11;
        if (flags & 0x100)
            list = 12;
        distance = distance * SortDistanceMultiplier;
        long key = ClampKey(GFFloatToLongNear(distance));
        if (chain.LastOpaque)
            chain.LastOpaque->NextSubPrimitive = subPrimitive;
        else
            SubPrimitiveLists[list].List.push_back(CSortElement(key, subPrimitive));
        chain.LastOpaque = subPrimitive;
    }
    subPrimitive->RenderFlags = flags;
    subPrimitive->Alpha = alpha;
    subPrimitive->Manager = manager;
}