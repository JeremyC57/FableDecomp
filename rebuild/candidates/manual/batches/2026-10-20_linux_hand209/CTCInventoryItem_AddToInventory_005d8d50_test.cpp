// Behaviour test for CTCInventoryItem::AddToInventory @ 0x005D8D50 (model copy, stub thing and inventory).
#include <cstdio>
struct CBaseClass {
    CBaseClass() { ++constructed; }
    ~CBaseClass() { ++destructed; }
    virtual void v00() {}
    static long constructed, destructed;
};
long CBaseClass::constructed = 0; long CBaseClass::destructed = 0;
namespace NTCMessages { struct CMessageOnPickup : CBaseClass { virtual void v01() {} }; }
struct CThing {
    unsigned char _pad00[0x92];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; unsigned char Drawable : 1; unsigned char Dying : 1; unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1; unsigned char GameStatePersisted : 1; unsigned char PersistOnUnloadWhenInLimbo : 1; unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1; unsigned char DeathPersistent : 1; unsigned char KillOnLevelUnload : 1; unsigned char ActivatedScript : 1; unsigned char _93_4 : 4;
    long messages; long persistedCalls; bool lastPersisted; long kills; bool lastKillFlag;
    void SendMessage(CBaseClass* message) { if (message) ++messages; }
    void SetAsGameStatePersisted(bool persisted) { ++persistedCalls; lastPersisted = persisted; }
    void Kill(bool flag) { ++kills; lastKillFlag = flag; }
};
struct CTCInventoryBase {
    bool accept; CThing* seenThing; bool seenA, seenB, seenD; long seenC; long adds;
    virtual bool AddItemToInventory(CThing* thing, bool a, bool b, long c, bool d) { ++adds; seenThing = thing; seenA = a; seenB = b; seenC = c; seenD = d; return accept; }
};
struct CTCInventoryItem {
    void* __vftable; CThing* ParentThing; long field8; void* PDef_Object; CTCInventoryBase* PInventory;
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool levelPersistent);
};
bool CTCInventoryItem::AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool levelPersistent)
{
    {
        NTCMessages::CMessageOnPickup message;
        ParentThing->SendMessage(&message);
    }
    bool added = false;
    CThing* thing = ParentThing;
    if (inventory->AddItemToInventory(thing, a, b, c, d)) {
        PInventory = inventory;
        added = true;
        if (levelPersistent) {
            ParentThing->LevelPersistent = 1;
            ParentThing->SetAsGameStatePersisted(true);
            ParentThing->DeathPersistent = 1;
        } else {
            ParentThing->LevelPersistent = 0;
            ParentThing->SetAsGameStatePersisted(false);
        }
    }
    ParentThing->Kill(false);
    return added;
}
static void ResetThing(CThing& t) { t.LevelPersistent = 1; t.DeathPersistent = 0; t.Drawable = 1; t.messages = 0; t.persistedCalls = 0; t.kills = 0; t.lastKillFlag = true; }
int main()
{
    CThing thing; ResetThing(thing);
    CTCInventoryBase inv; inv.accept = true; inv.adds = 0;
    CTCInventoryItem item; item.ParentThing = &thing; item.PInventory = 0;
    bool ok = true;
    // accepted + level persistent: flags set, persisted, death persistent, killed
    bool r = item.AddToInventory(&inv, true, false, 77, true, true);
    if (!r || item.PInventory != &inv || inv.adds != 1 || inv.seenThing != &thing || !inv.seenA || inv.seenB || inv.seenC != 77 || !inv.seenD) ok = false;
    if (!thing.LevelPersistent || !thing.DeathPersistent || thing.persistedCalls != 1 || !thing.lastPersisted || thing.kills != 1 || thing.lastKillFlag) ok = false;
    if (thing.messages != 1 || CBaseClass::constructed != 1 || CBaseClass::destructed != 1 || !thing.Drawable) ok = false;
    // accepted + not level persistent: flag cleared, not persisted, death persistence untouched
    ResetThing(thing); item.PInventory = 0;
    r = item.AddToInventory(&inv, false, true, 5, false, false);
    if (!r || item.PInventory != &inv || thing.LevelPersistent || thing.DeathPersistent || thing.persistedCalls != 1 || thing.lastPersisted || thing.kills != 1) ok = false;
    // refused: nothing changes but the message and the kill
    ResetThing(thing); item.PInventory = 0; inv.accept = false;
    r = item.AddToInventory(&inv, true, true, 1, true, true);
    if (r || item.PInventory != 0 || !thing.LevelPersistent || thing.DeathPersistent || thing.persistedCalls != 0 || thing.kills != 1 || thing.messages != 1) ok = false;
    if (!ok) { std::printf("005d8d50_TEST FAIL\n"); return 1; }
    std::printf("005d8d50_TEST PASS\n");
    return 0;
}
