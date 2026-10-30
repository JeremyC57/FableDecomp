// CTCInventoryItem::AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool levelPersistent) -- sends the owner an OnPickup message, asks the inventory to take the owner thing; on success remembers the inventory and sets the owner's level-persistence (persisted + death-persistent, or not persisted); the owner thing is then killed (it lives on inside the inventory); returns whether it was taken
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8D50, length 208 bytes (+ int3 padding); speed TU (no frame pointer, `mov dword ptr [..],imm` stores).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 and ~CBaseClass @ 0x0099A2E0 around the NTCMessages::CMessageOnPickup temporary (vtable
// 0x012554E0; its implicit destructor stores no vtable), CThing::SendMessage(CMessageBase*) @ 0x004C7820, CTCInventoryBase virtual slot 80
// (+0x140, AddItemToInventory(CThing*, bool, bool, long, bool) @ 0x005BF654 in the clothing vtable), CThing::SetAsGameStatePersisted(bool)
// @ 0x004C7880, CThing::Kill(bool) @ 0x004C9B80.
// Layout: engine/CTCInventoryItem.h (PDB) names +0x04 `slot`; it is the CTCBase ParentThing (the message and the kill go through it),
// PInventory +0x10. CThing flag bytes at retail +0x92 (LevelPersistent bit 4) and +0x93 (DeathPersistent bit 1), PDB +0x10 higher.
// Shape: the owner thing is read into a local that dies at the call (so it is loaded into eax before the four pass-through
// arguments are re-pushed as dwords through ecx), later uses re-read the member; the result rides in bl (`xor bl,bl` / `mov bl,1`);
// both arms end in their own Kill + epilogue (the persistent arm falls out of the `if`, the other arms share the tail).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
namespace NTCMessages {
class CMessageOnPickup : public CBaseClass { public:             // vtable 0x012554E0
    virtual void v01();                            // slot 1 @ 0x005D8CB0
};
}
class CThing { public:
    unsigned char _pad00[0x92];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;                 // +0x92 bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;        // +0x93 bit 0
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;               // +0x93 bit 2
    unsigned char ActivatedScript : 1;
    unsigned char _93_4 : 4;
    void SendMessage(CBaseClass* message);             // 0x004C7820
    void SetAsGameStatePersisted(bool persisted);      // 0x004C7880
    void Kill(bool flag);                              // 0x004C9B80
};
struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual bool AddItemToInventory(CThing* thing, bool a, bool b, long c, bool d);   // slot 80 (+0x140)
};
class CTCInventoryItem { public:                              // CTCBase-derived (engine/CTCInventoryItem.h offsets)
    void*             __vftable;                       // +0x00
    CThing*           ParentThing;                     // +0x04
    long              field8;                          // +0x08
    void*             PDef_Object;                     // +0x0c
    CTCInventoryBase* PInventory;                      // +0x10
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