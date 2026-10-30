#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSITheftMoralityStats
{
public:
    void SetMorality(long morality);

    unsigned char m_Pad00[0x28];
    long          m_Morality; // 0x28
};

struct CKeyPair
{
    ETCInterfaceType       m_Key;   // 0x00
    CGSITheftMoralityStats* m_Value; // 0x04
};

struct CVectorMap
{
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x20];
    unsigned char m_Flags20;       // 0x20
    unsigned char m_Pad21[0x23];
    CVectorMap    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};
class CThingPlayerCreature : public CThingOverlay { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

struct CTheftMoralityDefinition
{
    unsigned char m_Pad00[0xe4];
    long          m_Change; // 0xe4
};

struct CDefinitionManagerOverlay
{
    unsigned char             m_Pad00[0xdc];
    CTheftMoralityDefinition* m_Definition; // 0xdc
};
class CGameDefinitionManager : public CDefinitionManagerOverlay { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ


// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

const ETCInterfaceType kTargetType = (ETCInterfaceType)4;

void __fastcall CGameScriptInterface_ChangeHeroMoralityDueToTheft(
    const CGameScriptInterface* self)
{
    CThingOverlay* pThing = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer())->GetPControlledCreature();
    if (pThing != 0 &&
        (pThing->m_Flags91 & 0x01) == 0 &&
        (pThing->m_Flags20 & 0x10) != 0)
    {
        CVectorMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CKeyPair* pEntry = pMap->LowerBound(&key);
        CKeyPair* pEnd = pMap->m_pEnd;
        if (pEntry == pEnd || (int)pEntry->m_Key > 4)
            pEntry = pEnd;

        CGSITheftMoralityStats* pStats = pEntry->m_Value;
        CDefinitionManagerOverlay* pManager = &CGameDefinitionManager::Get();
        long change = pManager->m_Definition->m_Change;
        pStats->SetMorality(pStats->m_Morality + change);
    }
}
