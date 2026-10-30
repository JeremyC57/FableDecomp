class CCharString;
class CGameScriptInterface;
class CScriptThing;

class CCharString
{
public:
    CCharString(const char*, long);
    ~CCharString();

private:
    void* storage_;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

typedef void (CGameScriptInterface::*DispatchFn)(
    const CScriptThing&,
    const CCharString&,
    bool) const;

struct VTableOverlay
{
    unsigned char pad[0x938];
    DispatchFn dispatch;
};

struct InterfaceOverlay
{
    const VTableOverlay* vtable;
};

void CGameScriptInterface::EntitySetCombatEnabled(
    const CScriptThing& entity,
    bool enabled) const
{
    const InterfaceOverlay* p = (const InterfaceOverlay*)this;
    {
        CCharString state("SG_SEEK_ENEMY", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY2", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
}