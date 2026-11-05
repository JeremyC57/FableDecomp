// CThingCompare_Nearest::operator()<T>(T* a, T* b) const -- (T = CThingDeadCreature) return whichever thing is nearer to the reference position: null a loses to b, null b loses to a, else the smaller squared 3D distance wins (a on a strict less, b on a tie)
// Leaf: ??$?RVCThingDeadCreature@@@CThingCompare_Nearest@@QBEPAVCThingDeadCreature@@PAV1@0@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914130, 177 bytes, thiscall, `ret 8`, the nearer pointer in eax.
// Identity: T = CThingDeadCreature -- proven; evidence: its callers are the landed GetPBestThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature> 0x009142B0 (thing-type test `and eax,87h ; cmp al,87h`) and GetPBestLeafThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature> 0x00936950. The FableWin.pdb name set holds this exact
//   symbol (??$?RVCThingDeadCreature@@@CThingCompare_Nearest@@QBEPAVCThingDeadCreature@@PAV1@0@Z).
// Label: the manifest's "operator()<CThingMarker>" is propagated across byte-identical bodies (the template body does not depend on T).
// Callees: CThing::GetPos() const @ 0x004C73D0 (T derives from CThing at offset 0).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00.
// Shape: as the landed CThingCompare_NearestToPos::operator()<CThing> 0x007A2F90: the first delta is spilled to the
//   12-byte frame across the second GetPos call, the second stays on the x87 stack; both squared lengths are the
//   plain inline X*X + Y*Y + Z*Z (VC7.1 ranks the terms itself); the result is one `?:` on the strict less.
#include "engine/C3DVector.h"

#include "engine/CThing.h"
class CThingDeadCreature : public CThing { };
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                      // +0x00
    template<class T> T* operator()(T* a, T* b) const;
};

template<class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return (deltaA.GetSquaredMagnitude() < deltaB.GetSquaredMagnitude()) ? a : b;
}

template CThingDeadCreature* CThingCompare_Nearest::operator()<CThingDeadCreature >(CThingDeadCreature* a, CThingDeadCreature* b) const;