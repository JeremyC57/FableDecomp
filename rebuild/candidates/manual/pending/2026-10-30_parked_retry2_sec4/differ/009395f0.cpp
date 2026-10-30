// CAIStateGroup_SitRoundCampfire::GetPNearCampfire() const -- nearest campfire with a free seat within 22 units of the creature
// Grade: DIFFER(122v122) -- retry wave 2: store scheduling only (retail loads all three position dwords before the first store and stores the null result after the first creature load); PDB model now (private const leaf, CIsNearCampfireWithSpace, GetPos, const PeekThingSearchTools, template GetPBestLeafThing); tried late campfire init. Next: none obvious (scheduling)
// Leaf: ?GetPNearCampfire@CAIStateGroup_SitRoundCampfire@@ABEPAVCThingObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009395F0, length 122 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb ?GetPNearCampfire@CAIStateGroup_SitRoundCampfire@@ABEPAVCThingObject@@XZ (private const).
// Callees: CThing::GetPos() const @ 0x004C73D0 (twice, on PBrain->Creature), CAIStateGroupBase::PeekThingSearchTools() const
//   @ 0x008FD000, CThingSearchTools::GetPBestLeafThing<CIsNearCampfireWithSpace, CThingCompare_Nearest, CThingObject> @
//   0x00939AF0 (Ego_r.pdb template spelling, const).
// Shape: the comparator (Ego_r CThingCompare_Nearest { C3DVector Pos; }, 12 bytes) and the filter (Ego_r
//   CIsNearCampfireWithSpace { float DistanceSq; C3DVector FromPos; }, 16 bytes, ctor (float, const C3DVector&)) are
//   named locals copied from the creature position with the implicit C3DVector copy (three loads, then the stores);
//   484.0f = 22 squared.
// Layout: local AI state-group model (the engine headers for CAIStateGroupBase / CAIStateGroup_SitRoundCampfire are
//   flat Ego_r layouts without the vptr chain); engine/CAIBrain.h gives Creature +0x18.
#include "engine/CAIBrain.h"
class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    const C3DVector& GetPos() const;            // 0x004C73D0
};
class CThingAICreature : public CThing { };
class CThingObject;

class CThingCompare_Nearest {
public:
    C3DVector Pos;                              // +0x00
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};

class CIsNearCampfireWithSpace {
public:
    float     DistanceSq;                       // +0x00
    C3DVector FromPos;                          // +0x04
    CIsNearCampfireWithSpace(float distanceSq, const C3DVector& fromPos) : FromPos(fromPos) { DistanceSq = distanceSq; }
};

class CThingSearchTools {
public:
    template <class TFilter, class TCompare, class T>
    bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, T** result) const;   // 0x00939AF0
};

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain*     PBrain;                       // +0x04
    long          ScriptAIPriority;             // +0x08
    void*         Name_PStringData;             // +0x0c
    bool          Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    bool          Enabled;                      // +0x12
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
};

class CAIStateGroup_SitRoundCampfire : public CAIStateGroupBase {
private:
    CThingObject* GetPNearCampfire() const;
};

CThingObject* CAIStateGroup_SitRoundCampfire::GetPNearCampfire() const
{
    CThingObject* campfire = 0;
    CThingCompare_Nearest compare(PBrain->Creature->GetPos());
    CIsNearCampfireWithSpace filter(484.0f, PBrain->Creature->GetPos());
    PeekThingSearchTools().GetPBestLeafThing(filter, compare, &campfire);
    return campfire;
}
