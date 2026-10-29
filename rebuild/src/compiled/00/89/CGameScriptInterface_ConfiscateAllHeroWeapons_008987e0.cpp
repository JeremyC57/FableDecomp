#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> Node;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: char pad[4]; Node* endp; Node* LowerBound(const ETCInterfaceType& key); };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Container;
struct Obj { char pad20[0x20]; unsigned int flags20; char pad24[0x44-0x24]; Container c44; char padmid[0x91-0x4c]; unsigned char flag91; };
class CThingPlayerCreature : public Obj { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

extern CPlayer* __fastcall GetSub(void* self);
extern void __fastcall DoIt(void* self);

void __fastcall CGameScriptInterface_ConfiscateAllHeroWeapons(CGameScriptInterface* self)
{
    CPlayer* a = GetSub(self->PlayerManager);
    Obj* o = a->GetPControlledCreature();
    if (!o) return;
    if (o->flag91 & 1) return;
    if (!(o->flags20 & 0x80000)) return;
    Container* c = &o->c44;
    ETCInterfaceType k = (ETCInterfaceType)(0x13);
    Node* n = c->LowerBound(k);
    Node* end = c->endp;
    if (n != end && n->first <= 0x13) {
    } else {
        n = end;
    }
    DoIt(((void*)n->second));
}