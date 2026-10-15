// CCreatureAction_ShowLove::CCreatureAction_ShowLove(CThingCreatureBase*) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's random loaded "STANDARD_EXCITED" animation, zeroed CSoundPair, 0, 0), own vtable (0x01274F4C), the creature pointer member at +0xa8 set to the owner
// Label: the manifest row says CCreatureAction_AddRealObjectToInventory (propagated); the stored vtable 0x01274F4C is CCreatureAction_ShowLove (rtti-vtables.tsv).
// Leaf: ??0CCreatureAction_ShowLove
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008438C0, length 124 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the random-loaded-animation lookup @ 0x006924B0
// (`__fastcall`: ecx owner, edx name; a thunk onto CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) @ 0x00662FA0),
// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the animation name is an implicit conversion temporary built first and re-addressed with `lea`; the owner is kept
// in ebx because it is needed again after the base constructor (SetPItem), which frees edi/eax to carry the shared zero of
// the sound pair; base arguments right to left (0, 0, the sound pair, the lookup, 0.0f as `push edi` (the shared zero register), -1.0f, the owner); then the vtable
// and the CIntelligentPointer<CThingCreatureBase> member at +0xa8 (base ctor, vtable 0x0123E7E0, SetPItem(owner)). `ret 4`.
// Layout: retail CCreatureActionBase is 0xa8 bytes; no engine header exists for this class, so `this` is modelled locally.
struct CThingCreatureBase;
struct CAnimationEntry;
struct CBaseObject;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CCreatureAction_ShowLove : CCreatureActionBase {   // vtable 0x01274F4C
    CIntelligentPointer PCreatureLoving;   // +0xa8: the owner again
    virtual void v00();
    CCreatureAction_ShowLove(CThingCreatureBase* owner);
};

CCreatureAction_ShowLove::CCreatureAction_ShowLove(CThingCreatureBase* owner)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnimation(owner, "STANDARD_EXCITED"), CSoundPair(), 0, 0)
    , PCreatureLoving(owner)
{
}