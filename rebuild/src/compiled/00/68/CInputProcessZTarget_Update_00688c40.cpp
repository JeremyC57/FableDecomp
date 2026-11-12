// CInputProcessZTarget::Update(CProcessedInput& processed) -- touch the hero creature (player +0x18, 0x00487DC0), count the lock timer (+0x34) down while it is not negative, and when the player interface (+0x24) does not make Z-targeting a toggle (IsToggleZTarget 0x00445B80) and the (1, 2) button query (0x00687D50) does not hold, deactivate the Z-target (0x00688BA0 with the processed input)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688C40, length 80 bytes.
// Leaf: ?Update@CInputProcessZTarget@@UAEXAAVCProcessedInput@@@Z
// The labels "CTCShotDefinitionBase::CanBeHitByMeleeStrike" (0x00445B80) and "CPersistContext::Transfer" (0x00687D50) are propagated. Shape: `test eax,eax ; jl ; dec eax`, `ret 4`.
struct CThing_Retail;
class CThingPlayerCreature;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
};
// CGamePlayerInterface: the shared header (Ego_r/FableWin ?IsToggleZTarget@CGamePlayerInterface@@QBE_NXZ 0x00445B80; agentNF22 respelled
//   the CTarget_00445b80::IsValid stand-in -- the +0x24 member is the player interface, as the landed ProcessInput 0x00688EB0 has it).
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET
#include "engine/CGamePlayerInterface.h"
class CProcessedInput;
class CInputProcessZTarget { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void Update(CProcessedInput& processed);
    unsigned char      _pad04[0x14];
    CPlayer*           m_pPlayer;                  // +0x18
    unsigned char      _pad1c[8];
    CGamePlayerInterface* PGamePlayerInterface;    // +0x24
    unsigned char      _pad28[0xc];
    long               m_lockTimer;                // +0x34
    bool IsButtonHeld(long a, long b);             // 0x00687D50
    void DeactivateZTarget(CProcessedInput& processed);   // 0x00688BA0
};

void CInputProcessZTarget::Update(CProcessedInput& processed)
{
    m_pPlayer->GetPControlledCreature();
    if (m_lockTimer >= 0)
        m_lockTimer = m_lockTimer - 1;
    if (!PGamePlayerInterface->IsToggleZTarget() && !IsButtonHeld(1, 2))
        DeactivateZTarget(processed);
}