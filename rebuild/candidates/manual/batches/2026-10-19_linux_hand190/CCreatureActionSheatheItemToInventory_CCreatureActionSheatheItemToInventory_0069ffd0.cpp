// CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) -- base CCreatureActionBase(owner, -1.0, 0.55, a copy (transition time 4) of the animation entry named by GetAnimationToUse(owner), zeroed CSoundPair, the long, 150), then adopt the follow-up action in PFollowUpAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FFD0, length 240 bytes, `ret 0Ch`. Vtable 0x0125C83C.
// Callees: GetAnimationToUse(CThing*) @ 0x0069F3E0 (`__fastcall`, CCharString returned through the hidden pointer in ecx,
// the owner in edx), CCharString::~CCharString @ 0x0099EAE0, the animation lookup by name @ 0x006924B0 (`__fastcall`: ecx owner, edx the name; returns const CAnimationEntry&),
// CAnimationEntry::CAnimationEntry(const CAnimationEntry&, long) @ 0x005DEC70, CCreatureActionBase::CCreatureActionBase(
// CThingCreatureBase*, float, float, const CAnimationEntry&, const CSoundPair&, long, long) @ 0x00693B30, operator new
// @ 0x00BFEA1A (the 12-byte count block), the per-type deleter @ 0x00668610 (stored, not called).
// Shape: the overload of 0x0069EF80 whose animation name comes from GetAnimationToUse; that CCharString temporary is
// homed in the dead third-argument slot (the long is read first) and dies after the entry temporary. The zero register
// (ebx) of the CSoundPair feeds every later null test, and the entry's `Components = 0` store survives here.
// The CAnimationEntry temporary (16 bytes, frame +8) dies after the base constructor: its inline destructor
// releases the component map through the ref-count base at +4 (`mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi`),
// null store elided. After the vtable store the CCountedPointer<CCreatureActionBase> member is built from the raw
// pointer with the inline `if (p) Count = new Block(Object); else Count = 0;` constructor (three duplicated exits,
// the object re-read through `this`).
// Layout: Ego_r.pdb PFollowUpAction at +0xb4; retail CCreatureActionBase is 0xa8, so it sits at +0xa8.
struct CThing { };
struct CThingCreatureBase : CThing { };

struct CCharString {
    void* m_pData;
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GetAnimationToUse(CThing* owner);   // 0x0069F3E0

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CRefCountedBase {                               // second base of the pointee: the count at +4
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CPointeeVirtual {
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
struct CAnimComponentMap : CPointeeVirtual, CRefCountedBase {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CAnimationEntry {                               // 16 bytes
    long               BankIndex;                      // +0x00
    long               AnimName;                       // +0x04 CDefString
    long               GroupName;                      // +0x08 CDefString
    CAnimComponentMap* Components;                     // +0x0c CIVCountedPointer<CAnimComponentMap>
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime);   // 0x005DEC70
    ~CAnimationEntry()
    {
        if (Components) {
            Components->ReleaseTemporary();
            Components = 0;
        }
    }
};

const CAnimationEntry& __fastcall GetAnimationFromName(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                           // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, const CAnimationEntry& animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

void __fastcall DeleteCreatureAction(void* object);    // 0x00668610: the per-type deleter stored in the block

struct SCountBlock {
    long  m_references;                                // +0
    void (__fastcall* m_pfnDestroy)(void*);            // +4
    void* m_pObject;                                   // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DeleteCreatureAction), m_pObject(object) { }
};

template <class T> struct CCountedPointer {
    T*           m_pObject;                            // +0
    SCountBlock* m_pCount;                             // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
};

struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {   // vtable 0x0125C83C
    CCountedPointer<CCreatureActionBase> PFollowUpAction;              // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* followUp, long c);
};

CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* followUp, long c)
    : CCreatureActionBase(owner, -1.0f, 0.55f, CAnimationEntry(GetAnimationFromName(owner, GetAnimationToUse(owner)), 4), CSoundPair(), c, 150)
    , PFollowUpAction(followUp)
{
}
