#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CKeyPair
{
    ETCInterfaceType m_Key;   // 0x00
    void*            m_Value; // 0x04
};

struct CVectorMap
{
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x24];
    unsigned int  m_Flags24;       // 0x24
    unsigned char m_Pad28[0x1c];
    CVectorMap    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};
class CThingPlayerCreature : public CThingOverlay { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern "C" bool __fastcall Sub_IsMeleeFlourishValid(void* iface);

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x30;

bool __fastcall CGameScriptInterface_IsPlayerCreatureFlourishEnabled(
    const CGameScriptInterface* self)
{
    CPlayer* a = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    CThingOverlay* pThing = a->GetPControlledCreature();
    if (pThing != 0 &&
        (pThing->m_Flags91 & 0x01) == 0 &&
        (pThing->m_Flags24 & 0x10000) != 0)
    {
        CVectorMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CKeyPair* pEntry = pMap->LowerBound(&key);
        CKeyPair* pEnd = pMap->m_pEnd;
        if (pEntry == pEnd || (int)pEntry->m_Key > 0x30)
            pEntry = pEnd;

        return Sub_IsMeleeFlourishValid(pEntry->m_Value);
    }
    return false;
}
