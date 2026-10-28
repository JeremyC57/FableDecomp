// CCreatureAction_ShowItemToThing::CCreatureAction_ShowItemToThing(CThingCreatureBase& owner, CThing& receiver, CThing& item) -- the base action on the owner's random loaded "STANDARD_SHOW_OBJECT" animation (speeds -1.0 / 0.75, empty sound pair, flags 0 / 0), then the two intelligent pointers PReceivingThing and PItemBeingShown attached to the receiver and the item
// Leaf: ??0CCreatureAction_ShowItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED780, 161 bytes, thiscall, `ret 0Ch`, returns this. Vtable 0x012714C4.
// Identity (proven): the vtable it stores (0x012714C4) has the RTTI .?AVCCreatureAction_ShowItemToThing@@ (slot 0 the landed
//   ??_G 0x007ED830, slot 11 the landed Clone 0x007ED9F0); Ego_r.pdb / FableWin.pdb hold
//   ??0CCreatureAction_ShowItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z (three reference arguments = `ret 0Ch`);
//   the literal 0x012715DC is "STANDARD_SHOW_OBJECT"; its only caller is 0x007ED900.
// Label: the manifest's "CCreatureAction_GiveItemToThing::CCreatureAction_GiveItemToThing" is propagated (same signature,
//   different vtable / literal).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName(const
//   CThingCreatureBase&, const CCharString&) 0x006924B0 (protected static __fastcall, PDB), CCreatureActionBase::
//   CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::~CCharString() 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90; vtable CIntelligentPointer<CThing> 0x01238C6C.
// Layout: Ego_r.pdb CCreatureAction_ShowItemToThing ends with PReceivingThing / PItemBeingShown (CIntelligentPointer<CThing>);
//   retail CCreatureActionBase is 0xA8 bytes, so they sit at +0xA8 / +0xB0 (the landed copy ctor 0x007EDA20 agrees).
// Shape: the landed YellOverHere ctor 0x008421E0 (literal temporary first, one eax zero for the two longs and the sound
//   pair) followed by the landed GiveMoneyToThing pointer member (base ctor, vtable, SetPItem).
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CBaseObject;
class CThing;
class CAnimationEntry;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    void* PRecord;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: 0x01238C6C
public:
    CIntelligentPointer(T& item) { SetPItem((CBaseObject*)&item); }
};
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};
class CCreatureAction_ShowItemToThing : public CCreatureActionBase {
public:
    CCreatureAction_ShowItemToThing(CThingCreatureBase& owner, CThing& receiver, CThing& item);
    CIntelligentPointer<CThing> PReceivingThing;             // +0xA8
    CIntelligentPointer<CThing> PItemBeingShown;             // +0xB0
};

CCreatureAction_ShowItemToThing::CCreatureAction_ShowItemToThing(CThingCreatureBase& owner, CThing& receiver, CThing& item)
    : CCreatureActionBase(owner, -1.0f, 0.75f, GetRandomAnimFromName(owner, "STANDARD_SHOW_OBJECT"), CSoundPair(), 0, 0)
    , PReceivingThing(receiver)
    , PItemBeingShown(item)
{
}