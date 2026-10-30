// CCreatureAction_UseSwitch::CCreatureAction_UseSwitch(CThingCreatureBase& owner, CThing& switchThing) -- base CCreatureActionBase(owner, -1.0f, 1.0f, the owner's "PICK_UP_OBJECT" animation, a zeroed CSoundPair, 0, 0), then the PSwitch intelligent pointer at +0xa8 set to switchThing
// Leaf: ??0CCreatureAction_UseSwitch@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE010, 130 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x01271604 (RTTI
//   .?AVCCreatureAction_UseSwitch@@).
// Identity (proven): CCreatureAction_UseSwitch::CCreatureAction_UseSwitch; evidence: the stored vtable 0x01271604's RTTI names the class; the member pointer's vtable
//   0x01238C6C is CIntelligentPointer<CThing> (RTTI); Ego_r / FableWin.pdb name the constructor ??0CCreatureAction_UseSwitch@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z and the member
//   PSwitch (CIntelligentPointer<CThing>, Ego_r +0xb4 = retail +0xa8); literal 0x0127171C = "PICK_UP_OBJECT".
// Label: the manifest's "CCreatureAction_AddRealObjectToInventory" is propagated across family H026 (seven constructors sharing one shape, differing in the
//   animation literal, the two floats, the vtable and the member pointer's vtable); the landed G013 model
//   0x00843700 (CCreatureAction_PickUpGenericBox) is the same shape.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 and ~CCharString 0x0099EAE0 (the literal's conversion
//   temporary), GetAnimationFromName 0x006924B0 (`__fastcall`: ecx owner, edx name; descriptive name),
//   CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&,
//   long, long) 0x00693B30 (PDB), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90.
// Layout: retail CCreatureActionBase is 0xa8 bytes (Ego_r 0xb4), so PSwitch sits at +0xa8. Local model (the generated
//   engine headers carry the flattened Ego_r layout).
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };
class CThingCreatureBase : public CThing { };

class CAnimationEntry;

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CSoundPair {                                          // two sound ids, zeroed by the inline constructor
public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

const CAnimationEntry& __fastcall GetAnimationFromName(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

class CCreatureActionBase {                                 // retail sizeof 0xa8
public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase& owner, float a, float b, const CAnimationEntry& animation,
                        const CSoundPair& sound, long c, long d);                             // 0x00693B30
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                     // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

class CCreatureAction_UseSwitch : public CCreatureActionBase {  // vtable 0x01271604
public:
    CIntelligentPointer<CThing> PSwitch;                       // +0xa8
    virtual ~CCreatureAction_UseSwitch();
    CCreatureAction_UseSwitch(CThingCreatureBase& owner, CThing& switchThing);
};

CCreatureAction_UseSwitch::CCreatureAction_UseSwitch(CThingCreatureBase& owner, CThing& switchThing)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationFromName(&owner, "PICK_UP_OBJECT"), CSoundPair(), 0, 0)
    , PSwitch(&switchThing)
{
}