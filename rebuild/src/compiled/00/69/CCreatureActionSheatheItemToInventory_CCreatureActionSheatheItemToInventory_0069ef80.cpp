// CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, const CCharString&, CCreatureActionBase*, long) -- base CCreatureActionBase(owner, -1.0, 0.0, a copy of the owner's named animation entry with transition time 0, zeroed CSoundPair, the long, 0), then adopt the follow-up action in PFollowUpAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069EF80, length 216 bytes (+ int3 padding), `ret 10h`. Vtable 0x0125C83C.
// Callees: the animation lookup by name @ 0x006924B0 (`__fastcall`: ecx owner, edx the name; returns const CAnimationEntry&),
// CAnimationEntry::CAnimationEntry(const CAnimationEntry&, long) @ 0x005DEC70, CCreatureActionBase::CCreatureActionBase(
// CThingCreatureBase*, float, float, const CAnimationEntry&, const CSoundPair&, long, long) @ 0x00693B30, operator new
// @ 0x00BFEA1A (the 12-byte count block), the per-type deleter @ 0x00668610 (stored, not called).
// Shape: the CAnimationEntry temporary (16 bytes, frame +8) dies after the base constructor: its inline destructor
// releases the component map through the ref-count base at +4 (`mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi`),
// null store elided. After the vtable store the CCountedPointer<CCreatureActionBase> member is built from the raw
// pointer with the inline `if (p) Count = new Block(Object); else Count = 0;` constructor (three duplicated exits,
// the object re-read through `this`).
// Layout: Ego_r.pdb PFollowUpAction at +0xb4; retail CCreatureActionBase is 0xa8, so it sits at +0xa8.
class CThingCreatureBase;

class CCharString { public:
    void* m_pData;
};

class CSoundPair { public:
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
class CAnimComponentMap : public CPointeeVirtual, public CRefCountedBase { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CAnimationEntry { public:                               // 16 bytes
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

class CCreatureActionBase { public:                           // retail sizeof 0xa8
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

template <class T> class CCountedPointer { public:
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

class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:   // vtable 0x0125C83C
    CCountedPointer<CCreatureActionBase> PFollowUpAction;              // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, const CCharString& animation, CCreatureActionBase* followUp, long c);
};

CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, const CCharString& animation, CCreatureActionBase* followUp, long c)
    : CCreatureActionBase(owner, -1.0f, 0.0f, CAnimationEntry(GetAnimationFromName(owner, animation), 0), CSoundPair(), c, 0)
    , PFollowUpAction(followUp)
{
}