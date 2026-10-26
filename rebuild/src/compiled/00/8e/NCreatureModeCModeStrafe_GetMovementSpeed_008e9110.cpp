// NCreatureMode::CModeStrafe::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const -- the per-frame speed of the strafe animation chosen for this direction: its movement distance over its duration, divided by the frame rate
// Leaf: ?GetMovementSpeed@CModeStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9110, 123 bytes (+ int3 padding), `ret 0Ch`; CModeStrafe vtable slot 7.
// Callees: GetStrafeAnimationName @ 0x008E8E50 (a same-file free __fastcall returning CCharString by value: the return slot
//   in ecx, the first vector in edx, `ret 8`; label "GetStrafeAnimationName" in ghidra_out), CThingCreatureBase::GetAnimFromName
//   @ 0x00662A00, CMeshDataBank::PeekAnimation @ 0x00A242C0, C3DAnimationInfo::GetDuration @ 0x00A23CE0 (manifest
//   "DrawGetSeaLevel" is propagated), CThingCreatureBase::GetAnimMovementDistance @ 0x006627A0, ~CCharString @ 0x0099EAE0.
// Name: FableWin.pdb `?GetMovementSpeed@CModeStrafe@NCreatureMode@@UBEMABVC3DVector@@0W4EMovementType@@@Z` (slot 7).
// Globals: g_pMainGameComponent 0x013B86A0 (world +0x24, the world's main mesh bank +0x3c: CThing::PeekWorld inlined),
//   g_FramesPerSecond 0x01375550 (long).
// Shape: the name temporary takes the dead third-parameter slot; the bank is a named local fetched before the creature;
//   the animation index is a named local (edi, used twice); `speed / fps` with `speed` a named float.
class CCharString { public:
    void* PStringData;
    ~CCharString();                        // 0x0099EAE0
};

class C3DVector { public: float X, Y, Z; };
enum EMovementType { MOVEMENT_TYPE_0 };

CCharString __fastcall GetStrafeAnimationName(const C3DVector& a, const C3DVector& b, EMovementType type);   // 0x008E8E50

class C3DAnimationInfo { public:
    float GetDuration() const;             // 0x00A23CE0
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;          // +0x3c
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       World;                   // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_FramesPerSecond;                     // 0x01375550

class CAnimationEntry { public:                   // Ego_r.pdb CAnimationEntry (0x10)
    long BankIndex;                        // +0
    unsigned char _rest[0xc];
};
class CThingCreatureBase { public:
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
    float GetAnimMovementDistance(long animIndex) const;                    // 0x006627A0
    static const CWorld& PeekWorld() { return *g_pMainGameComponent->World; }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
};

namespace NCreatureMode {
struct CModeStrafe : CModeBase {
    float GetMovementSpeed(const C3DVector& a, const C3DVector& b, EMovementType type) const;
};
}

float NCreatureMode::CModeStrafe::GetMovementSpeed(const C3DVector& a, const C3DVector& b, EMovementType type) const
{
    CCharString name = GetStrafeAnimationName(a, b, type);
    CMeshDataBank* bank = CThingCreatureBase::PeekWorld().PMainMeshBank;
    CThingCreatureBase* creature = ParentThing;
    long animIndex = creature->GetAnimFromName(name, 0)->BankIndex;
    float duration = bank->PeekAnimation(animIndex)->GetDuration();
    float speed = creature->GetAnimMovementDistance(animIndex) / duration;
    return speed / (float)g_FramesPerSecond;
}