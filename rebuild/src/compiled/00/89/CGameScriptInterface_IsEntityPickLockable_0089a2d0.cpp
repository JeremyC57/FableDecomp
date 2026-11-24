#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum ETCInterfaceType { ETC_DUMMY = 0 };

class CScriptThing;
struct CThingOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CThingOverlay* (__fastcall* m_Resolve)(CScriptThing* self); // 0x2c
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self); // 0x12c
};

class CScriptThing
{ public:
    CScriptThingVTable* m_pVTable;
};

// NF386 (2026-11-24): the 0x26 interface is the door, and the call is ?IsLockPickable@CTCDoor@@QBE_NXZ (Ego_r.pdb + FableWin.pdb; landed self
// 0x00730AE0; was a made-up CGSIPickLockableDoor, non-const).
// CTCDoor comes from the shared header (flat branch) with its additive NF386 opt-in declaring IsLockPickable.
#define FABLE_RETAIL_CTCDOOR_FLAT_ISLOCKPICKABLE
#include "engine/CTCDoor.h"

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
    unsigned char m_Pad00[0x24];
    unsigned int  m_Flags24;       // 0x24
    unsigned char m_Pad28[0x1c];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >    m_InterfaceMap44; // 0x44
};


const ETCInterfaceType kTargetType = (ETCInterfaceType)0x26;

bool __fastcall CGameScriptInterface_IsEntityPickLockable(
    const CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing)
{
    (void)self;
    (void)edxUnused;
    if (pScriptThing->m_pVTable->m_IsAvailable(pScriptThing))
    {
        CThingOverlay* pThing =
            pScriptThing->m_pVTable->m_Resolve(pScriptThing);
        if ((pThing->m_Flags24 & 0x40) != 0)
        {
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* pMap = &pThing->m_InterfaceMap44;
            ETCInterfaceType key = kTargetType;
            const CKeyPair* pEntry = pMap->LowerBound(key);
            CKeyPair* pEnd = pMap->m_pEnd;
            if (pEntry == pEnd || (int)pEntry->first > 0x26)
                pEntry = pEnd;

            if (((CTCDoor*)pEntry->second)->IsLockPickable())
                return true;
        }
    }
    return false;
}
