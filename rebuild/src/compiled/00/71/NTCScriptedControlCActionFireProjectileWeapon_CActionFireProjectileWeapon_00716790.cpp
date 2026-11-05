// NTCScriptedControl::CActionFireProjectileWeapon::CActionFireProjectileWeapon(CThing* target) -- constructor: CBase(), the common action members zeroed, own vtable, target CScriptThing, an empty action list, action index -1
// Leaf: ??0CActionFireProjectileWeapon
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00716790, length 74 bytes (ret 4).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CThing::GetScriptThing @ 0x004C7E50 (by-value CScriptThing through the hidden
// result pointer, constructed straight into the member at +0xc), malloc @ 0x00BFEA0E (the list's sentinel node).
// Vtable 0x01265960 = NTCScriptedControl::CActionFireProjectileWeapon (rtti-vtables.tsv); the only caller is the
// scripted-control factory at 0x00903DF6. The catalog label "CTCTargeted::CTCTargeted" is propagated and rejected.
// Ego_r.pdb CActionFireProjectileWeapon (0x28): PTarget (CScriptThing) +0x0c, ActionList (list<CCountedPointer<
// CCreatureActionBase>>) +0x18, CurrentActionIndex +0x24 -- the retail list is a single sentinel-node pointer (4 bytes,
// node 0x10 = next, prev, 8-byte counted pointer), so CurrentActionIndex sits at +0x1c in retail.
// Shape: the landed CActionPickUpGenericBox / CActionFollowCreature model (novtable CAction intermediate, zero stores
// sharing ebx, the vtable store sunk to just before GetScriptThing) plus the inline sentinel-list constructor of the
// landed 0x00835370 family (head = 0, node linked to itself, then head = node).
extern "C" void* __cdecl malloc(unsigned int size);

class CThing;

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

struct __declspec(novtable) CAction : CBaseClassNonCopyable {   // NTCScriptedControl::CActionBase: abstract, its own vtable store is dead
    void*   PTCScript;                      // +0x04 CTCScriptedControl*
    bool    Activated;                      // +0x08
    bool    Terminated;                     // +0x09
    CAction() : CBaseClassNonCopyable(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionList {                        // retail std::list<CCountedPointer<CCreatureActionBase> >: sentinel pointer only
    struct node { node* next; node* prev; void* object; void* info; };
    node* head;
    CActionList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CActionFireProjectileWeapon : CAction {   // vtable 0x01265960
    CScriptThing PTarget;                   // +0x0c
    CActionList  ActionList;                // +0x18
    long         CurrentActionIndex;        // +0x1c
    CActionFireProjectileWeapon(CThing* target);
};

CActionFireProjectileWeapon::CActionFireProjectileWeapon(CThing* target)
    : CAction()
    , PTarget(target->GetScriptThing())
    , ActionList()
    , CurrentActionIndex(-1)
{
}