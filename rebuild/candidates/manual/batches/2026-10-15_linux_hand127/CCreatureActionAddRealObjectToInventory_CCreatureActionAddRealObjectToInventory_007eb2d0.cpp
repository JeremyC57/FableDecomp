// CCreatureAction_AddRealObjectToInventory::CCreatureAction_AddRealObjectToInventory(CThingCreatureBase*, CThing*) -- base CCreatureActionBase(owner, 0.0, 0.0, the owner's random loaded animation of the empty name, zeroed CSoundPair, 0, 0), own vtable (0x01270D5C), PObject set to the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB2D0, length 132 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the random-loaded-animation lookup @ 0x006924B0
// (`__fastcall`: ecx owner, edx name; a thunk onto CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) @ 0x00662FA0),
// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the (empty, 0x0122D70E) animation name is an implicit conversion temporary built first and re-addressed with `lea`;
// the base arguments go right to left (0, 0, the zeroed sound pair, the lookup, 0.0f, 0.0f as `push 0`, the owner); the
// temporary dies after the base constructor, then the vtable and the CIntelligentPointer<CThing> member at +0xa8
// (base ctor, its vtable 0x0125A858, SetPItem(thing) from the argument slot). `ret 8`: two arguments.
// Layout: retail CCreatureActionBase is 0xa8 bytes; no engine header exists for this class, so `this` is modelled locally.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CBaseObject;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (frame stores before the base call)
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

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {   // vtable 0x01270D5C
    CIntelligentPointer PObject;            // +0xa8: the real object to add
    virtual void v00();
    CCreatureAction_AddRealObjectToInventory(CThingCreatureBase* owner, CThing* object);
};

CCreatureAction_AddRealObjectToInventory::CCreatureAction_AddRealObjectToInventory(CThingCreatureBase* owner, CThing* object)
    : CCreatureActionBase(owner, 0.0f, 0.0f, GetRandomLoadedAnimation(owner, ""), CSoundPair(), 0, 0)
    , PObject(object)
{
}
