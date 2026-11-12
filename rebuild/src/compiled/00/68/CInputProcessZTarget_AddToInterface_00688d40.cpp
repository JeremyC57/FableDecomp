// CInputProcessZTarget::AddToInterface(CInputProcessManager& manager) -- entering Z-target mode: latch the player's controlled creature into PCreature; when it is alive, cache its hero combat def and, if it has a targeting component whose targeted thing is alive, latch that thing into PTarget; reset DeactivateHoldTimer to -1 and chain to the base AddToInterface
// Leaf: ?AddToInterface@CInputProcessZTarget@@UAE_NAAVCInputProcessManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688D40, length 360 bytes, `ret 4`, no frame pointer (this = ebp), no pragma / flag.
// Identity: Ego_r.pdb ?AddToInterface@CInputProcessZTarget@@UAE_NAAVCInputProcessManager@@@Z (Ego_r 0x006B4480, between Update and
//   ProcessInput as in retail 0x00688C40 / 0x00688EB0); rtti-vtables CInputProcessZTarget vtable 0x01238028 slot 1 = 0x00688D40.
// Callees: CPlayer::PeekPControlledCreature @ 0x00487DD0; CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90,
//   GetPItem @ 0x00A01B50, dtor @ 0x00A01C10 (the two temporaries of the inline CIntelligentPointer(T*) constructor + operator=,
//   vtables 0x01237F14 <const CThingPlayerCreature> and 0x01237F1C <const CThing>); CThingCreatureBase::GetPHeroCombatDef
//   @ 0x00663340 (CDefPointer through the hidden result pointer); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const)
//   @ 0x004365B0 (the expanded CThing::PeekTC<CTCTargetingBase>, TCI_TARGETING = 8, `test ch,1` on UsingComponents);
//   CTCTargetingBase::GetPTargetedThing (virtual slot 33, +0x84); CInputProcessGameBase::AddToInterface @ 0x00687A70.
// Layout: engine/CInputProcessZTarget.h (Ego_r.pdb): MyPlayer +0x18, PCreature +0x28, PHeroCombatDef +0x30, DeactivateHoldTimer +0x34,
//   PTarget +0x38; CThing Dead = bit 0 of +0x91.
// Shape: IS_THING_ALIVE(PCreature) reads the creature twice through GetPItem (null test, then IsDead); operator-> reads it again
//   for the def call and for PeekTC. The CDefPointer copy assignment re-reads the temporary's Object after the release; the
//   temporary's own destructor is the load / dec / store release. Everything comes from the shared headers; the three
//   AddToInterface declarations are the header opt-in FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CInputProcessZTarget.h"

bool CInputProcessZTarget::AddToInterface(CInputProcessManager& manager)
{
    PCreature = MyPlayer->PeekPControlledCreature();
    if (IS_THING_ALIVE(PCreature)) {
        PHeroCombatDef = PCreature->GetPHeroCombatDef();
        const CTCTargetingBase* targeting;
        if (PCreature->PeekTC(&targeting)) {
            CThing* target = targeting->GetPTargetedThing();
            if (IS_THING_ALIVE(target))
                PTarget = target;
        }
    }
    DeactivateHoldTimer = -1;
    return CInputProcessGameBase::AddToInterface(manager);
}