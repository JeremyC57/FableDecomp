#include "engine/CScriptThing.h"

struct CScriptThingVtbl {
    unsigned char _pad[0x12c];
    unsigned char (__fastcall *IsAlive)(CScriptThing* scriptThing);
};

bool __fastcall CScriptThing_IsDead(CScriptThing* scriptThing)
{
    return ((CScriptThingVtbl*)scriptThing->__vftable)->IsAlive(scriptThing) == 0;
}
