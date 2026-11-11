#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSITheftMoralityStats
{
public:
    void SetMorality(long morality);

    unsigned char m_Pad00[0x28];
    long          m_Morality; // 0x28
};

// agentNF16: the stand-in key-pair and map structs (CKeyPair, a non-template CVectorMap) are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the
//   landed 0x0074EE90 idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThingOverlay
{
    unsigned char m_Pad00[0x20];
    unsigned char m_Flags20;       // 0x20
    unsigned char m_Pad21[0x23];
    CInterfaceMap m_InterfaceMap44; // 0x44
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
        CInterfaceMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CInterfaceMap::value_type* pEntry = pMap->LowerBound(key);
        CInterfaceMap::value_type* pEnd = pMap->_M_finish;
        if (pEntry == pEnd || (int)pEntry->first > 4)
            pEntry = pEnd;

        CGSITheftMoralityStats* pStats = (CGSITheftMoralityStats*)pEntry->second;
        CDefinitionManagerOverlay* pManager = &CGameDefinitionManager::Get();
        long change = pManager->m_Definition->m_Change;
        pStats->SetMorality(pStats->m_Morality + change);
    }
}
