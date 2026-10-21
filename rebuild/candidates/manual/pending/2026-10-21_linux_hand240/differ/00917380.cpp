// CAIStateGroup_IdleAtHome::Update() -- with a home building: walk to a random point 0..4.5m from the home's position (random heading), retrying while the move reports "in progress", idle on arrival; terminate
// Grade: DIFFER(ours v retail) -- home-position copy uses ecx/edx/eax with all loads first vs retail's two-register interleave (edx,ecx), and the second creature load goes through eax vs ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917380, length 336 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's PHome at +0x168), CThing::GetPosition
//          @ 0x004C73D0, CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFFloatRandom(float, unsigned long*) @ 0x004978A0
//          (__fastcall: the seed in ecx), C3DVector::RotateAboutZ @ 0x00A14260 (adds the angle to the XY heading,
//          manifest "C3DVector_FromSpherical"), CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (C3DVector
//          by value), AIAction_MinionIdle @ 0x0097FA00, Terminate @ 0x008FCF40.
// Globals: 0.0001f @ 0x0129BA3C.
// Shape: the home position is a user-copy-ctor copy, the offset a (r + 0.0001, 0, 0) vector rotated in place, the
//          target `home + offset` built by a three-float ctor (Z first); a do/while re-tests the home before each try.
// Layout: engine/CAIStateGroup_IdleAtHome.h (PBrain is the first dword of the flattened CAIStateGroupBase at +0x04);
//          the creature's PHome intelligent pointer at +0x168 as in the landed InitCreaturePosition 0x00917330.
#include "engine/CAIStateGroup_IdleAtHome.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    void RotateAboutZ(float angle, bool flag);  // 0x00A14260
};

struct CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

struct CThingAICreature {
    unsigned char _pad[0x168];
    CBaseIntelligentPointer PHome;              // +0x168 CIntelligentPointer<CThingBuilding>
    CThing* GetPHome() { return PHome.GetPItem(); }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CAIStateGroupIdleAtHomeMethods : CAIStateGroup_IdleAtHome {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_base_0x04; }
    CThingAICreature* GetCreature() const { return Brain()->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_MinionIdle();                 // 0x0097FA00
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupIdleAtHomeMethods::Update()
{
    if (GetCreature()->GetPHome() != 0) {
        C3DVector home = GetCreature()->GetPHome()->GetPosition();
        C3DVector offset(GFFloatRandom(4.5f, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
        offset.RotateAboutZ(GFFloatRandom(1.0f, GetPWorldSeed()), false);
        C3DVector target = offset + home;
        long result;
        do {
            if (GetCreature()->GetPHome() == 0) {
                Terminate();
                return;
            }
            bool reached;
            result = AIAction_MoveToPosProximity(2, target, 1.0f, &reached, 3, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (result == 2 && GetCreature()->GetPHome() != 0)
            AIAction_MinionIdle();
    }
    Terminate();
}
