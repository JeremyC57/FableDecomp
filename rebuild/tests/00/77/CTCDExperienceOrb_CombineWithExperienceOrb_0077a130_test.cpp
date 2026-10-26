// Behaviour test for 0077a130: src/0077a130.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCDExperienceOrb::CombineWithExperienceOrb(CThing* otherOrb)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDExperienceOrb::CombineWithExperienceOrb(CThing* otherOrb) -- absorb the other orb's points, play the combining effect, kill it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077A130, length 72 bytes (+8 int3).
// Callees: CTCDExperienceOrb::SetExperiencePoints(long) @ 0x00779E10, CThing::GetPosition @ 0x004C73D0, the __fastcall
// thing creator @ 0x006E0880 (def index in ecx, position in edx, one stack argument), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the other thing's orb driver is the inlined GetTCDriver probe: bitset word +0x2c (CThing UsingComponents +0x20,
// word 3) bit 9 selects PTCDriver (+0x5c, retail offset) else null, read without a null test (`->ExperiencePoints`, +0x10).
// The effect is CExperienceDef::OrbCombiningParticleEffect (def +0x28) created at the parent thing's position.
class C3DVector { public: float X, Y, Z; };

class CTCDExperienceOrb;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];       // +0x20 bitset<274>
    unsigned char _pad44[0x5c - 0x44];
    void* PTCDriver;                        // +0x5c
    const C3DVector& GetPosition() const;   // 0x004C73D0
    void Kill(bool checkLocked);            // 0x004C9B80
    CTCDExperienceOrb* GetTCDriverExperienceOrb() const
    {
        CTCDExperienceOrb* driver = 0;
        if (UsingComponents[3] & 0x200)
            driver = (CTCDExperienceOrb*)PTCDriver;
        return driver;
    }
};

CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

class CExperienceDef { public:
    unsigned char _pad00[0x28];
    long OrbCombiningParticleEffect;        // +0x28
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CTCDExperienceOrb { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CDefPointer<CExperienceDef> PDef;       // +0x0c
    long ExperiencePoints;                  // +0x10
    long CombinationImmunityTimer;          // +0x14
    long LifeTimer;                         // +0x18
    bool DecayOverTime;                     // +0x1c
    bool PickedUp;                          // +0x1d
    CThing* GetParent() const { return ParentThing; }
    void SetExperiencePoints(long points);  // 0x00779E10
    void CombineWithExperienceOrb(CThing* otherOrb);
};

void CTCDExperienceOrb::CombineWithExperienceOrb(CThing* otherOrb)
{
    SetExperiencePoints(otherOrb->GetTCDriverExperienceOrb()->ExperiencePoints + ExperiencePoints);
    CreateThing(PDef->OrbCombiningParticleEffect, GetParent()->GetPosition(), 0);
    otherOrb->Kill(true);
}

// ---- stubs and driver ----

static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
static const CThing* g_posOf; static int g_posCalls = 0;
const C3DVector& CThing::GetPosition() const { g_posOf = this; ++g_posCalls; return g_pos; }
static CThing* g_killed; static bool g_killArg; static int g_kills = 0;
void CThing::Kill(bool checkLocked) { g_killed = this; g_killArg = checkLocked; ++g_kills; }
static long g_createDef; static const C3DVector* g_createPos; static long g_createFlags; static int g_creates = 0;
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags) { g_createDef = defIndex; g_createPos = &position; g_createFlags = flags; ++g_creates; return 0; }
static long g_setPoints; static CTCDExperienceOrb* g_setOn;
void CTCDExperienceOrb::SetExperiencePoints(long points) { g_setOn = this; g_setPoints = points; ExperiencePoints = points; }
int main()
{
    static CThing parent, other; memset(&parent, 0, sizeof parent); memset(&other, 0, sizeof other);
    CExperienceDef def; def.OrbCombiningParticleEffect = 0x123;
    CTCDExperienceOrb otherOrb; memset(&otherOrb, 0, sizeof otherOrb); otherOrb.ExperiencePoints = 40;
    other.UsingComponents[3] = 0x200; other.PTCDriver = &otherOrb;
    CTCDExperienceOrb orb; memset(&orb, 0, sizeof orb); orb.ParentThing = &parent; orb.PDef.Object = &def; orb.ExperiencePoints = 2;
    orb.CombineWithExperienceOrb(&other);
    CHECK(g_setOn == &orb); CHECK(g_setPoints == 42); CHECK(orb.ExperiencePoints == 42);
    CHECK(g_posOf == &parent); CHECK(g_createDef == 0x123); CHECK(g_createPos == &g_pos); CHECK(g_createFlags == 0); CHECK(g_creates == 1);
    CHECK(g_killed == &other); CHECK(g_killArg == true); CHECK(g_kills == 1);
    if (fails == 0) printf("0077a130_TEST PASS\n"); else printf("0077a130_TEST FAIL\n");
    return fails;
}