// CThingManager::SaveThingsOnLevelReadyForUnloading(long mapNumber, bool savePersistent, vector<CThing*>* toDelete) -- for every thing on the unloading map: park it in limbo, or queue it for deletion and save its persistent state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005252B0, length 310 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CThing virtual slot 42 (+0xa8, the limbo preparation, vtable 0x01239ABC slot 42 = 0x004CD280),
// CThing::SetInLimbo(bool) @ 0x004C8CF0, CQuestManager::OnDeactivateEntity(CThing*, bool) @ 0x004AFB00 on
// g_pQuestManager @ 0x013B89FC, vector<CThing*>::push_back @ 0x00436C80 (out of line, landed as the 4-byte-element
// push_back), CThing::IsGameStatePersisted @ 0x004C8280, CThing::IsLevelPersistent @ 0x004C82A0, the persistent-state
// writer @ 0x005239C0 (catalog "Thing_SavePersistentState"), free @ 0x00BFEA14 (the two local vectors' storage).
// Layout: CThing (retail = Ego_r.pdb - 0x10): CurrentMapNumber +0x8e; bits +0x91 Dead(0) GloballyLoaded(4) InLimbo(5);
// +0x92 DeletedOnOwnerLevelOutOfScope(0) PersistOnUnloadWhenInLimbo(6); +0x93 KillOnLevelUnload(2).
// CThingManager Things list sentinel +0x18 (the landed SaveToScript model).
// Shape: two local vectors (game-state first, level-persistent second: zeroed in that order, destroyed in reverse); the
// thing is a named local homed in the dead `mapNumber` slot because push_back takes it by const reference.
#include <new>
class CThing;
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct CThingVector {                                          // retail std::vector<CThing*>: first, last, end
    CThing** first;
    CThing** last;
    CThing** end;
    CThingVector() : first(0), last(0), end(0) { }
    ~CThingVector() { if (first) free(first); }
    void InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(CThing* const& value);                      // 0x00436C80
};

void* GrowVectorStorage_00436ac0(unsigned long count);
#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line push_back 0x00436C80 (its landed body) and its growth template 0x00436AC0
// (reads the value only through its const reference, never touches the empty tag, keeps the vector to itself):
// retail's LTCG knew the value is only read, so the caller keeps the thing in esi next to its homed copy.
__declspec(noinline) void CThingVector::InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    CThing** p = (CThing**)GrowVectorStorage_00436ac0(count);
    if (p) {
        *p = value;
        first = p;
        last = p + 1;
        end = p + count;
    }
}
__declspec(noinline) void CThingVector::push_back(CThing* const& value)
{
    if (last != end) {
        new (last) CThing*(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
#pragma auto_inline(on)

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void PrepareForLimbo();                            // slot 42 (+0xa8) 0x004CD280
    unsigned char _pad04[0x8e - 0x04];
    short         CurrentMapNumber;                            // +0x8e
    signed char   PlayerNumber;                                // +0x90
    unsigned char Dead : 1;                                    // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;                          // bit 4
    unsigned char InLimbo : 1;                                 // bit 5
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;           // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;              // bit 6
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;                // +0x93 bit 0
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;                       // bit 2
    void SetInLimbo(bool inLimbo);                             // 0x004C8CF0
public:
    bool IsGameStatePersisted() const;   // 0x004C8280
    bool IsLevelPersistent() const;   // 0x004C82A0
};

class CQuestManager {
public:
    void OnDeactivateEntity(CThing* thing, bool deactivate);   // 0x004AFB00
};
extern CQuestManager* g_pQuestManager;                         // 0x013B89FC

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;
};

class CThingManager {
public:
    void*           __vftable;                                 // +0x00
    unsigned char   _pad04[0x18 - 0x04];
    SThingListNode* Things;                                    // +0x18 list<CThing*> sentinel
    void SavePersistentState(long mapNumber, CThingVector& things, bool gameState);   // 0x005239C0
    void SaveThingsOnLevelReadyForUnloading(long mapNumber, bool savePersistent, CThingVector* toDelete);
};

// R&D I-3 sweep: a same-TU out-of-line SavePersistentState (it only reads the vector it is handed) lets `thing` share
// the dead mapNumber argument slot, as in retail. The per-thing save is the declaration-only helper below.
void SaveThingPersistentState(CThing* thing, long mapNumber, bool gameState);
#pragma auto_inline(off)
void CThingManager::SavePersistentState(long mapNumber, CThingVector& things, bool gameState)
{
    for (CThing** it = things.first; it != things.last; ++it)
        SaveThingPersistentState(*it, mapNumber, gameState);
}
#pragma auto_inline(on)
void CThingManager::SaveThingsOnLevelReadyForUnloading(long mapNumber, bool savePersistent, CThingVector* toDelete)
{
    CThingVector gameStateThings;
    CThingVector levelThings;
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->CurrentMapNumber != mapNumber)
            continue;
        if (!thing->KillOnLevelUnload) {
            thing->PrepareForLimbo();
            thing->SetInLimbo(true);
            g_pQuestManager->OnDeactivateEntity(thing, true);
        } else if (!thing->GloballyLoaded && (thing->PersistOnUnloadWhenInLimbo || !thing->InLimbo)
                   && thing->DeletedOnOwnerLevelOutOfScope) {
            toDelete->push_back(thing);
            if (savePersistent && !thing->Dead) {
                if (thing->IsGameStatePersisted())
                    gameStateThings.push_back(thing);
                else if (thing->IsLevelPersistent())
                    levelThings.push_back(thing);
            }
        }
    }
    SavePersistentState(mapNumber, gameStateThings, true);
    SavePersistentState(mapNumber, levelThings, false);
}