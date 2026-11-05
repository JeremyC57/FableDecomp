// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::CMurderTwistSheriffGuard(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me) -- script helper-object constructor: the owner's interface at +4, the base vtable, a copy of the thing at +8, the owner at +0x14, own vtable, an empty scripted-thing handle at +0x1c and an empty name at +0x34, then the index at +0x18
// Leaf: ??0CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB2000, length 88 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x0099A380 (the handle's base), CCharString::CCharString() @ 0x0099E4B0.
// Vtables 0x012C3224 (the base) and 0x012EFEE8 (this class) have no RTTI locator (quest-script module); the
// chunk label "CV_MurderTwistFinalScript::CV_MurderTwistFinalScript" is propagated (five constructors carry it),
// so the classes are named by their vtables.
// Shape: `[esi+4] = owner->m_pInterface` (+0x40 of the owner) is the base constructor's LAST argument (evaluated
// first, right to left, before the thing reference is loaded), a plain initialiser of the base (vtable
// 0x012C3224) whose store is scheduled before the base's vtable store (wave 24i: plain initialisers and body
// assignments sit above the vtable store, member objects below it); the base copies the thing into +8 through
// the out-of-line copy constructor; the owner at +0x14 is likewise a plain initialiser of this class stored
// before its vtable; the two member objects follow the vtable store (the handle's two words zeroed by an embedded
// {Data, Info} member object's inline constructor, else the name's `lea` hoists above the handle's vtable store); the index at +0x18 is stored LAST, after
// the name's constructor, i.e. a body assignment; `ret 0Ch`, `this` in eax. The name sits at +0x34 (8 bytes
// after the 16-byte handle: padding of unknown use).
// Naming wave 35: identity reg 00eaeacd "MurderTwistSheriffGuard" in 00eaea20; Alloc 00eb1f80; vtable 012efee8; called by the Alloc after operator new; PDB ??0CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@; refs(call)=Alloc; Anniversary RTTI agrees (confirmed); was CVtable_012efee8::CVtable_012efee8 (wave3/labels_alloc.tsv); PDB ??0CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z in FableWin.pdb.
//   Was `??0CVtable_012efee8@@QAE@PAUCScriptOwner@@JABVCScriptThing@@@Z`: the parent stand-in CScriptOwner is the PDB outer NScript::CV_MurderTwistFinalScript and the class is nested in it with the PDB
//   parameter types (the parent by reference, as the PDB spells it; same pointer ABI); same bytes.
class CScriptDataBase;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

namespace NScript {
class CV_MurderTwistFinalScript {
public:                              // the quest script owning this helper
    unsigned char _pad00[0x40];
    void*         m_pInterface;                    // +0x40
public:
    class CMurderTwistSheriffGuard;  // the entity class below (naming wave 35)
};
}
using NScript::CV_MurderTwistFinalScript;

struct CVtable_012c3224 {                          // vtable 0x012C3224
    virtual void v00();
    void*        m_pInterface;                     // +0x04 (a plain initialiser: its store is scheduled before the vtable store)
    CScriptThing m_thing;                          // +0x08
    CVtable_012c3224(const CScriptThing& thing, void* iface) : m_pInterface(iface), m_thing(thing) { }   // (thing, iface): the interface argument is evaluated first (right to left)
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

namespace NScript {
class CV_MurderTwistFinalScript::CMurderTwistSheriffGuard : public CVtable_012c3224 {
public:       // vtable 0x012EFEE8
    CV_MurderTwistFinalScript* m_pOwner;                        // +0x14 (a plain initialiser: stored before this class's vtable)
    long          m_index;                         // +0x18 (body assignment: stored last)
    CScriptGameResourceObjectScriptedThingBase m_scriptedThing;   // +0x1c
    unsigned char _pad2c[8];                       // +0x2c..+0x33
    CCharString   m_name;                          // +0x34
    virtual void v00();
    CMurderTwistSheriffGuard(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
}

NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::CMurderTwistSheriffGuard(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CVtable_012c3224(me, parent.m_pInterface), m_pOwner((&parent)), m_scriptedThing(), m_name()
{
    m_index = ((long)data);
}