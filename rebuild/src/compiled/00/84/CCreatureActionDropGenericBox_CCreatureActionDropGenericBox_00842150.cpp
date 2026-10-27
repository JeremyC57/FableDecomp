// CCreatureAction_DropGenericBox::CCreatureAction_DropGenericBox(CThingCreatureBase& owner) -- the base action for the owner: speed -1, end time 0.5f, the owner's random loaded "ST_HOLD_BOX_PUT_DOWN" animation, an empty sound pair, flags 0 / 0
// Leaf: ??0CCreatureAction_DropGenericBox@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00842150, 99 bytes, thiscall, `ret 4`. Vtable 0x012731E4.
// Identity (proven): it stores vtable 0x012731E4 whose RTTI is .?AVCCreatureAction_DropGenericBox@@; Ego_r.pdb ??0CCreatureAction_DropGenericBox@@QAE@AAVCThingCreatureBase@@@Z
//   (donor 0x0051C660); the literal 0x012732FC is "ST_HOLD_BOX_PUT_DOWN"; callers NTCScriptedControl::CActionDropGenericBox 0x00715560, CTCCarriedActionUseDropGenericBox 0x007ED060 (landed) and CAIStateGroup_RecoverStealableItem::Update 0x009658B0.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName(const
//   CThingCreatureBase&, const CCharString&) 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase(
//   CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::~CCharString() 0x0099EAE0.
// Shape (family K028, representative 0x007EBA20): the landed G044 constructor 0x00842AF0 with a nonzero end time; with no
//   shared zero for the float VC7.1 builds the CSoundPair() temporary before its address is pushed.
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
class CCreatureAction_DropGenericBox : public CCreatureActionBase {
public:
    CCreatureAction_DropGenericBox(CThingCreatureBase& owner);
};

CCreatureAction_DropGenericBox::CCreatureAction_DropGenericBox(CThingCreatureBase& owner)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomAnimFromName(owner, "ST_HOLD_BOX_PUT_DOWN"), CSoundPair(), 0, 0)
{
}