// CAIStateGroup_VillageTaskPatrol::IsRunnable() -- runnable when the task's village lists this creature as a patroller
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093EDF0, length 65 bytes (+ int3 padding).
// Callees: GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component pointer at
// +0x14: the component's village thing (+0x10) interface 0x22, or null; the manifest's
// IS_THING_ALIVE label is propagated),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the pointer at +0x18), virtual slot 10 (+0x28)
// on this when that item exists, CTCVillage::HasPatroller(CThing*) @ 0x00658E90 (forwards to the
// member list at +0x54; "_Umove" is a propagated label) with PBrain->Creature.
// Shape: nested if with the shared `xor al,al` false tail at the end (an early `return false`
// duplicates the epilogue inline); the village interface is kept in edi; the slot-10 call is a
// side effect guarded by the pointer at +0x18; the final `!= 0` of the long result is the
// `neg ; sbb ; neg` idiom (int expression converted to the bool return).
class CThing;

class CTCVillage { public:
    long HasPatroller(CThing* creature);        // 0x00658E90
};

struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void OnTaskThingFound();            // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_VillageTaskPatrol : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14 the creature's villager component
    CIntelligentPointer<CThing> PTaskThing;         // +0x18 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_VillageTaskPatrol::IsRunnable()
{
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    if (village != 0) {
        if (PTaskThing.GetPItem() != 0)
            OnTaskThingFound();
        CThing* creature = PBrain->Creature;
        return village->HasPatroller(creature) != 0;
    }
    return false;
}