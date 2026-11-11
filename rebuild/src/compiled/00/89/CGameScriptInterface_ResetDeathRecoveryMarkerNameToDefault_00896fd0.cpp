// CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct SubB;
// agentNF17: the MapKind / MapNode stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
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
struct Node2 { void* p; };

extern "C" void __fastcall FinalizeNode(void* n);      // this in ecx

void __fastcall CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(CGameScriptInterface* self)
{
    ETCInterfaceType key;
    CPlayer* a = (&self->PlayerManager->GetMainPlayer());
    CThingPlayerCreature* obj = a->GetPControlledCreature();
    if (!obj) return;
    if (*(unsigned char*)((char*)obj + 0x91) & 1) return;
    if (!(*(unsigned char*)((char*)obj + 0x20) & 0x10)) return;

    CInterfaceMap* m = (CInterfaceMap*)((char*)obj + 0x44);
    key = (ETCInterfaceType)4;
    CInterfaceMap::value_type* found = m->LowerBound(key);
    CInterfaceMap::value_type* endn = m->_M_finish;
    if (found == endn || found->first > 4) {
        found = endn;
    }
    FinalizeNode(found->second);
}