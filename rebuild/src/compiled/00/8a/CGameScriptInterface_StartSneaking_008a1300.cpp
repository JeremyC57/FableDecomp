// CGameScriptInterface::StartSneaking @ 008a1300
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> Node;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    Node* begin; Node* end_;
    Node* LowerBound(const ETCInterfaceType& key); // __fastcall: ecx=this, key on stack
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Container;

struct Obj {
    unsigned char _pad0[0x24];
    unsigned int field24;      // +0x24
    unsigned char _pad28[0x44 - 0x28];
    Container cont;            // +0x44 (begin), +0x48 (end)
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char flags;       // +0x91
};
class CThingPlayerCreature : public Obj { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


struct M14 { char d; };

class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern void __fastcall FinalCall(void* p);           // ecx

void __fastcall CGameScriptInterface_StartSneaking(CGameScriptInterface* self)
{
    CPlayer* a = (&((CPlayerManager*)((M14*)self->PlayerManager))->GetMainPlayer());
    Obj* o = a->GetPControlledCreature();
    if (!o) return;
    if (o->flags & 1) return;
    if (!(o->field24 & 0x400000)) return;

    Container* c = &o->cont;
    ETCInterfaceType local = (ETCInterfaceType)(0x36);
    Node* r = c->LowerBound(local);
    Node* e = c->end_;
    if (r != e && r->first <= 0x36) {
        // keep r
    } else {
        r = e;
    }
    FinalCall(((void*)r->second));
}