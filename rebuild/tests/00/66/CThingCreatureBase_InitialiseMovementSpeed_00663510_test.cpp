// Behaviour test for 00663510: src/00663510.cpp copied verbatim; an unset speed on a creature with TC 90 becomes
// |MovementVector| * 0.01 / (Duration * framesPerSecond); a set speed, a missing TC or a zero distance/duration leave it alone.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCreatureBase::InitialiseMovementSpeed(float& speed, const CCharString& animName) -- an unset speed (0) of a creature carrying TC 90 becomes the named animation's movement distance per second: (|MovementVector| * 0.01) / (Duration * frames per second)
// Retry 10b (statement-permutation sweep): the scaled distance is computed BEFORE the duration local is read; that order
// spills it with retail's `fstp [esp+10h] ; fld [esp+10h]` (duration first kept it with one `fst`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00663510, length 181 bytes, `ret 8`.
// Callees: CThingCreatureBase::GetAnimFromName(const CCharString&, long) @ 0x00662A00, CThing::PeekWorld @ 0x004C7AB0, the main
//   mesh bank's GetAnimation(long) @ 0x00A242C0.
// Globals/pool: g_framesPerSecond @ 0x01375550 (long, `fild`), 0x0122DEDC = 0.0f, 0x01230A5C = 0.01f.
// Identity: engine_api.tsv / Ego_r.pdb `InitialiseMovementSpeed(float*, CCharString*)` on CThingCreatureBase.
// Layout: retail CThing interface bitset +0x20 (TC 90 = word +0x28 bit 26); C3DAnimationInfo Duration +0x10,
//   MovementVector +0x18; CWorld PMainMeshBank +0x3c.
// Shape: the landed GetAnimMovementDistance / GetAnimDuration (0x006627A0 / 0x006627F0) bodies inlined on one
//   GetAnimation result: the duration copies as a dword into the dead argument slot, the magnitude is the by-value
//   Magnitude(x, y, z) helper (Z, Y, X loads, `fstp st(3)` cleanup); the HasTC probe is the raw bitset test.
#include <math.h>

inline float Magnitude(float x, float y, float z) { return (float)sqrt(x * x + y * y + z * z); }

class C3DVector { public:
    float X, Y, Z;
    float Length() const { return Magnitude(X, Y, Z); }
};

class C3DAnimationInfo { public:
    unsigned char _pad[0x10];
    float     Duration;                        // +0x10
    float     NonLoopingDuration;              // +0x14
    C3DVector MovementVector;                  // +0x18
    float     Rotation;                        // +0x24
};

class CMeshDataBank { public:
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};

class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};

class CCharString;
class CAnimationEntry { public:
    long AnimIndex;                            // +0x00
};

extern long g_framesPerSecond;                 // 0x01375550

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];          // +0x20 bitset<274>
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};

class CThingCreatureBase : public CThing { public:
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
    void InitialiseMovementSpeed(float& speed, const CCharString& animName);
};

void CThingCreatureBase::InitialiseMovementSpeed(float& speed, const CCharString& animName)
{
    if (speed == 0.0f && (UsingComponents[90 >> 5] & (1UL << (90 & 31)))) {
        CAnimationEntry* entry = GetAnimFromName(animName, 0);
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        C3DAnimationInfo* info = bank->GetAnimation(entry->AnimIndex);
        float distance = info->MovementVector.Length() * 0.01f;
        float duration = info->Duration;
        if (distance > 0.0f && duration > 0.0f)
            speed = distance / (duration * g_framesPerSecond);
    }
}
// ---- end source ----
long g_framesPerSecond = 30;
static C3DAnimationInfo g_info; static CMeshDataBank g_bank; static CWorld g_world; static CAnimationEntry g_entry;
static long g_askedIndex = -1;
C3DAnimationInfo* CMeshDataBank::GetAnimation(long index) { g_askedIndex = index; return &g_info; }
const CWorld& CThing::PeekWorld() const { return g_world; }
CAnimationEntry* CThingCreatureBase::GetAnimFromName(const CCharString&, long) { return &g_entry; }
int main()
{
    g_world.PMainMeshBank = &g_bank; g_entry.AnimIndex = 7;
    g_info.Duration = 2.0f; g_info.MovementVector.X = 300.0f; g_info.MovementVector.Y = 0.0f; g_info.MovementVector.Z = 400.0f;
    static CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.UsingComponents[i] = 0;
    c.UsingComponents[90 >> 5] = 1UL << (90 & 31);
    const CCharString* name = 0;
    float speed = 0.0f;
    c.InitialiseMovementSpeed(speed, *name);
    CHECK(g_askedIndex == 7);
    CHECK(speed > 0.0833f && speed < 0.0834f);                   // 500 * 0.01 / (2 * 30)
    speed = 3.0f; c.InitialiseMovementSpeed(speed, *name); CHECK(speed == 3.0f);
    speed = 0.0f; g_info.Duration = 0.0f; c.InitialiseMovementSpeed(speed, *name); CHECK(speed == 0.0f);
    g_info.Duration = 2.0f; c.UsingComponents[90 >> 5] = 0; c.InitialiseMovementSpeed(speed, *name); CHECK(speed == 0.0f);
    if (fails == 0) printf("00663510_TEST PASS\n"); else printf("00663510_TEST FAIL\n");
    return fails;
}