// Behaviour test for 0x008561A0 CCombatActionBase::GetSoundPair (links the REAL row object; NF123 q6403: the old test linked the free
// stand-in GetSoundPair(CSoundPair*, CThingCreatureBase*, CCharString*)). The row includes the shared headers; this test reaches the
// protected static through a never-instantiated derived probe on the same headers. The creature is raw storage whose vtable has the
// slot-89 (+0x164) GetSoundPair: a __fastcall stand-in with an unused edx (thiscall-compatible) that fills the caller's result buffer.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include <stdio.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"

struct GetSoundPairProbe : CCombatActionBase {
    static CSoundPair Call(const CThingCreatureBase& creature, const CCharString& name) { return GetSoundPair(creature, name); }
};

static const void* g_name = 0;
static void* __fastcall Slot89(void* self, int, unsigned long* result, const void* name)
{
    (void)self;
    g_name = name;
    result[0] = 0x11112222u;
    result[1] = 0x33334444u;
    return result;
}

int main()
{
    void* vtable[90];
    for (int i = 0; i < 90; ++i) vtable[i] = 0;
    vtable[89] = (void*)&Slot89;
    void* creature[64];
    creature[0] = vtable;
    const CCharString* fakeName = (const CCharString*)0xDEAD1234u;
    unsigned long out[4] = { 0, 0, 0, 0 };
    *(CSoundPair*)out = GetSoundPairProbe::Call(*(const CThingCreatureBase*)creature, *fakeName);
    bool ok = out[0] == 0x11112222u && out[1] == 0x33334444u && g_name == (const void*)fakeName;
    if (ok) { printf("GETSOUNDPAIR_OK\n"); return 0; }
    printf("GETSOUNDPAIR_FAIL\n");
    return 1;
}
