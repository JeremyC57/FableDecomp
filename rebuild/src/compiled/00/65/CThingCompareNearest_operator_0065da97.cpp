// CThingCompare_Nearest::operator()<CThingAICreature>(CThingAICreature* a, CThingAICreature* b) const -- the nearer of two things to the comparator's position (a null side loses)
// Leaf: ??R
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DA97, 145 bytes (`ret 8`); size-optimised TU with an ebp frame (`leave`).
// Callees: CThing::GetPos @ 0x004C73D0 (label "CVertexBufferWin32::DoSizeof" propagated).
// Layout: CThingCompare_Nearest (Ego_r.pdb 12 bytes) = C3DVector Pos.
// Shape: operator- and GetLengthSquared `__forceinline` (the size TU outlines them otherwise); a null -> b, b null -> a; both offsets from Pos (the first one spilled to the frame across the second
//   GetPos), then `distA < distB ? a : b` (A summed first) (`fcompp ; test ah,41h ; je`).
#pragma optimize("s", on)
#pragma optimize("y", off)   // ebp frame
#include "engine/C3DVector.h"
class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingAICreature : public CThing { public: };

class CThingCompare_Nearest { public:
    C3DVector Pos;                              // +0x00
    template<class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        C3DVector offsetA = a->GetPos() - Pos;
        C3DVector offsetB = b->GetPos() - Pos;
        if (offsetA.GetSquaredMagnitude() < offsetB.GetSquaredMagnitude())
            return a;
        return b;
    }
};

template CThingAICreature* CThingCompare_Nearest::operator()(CThingAICreature* a, CThingAICreature* b) const;