// CVillageTaskPatrol::GetNearestNode(const C3DVector& position) -- walk the patrol track from PatrolStart (following NextNode until the end node) and return the node nearest to `position` (0 without a track)
// Leaf: ?GetNearestNode@CVillageTaskPatrol@@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860A50, length 125 bytes (padded to 128), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PatrolStart +0x08), CThing::GetPos @ 0x004C73D0.
// Layout: engine/CVillageTaskPatrol.h (PatrolStart CIntelligentPointer<const CThingTrackNode> +0x08); CThingTrackNode retail
// NextNode +0xd0 and IsEnd +0xed (Ego_r.pdb +0xe8 / +0x105: the thing base is 0x18 bytes smaller in retail).
// Shape: the best distance starts at FLT_MAX (0x7F7FFFFF) in the frame; `dist < best` is `fcom ; test ah,5 ; jp`; the
// squared length is a `dist = dz*dz; dist += dx*dx; dist += dy*dy;` chain (a single sum expression is reordered by VC7.1).
#include "engine/C3DVector.h"
class CThing { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
class CThingTrackNode : public CThing { public:
    unsigned char    _pad[0xd0];
    CThingTrackNode* NextNode;                     // +0xd0 (retail)
    unsigned char    _pad2[0xed - 0xd4];
    bool             IsEnd;                        // +0xed (retail)
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CVillageTaskPatrol { public:
    virtual void v00();
    unsigned char           _pad04[4];             // +0x04 (CVillageTaskBase)
    CIntelligentPointer<CThing> PatrolStart;           // +0x08 CIntelligentPointer<const CThingTrackNode>
    CIntelligentPointer<CThing> CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool                    TaskLocked;            // +0x18
    const CThingTrackNode* GetNearestNode(const C3DVector& position);
};
const CThingTrackNode* CVillageTaskPatrol::GetNearestNode(const C3DVector& position)
{
    const CThingTrackNode* nearest = 0;
    const CThingTrackNode* node = (const CThingTrackNode*)PatrolStart.GetPItem();
    if (node) {
        float best = 3.40282347e+38f;
        do {
            const C3DVector& p = node->GetPos();
            float dx = p.X - position.X;
            float dy = p.Y - position.Y;
            float dz = p.Z - position.Z;
            float dist = dz * dz;
            dist += dx * dx;
            dist += dy * dy;
            if (dist < best) {
                best = dist;
                nearest = node;
            }
            node = node->NextNode;
        } while (node && !node->IsEnd);
    }
    return nearest;
}