// CCreatureAction_JackScream::Initialise() -- base Initialise, drop any running mode 0x31 from the creature's mode manager, create a fresh NCreatureMode::CModeJackOfBladesScream for the creature as this action's counted PMode, hand it the sword object carried in the primary slot and push it onto the mode manager
// Leaf: ?Initialise@CCreatureAction_JackScream@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
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
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

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
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    if (modeManager->IsModeActive(0x31))
        modeManager->RemoveMode(0x31);
    PMode = CCountedPointer<CModeJackOfBladesScream>(new CModeJackOfBladesScream(PCreature.Get()));
    CTCCarrying* carrying;
    PCreature.Get()->GetTC(TCI_CARRYING, &carrying);
    PMode.m_pObject->SetSwordObject(carrying->GetThingInPrimarySlot());
    modeManager->AddMode(CCountedPointer<CModeBase>(PMode));
}