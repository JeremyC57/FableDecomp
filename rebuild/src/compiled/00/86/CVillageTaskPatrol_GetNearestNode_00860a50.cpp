// CVillageTaskPatrol::GetNearestNode(const C3DVector& position) -- walk the patrol track from PatrolStart (following NextNode until the end node) and return the node nearest to `position` (0 without a track)
// Leaf: ?GetNearestNode@CVillageTaskPatrol@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860A50, length 125 bytes (padded to 128), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PatrolStart +0x08), CThing::GetPos @ 0x004C73D0.
// Layout: engine/CVillageTaskPatrol.h (PatrolStart CIntelligentPointer<const CThingTrackNode> +0x08); CThingTrackNode retail
// NextNode +0xd0 and IsEnd +0xed (Ego_r.pdb +0xe8 / +0x105: the thing base is 0x18 bytes smaller in retail).
// Shape: the best distance starts at FLT_MAX (0x7F7FFFFF) in the frame; `dist < best` is `fcom ; test ah,5 ; jp`; the
// squared length is a `dist = dz*dz; dist += dx*dx; dist += dy*dy;` chain (a single sum expression is reordered by VC7.1).
class C3DVector { public:
    float X, Y, Z;
};
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
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
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