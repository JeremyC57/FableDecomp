// NCreatureMode::CModeIntangible::CModeIntangible(CThing*, CCreatureActionBase*) -- CModeDragonBase(thing, "") then an untimed intangible phase (Timer -1, was damageable) holding the action to run on deactivation
// Leaf: ??0CModeIntangible
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741DB0, 224 bytes (`ret 8`). Vtables 0x012684E0 (NCreatureMode::CModeDragonBase,
// stored by its inline ctor) and 0x01268514 (NCreatureMode::CModeIntangible).
// Callees: CModeBase::CModeBase(CThing*) 0x0073EAC0; CCharString(const char*, long) 0x0099EBF0 (literal ""),
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0;
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10; operator new 0x00BFEA1A (the 12-byte count block);
// the count block's destroy routine 0x00668610 (stored, not called).
// Layout (PDB 0x44, matches retail): CModeDragonBase 0x30 (Filter +0x1c, PTarget +0x28), Timer +0x30, BossPhase +0x34,
// WasDamageable +0x38, PActionOnDeactivate +0x3c (CCountedPointer<CCreatureActionBase>).
// Shape: the group name is an implicit CCharString conversion temporary built BEFORE the base chain (it is the inline
// CModeDragonBase ctor's argument) and destroyed after it; the counted pointer's raw-pointer ctor is inline
// (object stored, block new'd only for a non-null action, as the landed 0x00695880).
class CThing;
class CThingCreatureBase;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
};

template <class T> struct CAllocator {     // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {    // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};

class C3DVector { public: float X, Y, Z; };

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

template <class T> class CDefPointer { public:    // 4 bytes: the def object (add-ref'd by its owner)
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    unsigned char _body[0x18];             // ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);              // 0x0073EAC0
};

class CCreatureActionBase;

struct SCountBlock {                       // 12 bytes, operator new'd
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object);
};

void __fastcall CountedDestroy(void* object);   // 0x00668610 (type-erased delete of the counted object)

inline SCountBlock::SCountBlock(void* object)
    : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object)
{
}

template <class T> class CCountedPointer { public:   // 8 bytes: object + shared count block
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(T* object)
    {
        m_pObject = object;
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    void Reset(T* object);                 // 0x00668D90 (one body shared by every instantiation)
};

namespace NCreatureMode {

// The header engine/CModeDragonBase.h flattens the vptr into a `void*` member, so it cannot be a polymorphic
// base here; this local model keeps its layout (CModeBase 0x1c, Filter +0x1c, PTarget +0x28; 0x30 bytes).
struct CModeDragonBase : CModeBase {       // vtable 0x012684E0
    CAnimFilterOfGroup       Filter;       // +0x1c
    CIntelligentPointerThing PTarget;      // +0x28
    virtual void v00();
    CModeDragonBase(CThing* thing, const CCharString& group)
        : CModeBase(thing)
        , Filter(group)
    {
    }
};

struct CModeIntangible : CModeDragonBase { // vtable 0x01268514
    long                                  Timer;               // +0x30
    long                                  BossPhase;           // +0x34
    bool                                  WasDamageable;       // +0x38
    CCountedPointer<CCreatureActionBase>  PActionOnDeactivate; // +0x3c
    virtual void v00();
    CModeIntangible(CThing* thing, CCreatureActionBase* actionOnDeactivate);
};

CModeIntangible::CModeIntangible(CThing* thing, CCreatureActionBase* actionOnDeactivate)
    : CModeDragonBase(thing, "")
    , Timer(-1)
    , WasDamageable(true)
    , PActionOnDeactivate(actionOnDeactivate)
{
}

}