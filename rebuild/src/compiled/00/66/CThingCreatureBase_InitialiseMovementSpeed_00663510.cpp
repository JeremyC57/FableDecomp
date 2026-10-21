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

struct C3DVector {
    float X, Y, Z;
    float Length() const { return Magnitude(X, Y, Z); }
};

struct C3DAnimationInfo {
    unsigned char _pad[0x10];
    float     Duration;                        // +0x10
    float     NonLoopingDuration;              // +0x14
    C3DVector MovementVector;                  // +0x18
    float     Rotation;                        // +0x24
};

struct CMeshDataBank {
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};

struct CWorld {
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};

struct CCharString;
struct CAnimationEntry {
    long AnimIndex;                            // +0x00
};

extern long g_framesPerSecond;                 // 0x01375550

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];          // +0x20 bitset<274>
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};

struct CThingCreatureBase : CThing {
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