// CGameScriptInterface::RemoveHeroClothing @ 0089b390
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
// agentNF17: the Node / Vec stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90
//   idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
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

class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern void  __fastcall DoRemove(void* self);    // 0xffd18d56

struct HeroData {
    char pad[0x20];
    unsigned int flags20; // +0x20
    char pad2[0x91 - 0x24];
    unsigned char flag91;  // +0x91
    // Vec at +0x44
};
class CThingPlayerCreature : public HeroData { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


void __fastcall CGameScriptInterface_RemoveHeroClothing(CGameScriptInterface* self)
{
    ETCInterfaceType key;
    CPlayer* h = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    HeroData* d = h->GetPControlledCreature();
    if (!d) return;
    if (d->flag91 & 1) return;
    if (!(d->flags20 & 0x40000)) return;
    CInterfaceMap* vec = (CInterfaceMap*)((char*)d + 0x44);
    key = (ETCInterfaceType)0x12;
    CInterfaceMap::value_type* it = vec->LowerBound(key);
    CInterfaceMap::value_type* end = vec->_M_finish;
    if (it != end && it->first <= 0x12) {
        // keep it
    } else {
        it = end;
    }
    DoRemove(it->second);
}