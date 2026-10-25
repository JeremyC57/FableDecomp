// CCreatureAction_BeDazed::CCreatureAction_BeDazed(CThingCreatureBase& owner) -- the base action for the owner: speed -1, end time 0, the owner's random loaded "DAZED_IDLE" animation, an empty sound pair, flags 0 / 0
// Leaf: ??0CCreatureAction_BeDazed@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865A00, 104 bytes (+ int3 padding), thiscall, `ret 4`. Vtable 0x01276F54.
// Identity: CCreatureAction_BeDazed::CCreatureAction_BeDazed(CThingCreatureBase&) -- proven; evidence: it stores vtable 0x01276F54 whose RTTI is .?AVCCreatureAction_BeDazed@@ :
//   CCreatureActionBase : CBaseObject : CBaseClass; FableWin.pdb name set holds ??0CCreatureAction_BeDazed@@QAE@AAVCThingCreatureBase@@@Z; the literal
//   0x0127706C is "DAZED_IDLE". Family G044 (8 action constructors differing in class, literal and owner type; the propagated
//   label "CCreatureAction_AlertToPlayer::CCreatureAction_AlertToPlayer" is right only for 0x00842300).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName(const
//   CThingCreatureBase&, const CCharString&) 0x006924B0 (`__fastcall` protected static of FableWin.pdb; it thunks onto
//   CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) 0x00662FA0), CCreatureActionBase::CCreatureActionBase(
//   CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30 (FableWin.pdb),
//   CCharString::~CCharString() 0x0099EAE0.
// Shape: the literal converts to a CCharString temporary before any argument is pushed and dies after the base call; the
//   CSoundPair() temporary's two zero words are stored after its address is pushed; no EH frame (built without /GX).
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
class CThingAICreature : public CThingCreatureBase { };
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
class CCreatureAction_BeDazed : public CCreatureActionBase {
public:
    CCreatureAction_BeDazed(CThingCreatureBase& owner);
};

CCreatureAction_BeDazed::CCreatureAction_BeDazed(CThingCreatureBase& owner)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomAnimFromName(owner, "DAZED_IDLE"), CSoundPair(), 0, 0)
{
}