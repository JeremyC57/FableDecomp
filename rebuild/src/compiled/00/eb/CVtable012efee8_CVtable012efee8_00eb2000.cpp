// CVtable_012efee8::CVtable_012efee8(CScriptOwner* owner, long index, const CScriptThing& thing) -- script helper-object constructor: the owner's interface at +4, the base vtable, a copy of the thing at +8, the owner at +0x14, own vtable, an empty scripted-thing handle at +0x1c and an empty name at +0x34, then the index at +0x18
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
class CScriptThing { public:                              // 12 bytes: vptr, PImp_Data, PImp_Info
    unsigned char _body[0xc];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
};

struct CScriptOwner {                              // the quest script owning this helper
    unsigned char _pad00[0x40];
    void*         m_pInterface;                    // +0x40
};

struct CVtable_012c3224 {                          // vtable 0x012C3224
    virtual void v00();
    void*        m_pInterface;                     // +0x04 (a plain initialiser: its store is scheduled before the vtable store)
    CScriptThing m_thing;                          // +0x08
    CVtable_012c3224(const CScriptThing& thing, void* iface) : m_pInterface(iface), m_thing(thing) { }   // (thing, iface): the interface argument is evaluated first (right to left)
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

struct CCountedPointerWords {                     // {Data, Info}: a member object so its zero stores follow the owner's vtable store
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
};

class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct CVtable_012efee8 : CVtable_012c3224 {       // vtable 0x012EFEE8
    CScriptOwner* m_pOwner;                        // +0x14 (a plain initialiser: stored before this class's vtable)
    long          m_index;                         // +0x18 (body assignment: stored last)
    CScriptGameResourceObjectScriptedThingBase m_scriptedThing;   // +0x1c
    unsigned char _pad2c[8];                       // +0x2c..+0x33
    CCharString   m_name;                          // +0x34
    virtual void v00();
    CVtable_012efee8(CScriptOwner* owner, long index, const CScriptThing& thing);
};

CVtable_012efee8::CVtable_012efee8(CScriptOwner* owner, long index, const CScriptThing& thing)
    : CVtable_012c3224(thing, owner->m_pInterface), m_pOwner(owner), m_scriptedThing(), m_name()
{
    m_index = index;
}