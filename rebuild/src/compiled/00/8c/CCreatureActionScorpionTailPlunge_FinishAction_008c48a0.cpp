// CCreatureAction_ScorpionTailPlunge::FinishAction() -- when the target's flag bit 3 (+0x6c) is set, put the scorpion king into a new CModeScorpionKingTailPlunge aimed at the action's target through the owner's mode manager; then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C48A0, length 239 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0xbc bytes), CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge(CThing*)
// @ 0x008D81F0 (vtable ctor labelled by its vtable store), CCountedPointer<CModeScorpionKingTailPlunge>::CCountedPointer(T*)
// @ 0x008C58F0 (out of line, per-type deleter 0x008C57E0), CModeScorpionKingTailPlunge::SetTarget(CThing*) @ 0x008D7430,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, operator delete @ 0x00BFE9BC (the count block),
// CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the twin of 0x008C4990 testing the TARGET's flag; the target is fetched again (ebx), then the UNCHECKED out-parameter GetInterface (the absent path reads the never
// written out pointer from the lookup key's frame word, which the counted-pointer local reuses afterwards); the counted
// pointer is converted into a CCountedPointer<CModeBase> built in the argument area (inline converting copy, add-ref) and
// released inline afterwards (block {refs, __fastcall destroy, object}, freed with operator delete). `this` is spilled to the
// frame across the block (register pressure).
// Layout: the owner (CIntelligentPointer<CThingCreatureBase>) at +8, the target at +0xa8 (retail CCreatureActionBase is 0xa8).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x20];
    unsigned char m_flags6c;                    // +0x6c bit 3
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

void operator delete(void* p);                  // 0x00BFE9BC

void __fastcall DestroyModeScorpionKingTailPlunge(void* object);   // 0x008C57E0: the per-type deleter stored in the block

struct SCountBlock {
    long  m_references;                         // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyModeScorpionKingTailPlunge), m_pObject(object) { }
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object);                 // 0x008C58F0 for CModeScorpionKingTailPlunge
    template <class U> CCountedPointer(const CCountedPointer<U>& other)
        : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    T* operator->() const { return m_pObject; }
};

struct CModeScorpionKingTailPlunge : CModeBase {
    unsigned char _pad[0xbc];
    CModeScorpionKingTailPlunge(CThing* owner); // 0x008D81F0
    void SetTarget(CThing* target);             // 0x008D7430
};

class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

// The counted pointer's constructor is compiled in this TU (0x008C58F0, landed separately): its body above the caller
// under auto_inline(off) tells VC7.1 the local does not escape, so the block pointer stays in esi across AddMode.
#pragma auto_inline(off)
CCountedPointer<CModeScorpionKingTailPlunge>::CCountedPointer(CModeScorpionKingTailPlunge* object)
    : m_pObject(object)
{
    if (object)
        m_pCount = new SCountBlock(m_pObject);
    else
        m_pCount = 0;
}
#pragma auto_inline(on)

class CCombatActionBase {
public:
    protected: virtual void FinishAction(); public:                // 0x00859A20
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    __forceinline CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
        return modeManager;
    }
};

class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase {
public:
    virtual void FinishAction();
};

void CCreatureAction_ScorpionTailPlunge::FinishAction()
{
    if (PTarget.GetPItem()->m_flags6c & 8) {
        CThing* target = PTarget.GetPItem();
        CTCCreatureModeManager* modeManager = GetModeManager();
        CCountedPointer<CModeScorpionKingTailPlunge> mode(new CModeScorpionKingTailPlunge(PCreature.GetPItem()));
        mode->SetTarget(target);
        modeManager->AddMode(mode);
    }
    CCombatActionBase::FinishAction();
}