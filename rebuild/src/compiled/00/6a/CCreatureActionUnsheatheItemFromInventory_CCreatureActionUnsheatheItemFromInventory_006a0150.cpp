// CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase*, CThing*, CCreatureActionBase*, long) -- base CCreatureActionBase(owner, -1.0, 0.55, the owner's unsheathe animation for the item with transition 4, zeroed CSoundPair, 100, the long), then the item pointer and the adopted follow-up action
// Leaf: ??0CCreatureAction_UnsheatheItemFromInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0150, 271 bytes, `ret 10h`; vtable 0x0125CCBC.
// Callees: GetAnimationToUse(CThingCreatureBase*, CThing*) 0x0069F980 (__fastcall, CCharString by value through the hidden
//   pointer in ecx), the animation lookup by name 0x006924B0 (__fastcall), CAnimationEntry(const CAnimationEntry&, long)
//   0x005DEC70, CCreatureActionBase(CThingCreatureBase*, float, float, const CAnimationEntry&, const CSoundPair&, long,
//   long) 0x00693B30, ~CCharString 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), operator new 0x00BFEA1A (the 12-byte count block), the per-type deleter
//   0x00668610 (stored, not called).
// Shape: the landed sibling CCreatureAction_SheatheItemToInventory ctor 0x0069EF80 with the animation name coming from
//   GetAnimationToUse (its CCharString result homed in the dead last-argument slot, destroyed after the base ctor), an
//   intelligent pointer member built from the item (inline ctor: base ctor, vtable, SetPItem) and the counted follow-up.
// Layout: retail CCreatureActionBase is 0xa8: PItem +0xa8, PFollowUpAction +0xb0.
class CThingCreatureBase;

class CThing;
class CCharString { public:
    void* m_pData;
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GetAnimationToUse(CThingCreatureBase* owner, CThing* item);   // 0x0069F980
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
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

class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:   // vtable 0x0125CCBC
    CIntelligentPointer<CThing> PItem;                                                  // +0xa8
    CCountedPointer<CCreatureActionBase> PFollowUpAction;                       // +0xb0
    virtual void v00();
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* followUp, long c);
};

CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* followUp, long c)
    : CCreatureActionBase(owner, -1.0f, 0.55f, CAnimationEntry(GetAnimationFromName(owner, GetAnimationToUse(owner, item)), 4), CSoundPair(), 100, c)
    , PItem(item)
    , PFollowUpAction(followUp)
{
}