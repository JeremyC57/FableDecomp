// CTCCameraPointBuilding::IsLeafCriteriaMet() -- a building camera point applies when the observed thing stands in this point's building and the camera tools see the hero from the thing's focal point (entry/exit points never apply)
// Leaf: IsLeafCriteriaMet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DAA0, length 229 bytes (+ int3 padding).
// Callees: CTCBase::PeekWorld() const @ 0x00686A80, CGameCameraManager::GetCameraManager(CCameraManager**) (virtual slot 10, +0x28;
// world +0x34), CTCCameraPointBuilding::GetOwnerBuilding @ 0x0075D940, NCameraModes::CBase::GetObservedThing() const @ 0x0084F1B0 (the
// camera manager's current mode, +0x98), CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0, CThing::GetPosition
// @ 0x004C73D0, GetPBuildingThingOccupying(CThing*) @ 0x006989D0 (__fastcall), CTools::IsLineOfSightFiltered @ 0x00877C40 (the
// camera manager's tools at +0x78; 8 arguments).
// Layout = Ego_r.pdb CTCCameraPointBuilding: InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42.
// Shape: nested tests sharing one false exit; the observed thing missing clears InMyBuilding; the hero position is copied into
// a local; the owner-building out pointer and the cleared line-of-sight hit-thing out are two locals that share one frame
// word. That sharing needs the same-TU GetOwnerBuilding body (0x0075D940 stand-in: writes the out pointer only on success,
// the lookup itself behind an extern), which tells VC7.1 the owner's address does not escape; with only a declaration the
// hit-thing gets its own word, and one shared variable swaps the camera/owner words.
class C3DVector { public: float X, Y, Z; };
class CThing;
class CThingBuilding;
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float height) const;   // 0x004C12D0
};
class CThing {
public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
const CThing* __fastcall GetPBuildingThingOccupying(CThing* thing);     // 0x006989D0 (the CThingBuilding)
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector* camera, const C3DVector* pos, C3DVector* hitPos, C3DVector* hitNormal, long* hitIndex,
                               const CThing*& hitThing, long filter, bool flag);    // 0x00877C40
};
namespace NCameraModes {
class CBase { public:
    CThing* GetObservedThing() const;           // 0x0084F1B0
};
}
struct CCameraManager {
    unsigned char _pad00[0x78];
    CTools Tools;                               // +0x78
    unsigned char _pad7c[0x98 - 0x7c];
    NCameraModes::CBase* PCurrentMode;          // +0x98
};
class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetCameraManager(CCameraManager** out);   // slot 10 (+0x28) 0x0069E1D0
};
class CWorld { public:
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
};
class CTCBase
{
public:
    virtual void v00();
    virtual bool IsLeafCriteriaMet();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCCameraPointDefinitionBase : public CTCBase
{
public:
};

class CTCCameraPointBuilding : public CTCCameraPointDefinitionBase {
public:
    virtual bool IsLeafCriteriaMet();

public:
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x40 - 0x08];
    bool    InMyBuilding;                       // +0x40
    bool    ExitPoint;                          // +0x41
    bool    StartPoint;                         // +0x42
    bool GetOwnerBuilding(const CThing** building);     // 0x0075D940
};

const CThing* FindOwnerBuilding_0075d940(const CTCCameraPointBuilding* point);   // stand-in callee of the body below
#pragma auto_inline(off)
__declspec(noinline) bool CTCCameraPointBuilding::GetOwnerBuilding(const CThing** building)
{
    const CThing* owner = FindOwnerBuilding_0075d940(this);
    if (!owner)
        return false;
    *building = owner;
    return true;
}
#pragma auto_inline(on)
bool CTCCameraPointBuilding::IsLeafCriteriaMet()
{
    const CThing* thing;
    CCameraManager* camera;
    if (PeekWorld().GetCameraManager()->GetCameraManager(&camera) && GetOwnerBuilding(&thing)) {
        CThing* observed = camera->PCurrentMode->GetObservedThing();
        if (observed) {
            C3DVector focal = observed->PGraphicAppearance->GetCameraFocalPos(1.0f);
            C3DVector heroPosition = ParentThing->GetPosition();
            if (thing == GetPBuildingThingOccupying(observed)) {
                const CThing* hitThing = 0;
                if (!ExitPoint && !StartPoint) {
                    long hitIndex;
                    C3DVector hitNormal;
                    C3DVector hitPos;
                    if (camera->Tools.IsLineOfSightFiltered(&focal, &heroPosition, &hitPos, &hitNormal, &hitIndex, hitThing, 7, true))
                        return true;
                }
            }
        } else {
            InMyBuilding = false;
        }
    }
    return false;
}