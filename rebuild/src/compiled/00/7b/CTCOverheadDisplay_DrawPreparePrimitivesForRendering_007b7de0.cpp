// CTCOverheadDisplay::DrawPreparePrimitivesForRendering(long) -- clears PrimitiveHandles (vector<CEnginePrimitiveHandle> at +0x1c: erase of the whole [first, last) range); the argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B7DE0, length 19 bytes.
// Callees: vector<T>::erase(first, last) @ 0x00440D70 (Vector_EraseRange, a __fastcall taking two element pointers).
// Shape: the inlined clear() pushes m_pLast then m_pFirst (right to left) and forms `this` of the vector with
// `add ecx,1Ch`; the erase result is dropped and the `ret 4` of the unused argument blocks a tail jump.
// NF48 (round 7): the CRetailHandleVector stand-in is STLport 4.6.2 std::vector<CEnginePrimitiveHandle> from the shared stlport/fable_containers.h;
// FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE keeps vector<CEnginePrimitiveHandle>::erase(iterator, iterator) 0x00440D70 out of line as retail calls it.
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
class CEnginePrimitiveHandle;


struct CTCBase_007b7de0 {
    virtual void v00();
    unsigned char _pad04[8];                        // +0x04 ParentThing, +0x08 flags
};

class CTCOverheadDisplay : public CTCBase_007b7de0 { public:
    unsigned char       _pad0c[0x1c - 0x0c];       // DamageValues/RepairValues/RenownValues (4-byte retail lists), PDef
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;                   // +0x1c
    virtual void DrawPreparePrimitivesForRendering(long unused);
};

void CTCOverheadDisplay::DrawPreparePrimitivesForRendering(long)
{
    PrimitiveHandles.clear();
}