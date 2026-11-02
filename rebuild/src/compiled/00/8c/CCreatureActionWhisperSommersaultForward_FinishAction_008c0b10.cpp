// CCreatureAction_WhisperSommersaultForward::FinishAction() -- if the creature still carries thing component 77, remove it; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_WhisperSommersaultForward@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0B10, length 47 bytes (byte twin of LeapLong 0x008B74A0); vtable 0x01296754 slot 4 (the only reference).
// Name: Ego_r + FableWin ?FinishAction@CCreatureAction_WhisperSommersaultForward@@UAEXXZ (public virtual; the base CCombatActionBase::FinishAction is
// protected `MAE`); donor order Initialise / FinishAction / PerformAction (both PDBs); retail FinishAction 0x008C0B10 sits just before PerformAction 0x008C0B40.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, called twice), CThing::RemoveTC(ETCInterfaceType) @ 0x004C9840
// (both PDBs), CCombatActionBase::FinishAction @ 0x00859A20 (tail jump).
// Shape: `test ch,20h` on +0x28 is the inline CThing::HasTC(77): the present-TC bitset starts at +0x20, 77 >> 5 = dword 2, bit 13.
// Component 77 (0x4D) is CTCSolidToThing: the only CTC vtable whose GetInterfaceType (slot 21) is `mov eax,4Dh ; ret` is
//   0x0126A62C (RTTI CTCSolidToThing, body 0x00770FF0).
// The intelligent pointer's address is kept in esi; `this` lives in edi for the base tail call.
// Model: CThing : CClass : CBaseObject (RTTI) from engine/CBaseObject.h (retail opt-in) and engine/CClass.h under the layout-rename
//   include (the generated CClass.h flattens the vptr); CClass is 0x14 bytes unpacked, so CThing's own fields start at +0x14.
enum ETCInterfaceType { };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // CBaseObject : CBaseClass (IntelligentPointer +4)
#define CClass CClassFields                             // engine/CClass.h is flattened (vptr as data): layout-rename include
#include "engine/CClass.h"
#undef CClass

class CClass : public CBaseObject { public:            // RTTI CClass : CBaseObject; fields as engine/CClass.h
    unsigned char ClassListPos[0x4];                    // +0x08 list<CClass*>::iterator
    unsigned char TypeListPos[0x4];                     // +0x0c list<CClass*>::iterator
    unsigned char Type;                                 // +0x10
    unsigned long GetType() const { return Type; }      // FableWin.pdb ?GetType@CClass@@QBEKXZ (inline)
};

class CThing : public CClass { public:                  // RTTI CThing : CClass
    unsigned char _pad14[0x20 - 0x14];
    unsigned long PresentTCs[9];                        // +0x20 bitset of the thing's components (descriptive member name)
    bool HasTC(ETCInterfaceType type) const             // inline in both PDBs' callers
    {
        if (PresentTCs[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    void RemoveTC(ETCInterfaceType type);               // 0x004C9840
};
class CThingCreatureBase : public CThing { };

class CBaseIntelligentPointer {                       // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void PerformAction();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08
    unsigned char _pad10[0x114 - 0x10];                     // +0x10
};

class CCreatureAction_WhisperSommersaultForward : public CCombatActionBase { public:
    virtual void FinishAction();
    unsigned char Finished;                                 // +0x114 (set by PerformAction 0x008C0B40)
};

void CCreatureAction_WhisperSommersaultForward::FinishAction()
{
    if (PCreature.GetPItem()->HasTC((ETCInterfaceType)77))
        PCreature.GetPItem()->RemoveTC((ETCInterfaceType)77);
    CCombatActionBase::FinishAction();
}