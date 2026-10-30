#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

class CTCBase;
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CKeyPair;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    const CKeyPair* LowerBound(const ETCInterfaceType& pKey) const;
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x2c];
    unsigned int  m_Flags2C;       // 0x2c
    unsigned char m_Pad30[0x14];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};
class CThingPlayerCreature : public CThingOverlay { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern "C" long __fastcall Sub_GetExperiencePickUpMultiplier(void* iface);

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x68;

long __fastcall CGameScriptInterface_GetPlayerCreatureCombatMultiplier(
    const CGameScriptInterface* self)
{
    CPlayer* a = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    CThingOverlay* pThing = a->GetPControlledCreature();
    if (pThing != 0 &&
        (pThing->m_Flags91 & 0x01) == 0 &&
        (pThing->m_Flags2C & 0x100) != 0)
    {
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        const CKeyPair* pEntry = pMap->LowerBound(key);
        CKeyPair* pEnd = pMap->m_pEnd;
        if (pEntry == pEnd || (int)pEntry->first > 0x68)
            pEntry = pEnd;

        return Sub_GetExperiencePickUpMultiplier(((void*)pEntry->second));
    }
    return 1;
}
