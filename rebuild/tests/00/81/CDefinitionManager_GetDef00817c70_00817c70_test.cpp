// Leaf: ??$GetDef@VCHitLocationDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCHitLocationDef@@@@@Z
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

template bool CDefinitionManager::GetDef<CHitLocationDef>(long, CDefPointer<const CHitLocationDef>*) const;

#include <stdio.h>
#include <string.h>
template<class T> static T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
static unsigned long def[32], manager[128];
static int queries;
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const {
    ++queries;
    CDefPointer<const CDefClassBase> value;
    if (index == 7) {
        ++def[1];
        field<void*>(&value, 0) = def;
    }
    return value;
}
int __cdecl main() {
    CDefinitionManager* m = reinterpret_cast<CDefinitionManager*>(manager);
    unsigned long old[32] = {0, 2};
    def[1] = 1;
    CDefPointer<const CHitLocationDef> out;
    field<void*>(&out, 0) = old;
    for (int i = -1; i < 2; ++i)
        if (m->GetDef(i, &out) || out.Get() != reinterpret_cast<CHitLocationDef*>(old)) return 1;
    if (queries != 1 || old[1] != 2) return 2;
    if (!m->GetDef(7, &out) || out.Get() != reinterpret_cast<CHitLocationDef*>(def)) return 3;
    if (old[1] != 1 || def[1] != 2 || !m->GetDef(7, &out) || def[1] != 2) return 4;
    out.Release();
    if (def[1] != 1 || queries != 3) return 5;
    puts("00817c70_TEST PASS");
    return 0;
}
