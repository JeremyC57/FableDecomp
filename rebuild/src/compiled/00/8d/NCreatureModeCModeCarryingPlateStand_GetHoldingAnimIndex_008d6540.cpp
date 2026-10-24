// NCreatureMode::CModeCarryingPlateStand::GetHoldingAnimIndex() const -- the bank index of the creature's "ST_HOLDING_PLATE" animation
// Leaf: ?GetHoldingAnimIndex@CModeCarryingPlateStand
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6540, 51 bytes (+ int3 padding), `ret`; vtable 0x01267AA0 slot 12.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("ST_HOLDING_PLATE" @ 0x0128565C),
//   CThingCreatureBase::GetAnimFromName(const CCharString&, long) @ 0x00662A00, ~CCharString @ 0x0099EAE0.
// Name: FableWin.pdb `?GetHoldingAnimIndex@CModeCarryingPlateStand@NCreatureMode@@MBEJXZ` (protected const virtual; the
//   CModeCarryingWeaponStand override @ 0x008E3600 returns -1).
// Layout: CModeBase ParentThing +0x04; CAnimationEntry (Ego_r.pdb, 0x10) BankIndex +0.
// Shape: the literal's CCharString temporary is built before the `0` push (a literal bound to a const CCharString&
//   parameter is converted before the other arguments); ParentThing is a named local
//   read into esi before the conversion call (the direct member form keeps `this` in esi and reloads it after), and the
//   index survives the temporary's dtor in esi.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CAnimationEntry {                   // Ego_r.pdb CAnimationEntry (0x10)
    long BankIndex;                        // +0
    unsigned char _rest[0xc];              // AnimName, GroupName, Components
};

struct CThingCreatureBase {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
};

namespace NCreatureMode {
struct CModeCarryingPlateStand : CModeBase {   // vtable 0x01267AA0
    long GetHoldingAnimIndex() const;
};
}

long NCreatureMode::CModeCarryingPlateStand::GetHoldingAnimIndex() const
{
    CThingCreatureBase* creature = ParentThing;
    return creature->GetAnimFromName("ST_HOLDING_PLATE", 0)->BankIndex;
}