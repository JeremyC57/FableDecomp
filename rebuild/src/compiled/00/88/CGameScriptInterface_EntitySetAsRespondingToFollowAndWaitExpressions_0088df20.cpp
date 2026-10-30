// CGameScriptInterface::EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const -- marks the creature as responding to follow-and-wait expressions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DF20, length 64 bytes.
// Callees: CScriptThing virtual slot 75 (+0x12c, IsValid) and slot 11 (+0x2c, GetThing); then the thing's byte at +0x10 must be 1
// (a creature) and bit 2 of the flag byte at +0x1c9 is assigned (`shl cl,2 ; xor ; and 4 ; xor`).
// Oracle name: ?EntitySetAsRespondingToFollowAndWaitExpressions@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (`UBE` = virtual const; `this` is unused, so the
// method is written on the PDB-header wrapper and selected by its leaf).
// Leaf: EntitySetAsRespondingToFollowAndWaitExpressions
#include "engine/CGameScriptInterface.h"

class CThing { public:
    unsigned char _pad[0x10];
    unsigned char m_thingKind;                      // +0x10 (1 = creature)
    unsigned char _pad11[0x1b8];
    unsigned char m_followBit0 : 1;                 // +0x1c9 allowed to follow hero
    unsigned char m_followBit1 : 1;                 //        NOT allowed to change region following state
    unsigned char m_followBit2 : 1;                 //        responds to follow-and-wait expressions
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const;
};

void CGameScriptInterface_Methods::EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& thing, bool flag) const
{
    if (thing.IsAlive()) {
        CThing* object = thing.GetPThing();
        if (object->m_thingKind == 1)
            object->m_followBit2 = flag;
    }
}