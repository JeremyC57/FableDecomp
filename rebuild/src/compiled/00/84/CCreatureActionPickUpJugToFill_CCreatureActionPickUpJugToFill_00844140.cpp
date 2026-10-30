// CCreatureAction_PickUpJugToFill::CCreatureAction_PickUpJugToFill(CThingCreatureBase& owner, CThing& target) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's "FILL_JUG_INTO" animation, a zeroed CSoundPair, 0, 0), then the PThingToPickUp intelligent pointer at +0xa8 set to the target
// Leaf: ??0CCreatureAction_PickUpJugToFill@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844140, 144 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x01275504
//   (RTTI .?AVCCreatureAction_PickUpJugToFill@@).
// Identity: the stored vtable's RTTI names the class; Ego_r / FableWin.pdb name set
//   `??0CCreatureAction_PickUpJugToFill@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z`. Family G013 (10 constructors sharing one
//   shape, differing in the animation-name literal and the vtable); the manifest label "CCreatureAction_AddRealObjectToInventory" is propagated.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 and ~CCharString 0x0099EAE0 (the implicit conversion
//   temporary of the literal, built before any argument is pushed and destroyed after the base call), the
//   owner's animation lookup 0x006924B0 (`__fastcall`: ecx owner, edx name; descriptive name), CCreatureActionBase::
//   CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30
//   (PDB), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90; string literal 0x0127561C ("FILL_JUG_INTO"); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: retail CCreatureActionBase is 0xa8 bytes (Ego_r 0xb4: its STLport containers are 4 bytes shorter each), so
//   PThingToPickUp (Ego_r +0xb4) sits at +0xa8. Local model: the generated engine headers carry the flattened Ego_r layout.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };
class CThingCreatureBase;
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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CCreatureAction_PickUpJugToFill : public CCreatureActionBase {  // vtable 0x01275504
public:
    CIntelligentPointer<CThing> PThingToPickUp;                       // +0xa8
    virtual ~CCreatureAction_PickUpJugToFill();
    CCreatureAction_PickUpJugToFill(CThingCreatureBase& owner, CThing& target);
};

CCreatureAction_PickUpJugToFill::CCreatureAction_PickUpJugToFill(CThingCreatureBase& owner, CThing& target)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetAnimationFromName(&owner, "FILL_JUG_INTO"), CSoundPair(), 0, 0)
    , PThingToPickUp(&target)
{
}