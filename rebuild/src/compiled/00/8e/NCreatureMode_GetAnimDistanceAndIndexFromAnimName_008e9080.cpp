// NCreatureMode::GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase&, const CCharString&, long&, float&) -- bank index of the named animation and its per-frame movement distance
// Leaf: ?GetAnimDistanceAndIndexFromAnimName@
// Retail 0x008E9080, 0x5B bytes, __fastcall bool (creature in ecx, name in edx, index& and distance& on the stack,
//   `ret 8`). Manifest label "GetAbilityAnimationDuration" (_global) is wrong; the FableWin PDB name set has
//   `?GetAnimDistanceAndIndexFromAnimName@NCreatureMode@@YI_NABVCThingCreatureBase@@ABVCCharString@@AAJAAM@Z`.
//   Callers: CModeStrafe::OnActivate 0x008E9A30 (twice) and one more NCreatureMode site (0x008E9BE3).
// Callees: CThingCreatureBase::GetAnimFromName(const CCharString&, long) const @ 0x00662A00, CMeshDataBank::PeekAnimation
//   @ 0x00A242C0, C3DAnimationInfo::GetDuration @ 0x00A23CE0, CThingCreatureBase::GetAnimMovementDistance @ 0x006627A0.
// Globals: g_pMainGameComponent 0x013B86A0 (world +0x24, main mesh bank +0x3c), g_FramesPerSecond 0x01375550 (long).
// Shape: the model of the landed sibling CModeStrafe::GetMovementSpeed 0x008E9110: the bank is a named local fetched
//   first, the index is stored through the out reference and passed straight on (eax), then re-read for the distance
//   call; `speed = distance / duration` as a named float, `distance = speed / (float)fps` (the fild / fdivr / pop tail).
class CCharString {
public:
    void* PStringData;                          // +0x00
};

class C3DAnimationInfo {
public:
    float GetDuration() const;                  // 0x00A23CE0
};

class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

class CWorld {
public:
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;               // +0x3c
};

class CMainGameComponent {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_FramesPerSecond;                     // 0x01375550

class CAnimationEntry {                         // Ego_r.pdb CAnimationEntry (0x10)
public:
    long BankIndex;                             // +0x00
    unsigned char _rest[0xc];
};

class CThingCreatureBase {
public:
    const CAnimationEntry* GetAnimFromName(const CCharString& name, long index) const;   // 0x00662A00
    float GetAnimMovementDistance(long animIndex) const;                                 // 0x006627A0
    static const CWorld& PeekWorld() { return *g_pMainGameComponent->World; }
};

namespace NCreatureMode {
bool __fastcall GetAnimDistanceAndIndexFromAnimName(const CThingCreatureBase& creature, const CCharString& animName,
                                                    long& index, float& distance)
{
    CMeshDataBank* bank = CThingCreatureBase::PeekWorld().PMainMeshBank;
    index = creature.GetAnimFromName(animName, 0)->BankIndex;
    float duration = bank->PeekAnimation(index)->GetDuration();
    float speed = creature.GetAnimMovementDistance(index) / duration;
    distance = speed / (float)g_FramesPerSecond;
    return true;
}
}