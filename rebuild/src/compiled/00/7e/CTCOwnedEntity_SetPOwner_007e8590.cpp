// CTCOwnedEntity::SetPOwner(CThing* owner) -- hand the parent thing from its old owner's CTCThingOwner to the new owner's
// (creating the new owner's CTCThingOwner if it has none), tell the components, drop / add the switchable-navigation TC (a
// "Dress..." thing in a buyable house gets one), and copy the owner's UID, drawable and limbo state onto the parent thing.
// Retail 0x007E8590, 650 bytes, __fastcall `ret 4`, /O2, no SEH.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020, CTCThingOwner::RemoveOwnedObject
// 0x0071B1B0 / AddOwnedObject 0x0071B300, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, CThing::SendMessageW 0x004C7820,
// CThing::RemoveTC 0x004C9840, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~ 0x00A01C10, CCharString 0x0099EBF0 /
// ~CCharString 0x0099EAE0, CThing::AddTC 0x004C9D60, CThing::SetInLimbo 0x004C8CF0, CThing slots 27/28 DrawIsDrawable /
// SetAsDrawable, _strnicmp 0x00F3FB90. Message vtable 0x01270BF0 (NTCMessages::CMessageOnSetThingOwner).
// Leaf: ?SetPOwner@CTCOwnedEntity@@QAEXPAVCThing@@@Z
// Spellings the retail bytes require (each checked by grading the plain alternative):
// - GetTC's bool result is ignored on purpose, as retail does: when the owner has no CTCThingOwner bit the out-pointer is left
//   unset and retail reads the uninitialised local anyway (0x007E85E7 `mov eax,[esp+20h]`, then RemoveOwnedObject on it).
// - `GetParentThing().SetAsDrawable(...)` (reference accessor) and not `ParentThing->`: retail reloads the parent into ebp after
//   the GetPItem call (0x007E871F `mov ebp,[esi+4]` ; `mov ebx,[ebp]` ; DrawIsDrawable ; `mov ecx,ebp`). The `ParentThing->`
//   spelling grades DIFFER(650v650) with ebx/ebp swapped through the rest of the body.
// - The "CTCSwitchableNavigation" name lives in its own block: retail destroys it before setting the flag (AddTC 0x007E87FE,
//   ~CCharString 0x007E8807, then 0x007E880C `mov byte ptr [esi+0Ch],1`). Without the block the flag store moves ahead of the
//   destructor call (DIFFER at 0x007E8807/0x007E880C); a CCharString temporary in the AddTC call grades DIFFER(646v650).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITHCASEINSENSITIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CTHING_GETSCRIPTDATA
#define FABLE_RETAIL_CTHING_SENDMESSAGEW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCTHINGOWNER_OWNEDOBJECTS
#include "fable_string.h"
#include "engine/CBaseClass.h"
#include "engine/CVectorMap.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCThingOwner.h"
class CTCParamsBase;

namespace NTCMessages {
// No shared header: Ego_r.pdb CMessageBase : CBaseClass (no members); CMessageOnSetThingOwner : CMessageBase, Owner CThing* +4.
class CMessageBase : public CBaseClass {
public:
    virtual unsigned long GetType() const;                      // slot 1 (FableWin ?GetType@CMessageBase@NTCMessages@@UBEKXZ)
};
class CMessageOnSetThingOwner : public CMessageBase {           // vtable 0x01270BF0: slot 0 dtor 0x007E8350, slot 1 GetType 0x007E8330
public:
    CMessageOnSetThingOwner(CThing* owner) : Owner(owner) { }
    virtual unsigned long GetType() const { return 4; }        // slot 1 0x007E8330 `mov eax,4 ; ret` (rtti-vtables 01270bf0)
    CThing* PeekOwner() const { return Owner; }
private:
    CThing* Owner;                                              // +0x04
};
}

void CTCOwnedEntity::SetPOwner(CThing* owner)
{
    if (GetPOwner()) {
        CTCThingOwner* thingOwner;
        GetPOwner()->GetTC(TCI_THING_OWNER, &thingOwner);
        thingOwner->RemoveOwnedObject(*ParentThing);
    }
    ParentThing->SendMessageW(NTCMessages::CMessageOnSetThingOwner(owner));
    if (SwitchableNavigationTCAdded) {
        ParentThing->RemoveTC(TCI_SWITCHABLE_NAVIGATION);
        SwitchableNavigationTCAdded = false;
    }
    POwner = CIntelligentPointer<CThing>(owner);
    if (GetPOwner() && !GetPOwner()->IsDead()) {
        OwnerUID = GetPOwner()->GetUniqueID();
        CTCThingOwner* thingOwner;
        if (!GetPOwner()->GetTC(TCI_THING_OWNER, &thingOwner)) {
            CCharString name("CTCThingOwner");
            thingOwner = (CTCThingOwner*)GetPOwner()->AddTC(name, false, 0);
        }
        thingOwner->AddOwnedObject(*ParentThing);
        GetParentThing().SetAsDrawable(GetPOwner()->DrawIsDrawable());
        ParentThing->SetInLimbo(GetPOwner()->IsInLimbo());
        if (!ParentThing->HasTC(TCI_SWITCHABLE_NAVIGATION) && GetPOwner()->HasTC(TCI_BUYABLE_HOUSE) &&
            ParentThing->GetType() == 5 && ParentThing->GetScriptData().StartsWithCaseInsensitive("Dress")) {
            {
                CCharString name("CTCSwitchableNavigation");
                ParentThing->AddTC(name, false, 0);
            }
            SwitchableNavigationTCAdded = true;
        }
    }
}