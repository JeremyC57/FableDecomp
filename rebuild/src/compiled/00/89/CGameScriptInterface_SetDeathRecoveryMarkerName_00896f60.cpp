// CGameScriptInterface::SetDeathRecoveryMarkerName
#include "fable_compiler.h"
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString
{ public:
    const CCharString& operator=(const CCharString& other);
};
struct DeathRecoveryMarkerNameInterface
{
    char pad00[0x170];
    CCharString markerName;
    void SetMarkerName(const CCharString& n) { markerName = n; }
};
// agentNF17: the DeathRecoveryMarkerNameNode / DeathRecoveryMarkerNameMap stand-ins are the explicit specialisation
//   CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90 idiom) and its std::pair value_type; 0x0040F020 is its LowerBound
//   (NF14 / established spelling). The inline map find keeps the row's lookup (key copy, end when absent).
class CTCBase;
enum ETCInterfaceType { };
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

class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

template <class T> FABLE_FORCEINLINE bool GetTCOut(unsigned char* thing, int kind, T** out)
{
    if (*(unsigned char*)(thing + 0x20) & 0x10) {
        CInterfaceMap* map = (CInterfaceMap*)(thing + 0x44);
        ETCInterfaceType key = (ETCInterfaceType)kind;
        CInterfaceMap::value_type* found = map->LowerBound(key);
        if (found == map->_M_finish || found->first > kind)
            found = map->_M_finish;
        *out = (T*)found->second;
        return true;
    }
    return false;
}

void __fastcall CGameScriptInterface_SetDeathRecoveryMarkerName(
    CGameScriptInterface* self,
    unsigned long,
    const CCharString& name)
{
    CPlayer* source = (&self->PlayerManager->GetMainPlayer());
    unsigned char* target =
        (unsigned char*)source->GetPControlledCreature();

    if (target == 0)
        return;
    if (*(unsigned char*)(target + 0x91) & 0x01)
        return;
    DeathRecoveryMarkerNameInterface* markerInterface;
    if (GetTCOut(target, 4, &markerInterface))
        markerInterface->markerName = name;
}
