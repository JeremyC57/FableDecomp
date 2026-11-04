// CEngineSubPrimitiveRenderer::GetCount(ESubPrimitiveType type) const -- number of sort elements queued in the list of one sub-primitive type
// Leaf: ?GetCount@CEngineSubPrimitiveRenderer@@QBEKW4ESubPrimitiveType@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B842B0, 25 bytes (+ int3 padding), __fastcall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb `?GetCount@CEngineSubPrimitiveRenderer@@QBEKW4ESubPrimitiveType@@@Z`; the manifest
//   label "CEnginePrimitiveRenderer_GetIndexCount" is descriptive and names the wrong class. The retail
//   CSubPrimitiveList is 24 bytes (STLport 12-byte CArray/vector first, then the pass settings and flags; Ego_r has 28
//   bytes with a 16-byte Dinkumware vector), and SubPrimitiveLists starts at +0x18, so list[type] sits at
//   this + (3 * type + 3) * 8 (`lea eax,[eax+eax*2+3]`), the landed RenderType 0x00B849F0 uses the same layout.
// Shape: CArray::size() = vector::size() = (_M_finish - _M_start) / 8 (`sar eax,3`) through an inline accessor.
//   The generated engine header carries the Ego_r layout (CArray 16 bytes, lists 28 bytes), so the retail list is
//   modelled locally.
class CEngineSubPrimitiveBase;

enum ESubPrimitiveType { ESUBPRIMITIVE_TYPE_FIRST = 0 };

#include "engine/CArray.h"

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
    };
    class CSubPrimitiveList {                           // retail 24 bytes
    public:
        CArray<CSortElement> Elements;                  // +0x00
        long RenderPass;                                // +0x0C ERenderPassType
        long PreRenderPass;                             // +0x10 ERenderPassType
        bool BoundingVolumePass;                        // +0x14
        bool RequiresStableSort;                        // +0x15
        bool Sorted;                                    // +0x16
        bool Enable;                                    // +0x17
    };
    void* __vftable;                                    // +0x00
    unsigned char _pad_0x04[0x0C];                      // +0x04
    float MaxSortDistance;                              // +0x10
    float SortDistanceMultiplier;                       // +0x14
    CSubPrimitiveList SubPrimitiveLists[15];            // +0x18
    unsigned long GetCount(ESubPrimitiveType type) const;
};

unsigned long CEngineSubPrimitiveRenderer::GetCount(ESubPrimitiveType type) const
{
    return SubPrimitiveLists[type].Elements.size();
}