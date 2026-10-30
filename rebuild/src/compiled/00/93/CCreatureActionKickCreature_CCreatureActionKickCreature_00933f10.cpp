// CCreatureAction_KickCreature::CCreatureAction_KickCreature(CThingCreatureBase& owner, CThingCreatureBase* target) -- base CCreatureActionBase(owner, -1.0f, 0.8f, the owner's "KICK" animation, a zeroed CSoundPair, 0, 0), then the PTarget intelligent pointer at +0xa8 set to target
// Leaf: ??0CCreatureAction_KickCreature@@QAE@AAVCThingCreatureBase@@PAV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933F10, 130 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x0129215C (RTTI
//   .?AVCCreatureAction_KickCreature@@).
// Identity (proven): CCreatureAction_KickCreature::CCreatureAction_KickCreature; evidence: the stored vtable 0x0129215C's RTTI names the class; the member pointer's vtable
//   0x0123E7D8 is CIntelligentPointer<CThingCreatureBase> (RTTI); Ego_r / FableWin.pdb name the constructor ??0CCreatureAction_KickCreature@@QAE@AAVCThingCreatureBase@@PAV1@@Z and the member
//   PTarget (CIntelligentPointer<CThingCreatureBase>, Ego_r +0xb4 = retail +0xa8); literal 0x01292274 = "KICK".
// Label: the manifest's "CCreatureAction_AddRealObjectToInventory" is propagated across family H026 (seven constructors sharing one shape, differing in the
//   animation literal, the two floats, the vtable and the member pointer's vtable); the landed G013 model
//   0x00843700 (CCreatureAction_PickUpGenericBox) is the same shape.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 and ~CCharString 0x0099EAE0 (the literal's conversion
//   temporary), GetAnimationFromName 0x006924B0 (`__fastcall`: ecx owner, edx name; descriptive name),
//   CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&,
//   long, long) 0x00693B30 (PDB), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90.
// Layout: retail CCreatureActionBase is 0xa8 bytes (Ego_r 0xb4), so PTarget sits at +0xa8. Local model (the generated
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

class CCreatureAction_KickCreature : public CCreatureActionBase {  // vtable 0x0129215C
public:
    CIntelligentPointer<CThingCreatureBase> PTarget;                       // +0xa8
    virtual ~CCreatureAction_KickCreature();
    CCreatureAction_KickCreature(CThingCreatureBase& owner, CThingCreatureBase* target);
};

CCreatureAction_KickCreature::CCreatureAction_KickCreature(CThingCreatureBase& owner, CThingCreatureBase* target)
    : CCreatureActionBase(owner, -1.0f, 0.8f, GetAnimationFromName(&owner, "KICK"), CSoundPair(), 0, 0)
    , PTarget(target)
{
}