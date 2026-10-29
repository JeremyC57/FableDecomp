// CCreatureAction_JackScream::Initialise() -- base Initialise, drop any running mode 0x31 from the creature's mode manager, create a fresh NCreatureMode::CModeJackOfBladesScream for the creature as this action's counted PMode, hand it the sword object carried in the primary slot and push it onto the mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0D10, length 393 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24; 0x46 = CTCCarrying, bit 6 of +0x28; both untested),
// CTCCreatureModeManager::IsModeActive(long) @ 0x0073A8A0 and RemoveMode(long) @ 0x0073ABA0, operator new @ 0x00BFEA1A (0xbc-byte mode,
// 12-byte count block) / operator delete @ 0x00BFE9BC, NCreatureMode::CModeJackOfBladesScream(CThing*) @ 0x00740450,
// CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CModeJackOfBladesScream::SetSwordObject(CThing*) @ 0x008D7370,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0; the count block's destroy fn for this mode type 0x008C2D60.
// Layout: retail CCombatActionBase is 0x114 bytes (the PDB header is 0x18 larger): PMode {object +0x114, count +0x118}.
// Shape: `PMode = CCountedPointer<T>(new T(creature))` -- the temporary's inline ctor (null-checked new, null-checked block), the
// inline operator= (release of the old block through the re-read member, adopt, add-ref) and the temporary's inline dtor; the
// AddMode argument is the converting copy built in the argument area. The mode manager is spilled to the lookup key slot.
void __fastcall Destroy_008c2d60(void* object);   // 0x008C2D60
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
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
struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_008c2d60), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;                // operator delete @ 0x00BFE9BC
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;    // the source's fields are read first (kept in ebx/esi across the release)
        T* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
    T* operator->() const { return m_pObject; }
};
struct CModeBase { };
struct CModeJackOfBladesScream : CModeBase {    // retail sizeof 0xbc
    unsigned char _body[0xbc];
    CModeJackOfBladesScream(CThing* owner);     // 0x00740450
    void SetSwordObject(CThing* sword);         // 0x008D7370
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
    void RemoveMode(unsigned long);   // 0x0073ABA0
};
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];
};
class CCreatureAction_JackScream : public CCombatActionBase { public:
    CCountedPointer<CModeJackOfBladesScream> PMode;   // +0x114
    virtual void Initialise();
};

void CCreatureAction_JackScream::Initialise()
{
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
    if (modeManager->IsModeActive(0x31))
        modeManager->RemoveMode(0x31);
    PMode = CCountedPointer<CModeJackOfBladesScream>(new CModeJackOfBladesScream(PCreature.GetPItem()));
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    PMode.m_pObject->SetSwordObject(carrying->GetThingInPrimarySlot());
    modeManager->AddMode(CCountedPointer<CModeBase>(PMode));
}