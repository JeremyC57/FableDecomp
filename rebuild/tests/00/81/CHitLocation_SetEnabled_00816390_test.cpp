// Leaf: ?SetEnabled@CHitLocation@NHitLocations@@QAEX_N@Z
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include "rebuild_abi.h"
#include "engine/CThing.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHitLocationsDef.h"
#include "engine/CArmourDef.h"
#include "engine/CTCHitLocations.h"

// The shared inline SetEnabled definition is provided by the activating component.

#include <stdio.h>
#include <string.h>
template<class T> static T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
int __cdecl main() {
    unsigned long owner[128] = {0}, tc[16] = {0}, def[32] = {0}, entry[6] = {0};
    unsigned long map[2] = {127, reinterpret_cast<unsigned long>(tc)};
    field<unsigned long>(owner, 44) = 0x80000000UL;
    field<void*>(owner, 68) = map; field<void*>(owner, 72) = map+2;
    field<void*>(entry, 0) = owner; field<void*>(entry, 4) = def;
    NHitLocations::CHitLocation* location = reinterpret_cast<NHitLocations::CHitLocation*>(entry);
    for (int d = 0; d != 2; ++d) for (int enabled = 0; enabled != 2; ++enabled)
        for (int current = 0; current != 3; ++current) {
            field<bool>(def, 0x74) = d != 0;
            field<bool>(entry, 20) = !enabled;
            void* before = current == 2 ? entry : current == 1 ? tc : 0;
            void* wanted = d && enabled ? entry : d && before == entry ? 0 : before;
            field<void*>(tc, 24) = before;
            location->SetEnabled(enabled != 0);
            if (field<bool>(entry, 20) != (enabled != 0) || field<void*>(tc, 24) != wanted) return 1;
        }
    puts("00816390_TEST PASS");
    return 0;
}
