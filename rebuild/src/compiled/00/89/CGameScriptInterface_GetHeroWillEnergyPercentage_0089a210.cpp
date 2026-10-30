// CGameScriptInterface::GetHeroWillEnergyPercentage @ 0089a210
// __fastcall, returns long, 0 params.

#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> MapNode;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    void* pad0;         // +0x0
    MapNode* end;       // +0x4
    const MapNode* LowerBound(const ETCInterfaceType& key) const; // __fastcall: this in ecx, key on stack
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > IFaceMap;

struct Thing {
    unsigned char pad0[0x20];
    unsigned int field20;      // +0x20
    unsigned char pad24[0x44 - 0x24];
    IFaceMap ifaces;           // +0x44
    unsigned char pad4c[0x91 - 0x4c];
    unsigned char flags91;     // +0x91
};
class CThingPlayerCreature : public Thing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


// externs (reloc-masked); all __fastcall to match retail this-in-ecx idiom
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern float __fastcall CalcPct(void* value);          // call C: this=value, returns a float (st0)

long __fastcall CGameScriptInterface_GetHeroWillEnergyPercentage(CGameScriptInterface* self)
{
    Thing* thing = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer())->GetPControlledCreature();
    if (thing != 0 && !(thing->flags91 & 1) && (thing->field20 & 0x10)) {
        ETCInterfaceType key = (ETCInterfaceType)(4);
        IFaceMap* m = &thing->ifaces;
        const MapNode* it = m->LowerBound(key);
        MapNode* end = m->end;
        if (it != end && it->first <= 4) {
            // use it
        } else {
            it = end;
        }
        return (long)CalcPct(((void*)it->second));       // (long) float: call __ftol2 0x00BFEA70
    }
    return 0;
}