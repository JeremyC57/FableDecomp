#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

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
    unsigned char m_Pad00[0x24];
    unsigned int  m_Flags24;       // 0x24
    unsigned char m_Pad28[0x1c];
    CInterfaceMap m_InterfaceMap44; // 0x44
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
        CInterfaceMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CInterfaceMap::value_type* pEntry = pMap->LowerBound(key);
        CInterfaceMap::value_type* pEnd = pMap->_M_finish;
        if (pEntry == pEnd || (int)pEntry->first > 0x30)
            pEntry = pEnd;

        return Sub_IsMeleeFlourishValid(pEntry->second);
    }
    return false;
}
