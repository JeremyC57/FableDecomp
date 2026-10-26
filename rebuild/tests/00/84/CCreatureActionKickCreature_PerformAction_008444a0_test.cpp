// Behaviour test for 008444a0: src/008444a0.cpp copied verbatim; positions, radii and the kick callbacks stubbed so
// the alive gate, the reach test (distance < radii + 1.5) and the kickable follow-up are checked.
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_KickCreature::PerformAction() -- if the target creature is alive and within reach (both radii + 1.5m), notify it and, when it accepts, fetch the kicker's fire position/orientation
// Leaf: ?PerformAction@CCreatureAction_KickCreature
// Retry 10b (getsw sweep, FIXES B-15): the placement is read through an inline getter `GetPlacement()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008444A0, 242 bytes (256 with padding), virtual PerformAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTargetCreature +0xa8, PCreature +0x08), CThing::GetPosition
//   0x006A4D00, C3DVector::GetLength 0x00A14480 (manifest "GetScaled" is propagated: the body is sqrt(x*x+y*y+z*z)),
//   target calls 0x00661EE0 (void) and 0x00838A30 (bool) (names unconfirmed; manifest labels are propagated),
//   GetCreatureFirePosAndOrientation(CThing*, C3DVector*, CRightHandedSet*) 0x007BF000 (__fastcall). Float 1.5f at 0x0125AADC.
// Layout: the landed sibling CCreatureAction_KillCreature::PerformAction 0x008445A0 (same TU): retail CCreatureActionBase is
//   0xa8 bytes, PTargetCreature +0xa8; CThing +0x60 the placement block (GetPosition is its +0xc), whose +0x34 is the radius.
// Shape: the target is a named local fetched first, so its position is the left operand of an inline C3DVector `-`
//   (the creature's position is fetched first, the components computed z,y,x and stored x,y,z); the two radii are named
//   float locals (copied through integer registers); `length < creatureRadius + targetRadius + 1.5f`.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const;                // 0x00A14480
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};
struct CThingPlacement {
    unsigned char _pad00[0x34];
    float Radius;                           // +0x34
};
class CThing { public:
    unsigned char _pad00[0x60];
    CThingPlacement* m_pPlacement;          // +0x60
    CThingPlacement* GetPlacement() const { return m_pPlacement; }
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPosition_006a4d00() const;   // 0x006A4D00
    void OnKicked();                        // 0x00661EE0 (name unconfirmed)
    bool IsKickable();                      // 0x00838A30 (name unconfirmed)
};
void __fastcall GetCreatureFirePosAndOrientation(CThing* creature, C3DVector* position, CRightHandedSet* orientation);   // 0x007BF000

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCreatureAction_KickCreature { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    CBaseIntelligentPointer PTargetCreature;   // +0xa8
};

void CCreatureAction_KickCreature::PerformAction()
{
    if (IS_THING_ALIVE(PTargetCreature)) {
        CThing* target = PTargetCreature.GetPItem();
        C3DVector delta = target->GetPosition_006a4d00() - PCreature.GetPItem()->GetPosition_006a4d00();
        float creatureRadius = PCreature.GetPItem()->GetPlacement()->Radius;
        float targetRadius = PTargetCreature.GetPItem()->GetPlacement()->Radius;
        if (delta.GetLength() < creatureRadius + targetRadius + 1.5f) {
            PTargetCreature.GetPItem()->OnKicked();
            if (PTargetCreature.GetPItem()->IsKickable()) {
                C3DVector position;
                CRightHandedSet orientation;
                GetCreatureFirePosAndOrientation(PCreature.GetPItem(), &position, &orientation);
            }
        }
    }
}
// ---- end source ----
static unsigned char g_aRaw[0x100], g_bRaw[0x100];
static CThing* g_a = (CThing*)g_aRaw; static CThing* g_b = (CThing*)g_bRaw;
static C3DVector g_posA(0, 0, 0), g_posB(3, 4, 0);
static int g_kicked = 0, g_fires = 0; static bool g_kickable = false; static CThing* g_fireCreature = 0;
float C3DVector::GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
const C3DVector& CThing::GetPosition_006a4d00() const { return this == g_a ? g_posA : g_posB; }
void CThing::OnKicked() { ++g_kicked; }
bool CThing::IsKickable() { return g_kickable; }
void __fastcall GetCreatureFirePosAndOrientation(CThing* creature, C3DVector*, CRightHandedSet*) { ++g_fires; g_fireCreature = creature; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
int main()
{
    static CThingPlacement pa, pb; pa.Radius = 1.0f; pb.Radius = 2.0f;
    g_a->m_pPlacement = &pa; g_b->m_pPlacement = &pb;
    static unsigned char raw[sizeof(CCreatureAction_KickCreature)]; memset(raw, 0, sizeof(raw));
    CCreatureAction_KickCreature* k = (CCreatureAction_KickCreature*)raw;
    k->PCreature.m_pItem = g_a;
    k->CCreatureAction_KickCreature::PerformAction(); CHECK(g_kicked == 0);           // no target
    k->PTargetCreature.m_pItem = g_b;
    k->CCreatureAction_KickCreature::PerformAction(); CHECK(g_kicked == 0 && g_fires == 0);   // distance 5 is not below 1 + 2 + 1.5
    // the distance 5 is beyond 4.5: recheck with a closer target
    g_kicked = 0; g_posB = C3DVector(3, 2, 0);                                       // sqrt(13) = 3.6 < 4.5
    k->CCreatureAction_KickCreature::PerformAction(); CHECK(g_kicked == 1 && g_fires == 0);
    g_kickable = true;
    k->CCreatureAction_KickCreature::PerformAction(); CHECK(g_kicked == 2 && g_fires == 1 && g_fireCreature == g_a);
    g_b->m_flags91 = 1;
    k->CCreatureAction_KickCreature::PerformAction(); CHECK(g_kicked == 2);           // dead target
    if (fails == 0) printf("008444a0_TEST PASS\n"); else printf("008444a0_TEST FAIL\n");
    return fails;
}