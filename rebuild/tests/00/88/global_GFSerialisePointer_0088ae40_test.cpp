// Behaviour test for GFSerialisePointer @ 0x0088AE40: saving writes the live thing's remapped UID (0 for a null or dead
// thing); loading reads a UID and, when non-zero, creates the dummy thing, stores the remapped UID in its dummy component
// and points the intelligent pointer at it.
#include <stdio.h>

#ifdef _MSC_VER
typedef unsigned __int64 UID64;
#else
typedef unsigned long long UID64;
#endif

class CTCThingUIDDummyForSerialisation { public: UID64 ThingUID; };
class CThing { public:
    UID64 UniqueID;
    bool Dead;
    CTCThingUIDDummyForSerialisation* Dummy;
    UID64 GetUniqueID() const { return UniqueID; }
    bool GetTC(CTCThingUIDDummyForSerialisation** out) { *out = Dummy; return Dummy != 0; }
};
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
class CThingManager { public:
    int remapCalls;
    bool lastSaving;
    UID64 RemapUIDForSerialisation(bool saving, bool, unsigned long level, UID64 uid)
    {
        ++remapCalls;
        lastSaving = saving;
        return uid * 10 + level;
    }
};
class CPersistContext { public:
    bool Saving;
    UID64 Stored;
    bool IsSaving() const { return Saving; }
    void Transfer(const char*, UID64& value, const UID64&)
    {
        if (Saving) Stored = value; else value = Stored;
    }
};
static CThingManager g_manager;
static CTCThingUIDDummyForSerialisation g_dummy;
static CThing g_created;
static int g_creates;
static CThing* CreateDummyThing() { ++g_creates; g_created.Dummy = &g_dummy; return &g_created; }

static void GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer& pointer, unsigned long level)
{
    if (context.IsSaving()) {
        UID64 uid = 0;
        CThing* thing = pointer.GetPItem();
        if (thing != 0 && !thing->Dead) {
            uid = thing->GetUniqueID();
            uid = g_manager.RemapUIDForSerialisation(true, false, level, uid);
        }
        UID64 zero = 0;
        context.Transfer(name, uid, zero);
    }
    else {
        UID64 uid = 0;
        UID64 zero = 0;
        context.Transfer(name, uid, zero);
        if (uid != 0) {
            uid = g_manager.RemapUIDForSerialisation(false, false, level, uid);
            CThing* thing = CreateDummyThing();
            CTCThingUIDDummyForSerialisation* dummy = 0;
            thing->GetTC(&dummy);
            dummy->ThingUID = uid;
            pointer.Item = thing;
        }
    }
}

int main()
{
    int ok = 1;
    CThing live = { 7, false, 0 };
    CThing dead = { 9, true, 0 };
    CPersistContext save = { true, 123 };
    CIntelligentPointer p = { &live };
    GFSerialisePointer(save, "P", p, 3);
    ok &= save.Stored == 73 && g_manager.lastSaving;
    p.Item = &dead;
    GFSerialisePointer(save, "P", p, 3);
    ok &= save.Stored == 0;
    p.Item = 0;
    save.Stored = 55;
    GFSerialisePointer(save, "P", p, 3);
    ok &= save.Stored == 0 && g_manager.remapCalls == 1;
    CPersistContext load = { false, 0 };
    CIntelligentPointer q = { 0 };
    GFSerialisePointer(load, "P", q, 2);
    ok &= q.Item == 0 && g_creates == 0;
    load.Stored = 4;
    GFSerialisePointer(load, "P", q, 2);
    ok &= q.Item == &g_created && g_creates == 1 && g_dummy.ThingUID == 42 && !g_manager.lastSaving;
    if (!ok) { printf("0088ae40_TEST FAIL\n"); return 1; }
    printf("0088ae40_TEST PASS\n");
    return 0;
}