// CCreatureAction_ScorpionPrepareTailPlunge::FinishAction() -- when the owner's flag bit 3 (+0x6c) is set, put the scorpion king into a new CModeScorpionKingVulnerable aimed at the action's target through the owner's mode manager; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_ScorpionPrepareTailPlunge@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4990, length 237 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0xb8 bytes), CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing*)
// @ 0x008D8120 (vtable 0x0128582C, mode name "VULNERABLE"), CCountedPointer<CModeScorpionKingVulnerable>::CCountedPointer(T*)
// @ 0x008C5980 (out of line, per-type deleter 0x008C57F0), CModeScorpionKingVulnerable::SetTarget(CThing*) @ 0x008D73E0,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, operator delete @ 0x00BFE9BC (the count block),
// CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the target is fetched first (ebx), then the UNCHECKED out-parameter GetInterface (the absent path reads the never
// written out pointer from the lookup key's frame word, which the counted-pointer local reuses afterwards); the counted
// pointer is converted into a CCountedPointer<CModeBase> built in the argument area (inline converting copy, add-ref) and
// released inline afterwards (block {refs, __fastcall destroy, object}, freed with operator delete). `this` is spilled to the
// frame across the block (register pressure).
// Layout: the owner (CIntelligentPointer<CThingCreatureBase>) at +8, the target at +0xa8 (retail CCreatureActionBase is 0xa8).
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


void operator delete(void* p);                  // 0x00BFE9BC

void __fastcall DestroyModeScorpionKingVulnerable(void* object);   // 0x008C57F0: the per-type deleter stored in the block

struct SCountBlock {
    long  m_references;                         // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyModeScorpionKingVulnerable), m_pObject(object) { }
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*           m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer(T* object);                 // 0x008C5980 for CModeScorpionKingVulnerable
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

struct CModeScorpionKingVulnerable : CModeBase {
    unsigned char _pad[0xb8];
    CModeScorpionKingVulnerable(CThing* owner); // 0x008D8120
    void SetTarget(CThing* target);             // 0x008D73E0
};

class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

// The counted pointer's constructor is compiled in this TU (0x008C5980, landed separately): its body above the caller
// under auto_inline(off) tells VC7.1 the local does not escape, so the block pointer stays in esi across AddMode.
#pragma auto_inline(off)
CCountedPointer<CModeScorpionKingVulnerable>::CCountedPointer(CModeScorpionKingVulnerable* object)
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
    FABLE_FORCEINLINE CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modeManager;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
        return modeManager;
    }
};

class CCreatureAction_ScorpionPrepareTailPlunge : public CCombatActionBase {
public:
    virtual void FinishAction();
};

void CCreatureAction_ScorpionPrepareTailPlunge::FinishAction()
{
    if (PCreature.Get()->GetTypeFlags() & 8) {
        CThing* target = PTarget.Get();
        CTCCreatureModeManager* modeManager = GetModeManager();
        CCountedPointer<CModeScorpionKingVulnerable> mode(new CModeScorpionKingVulnerable(PCreature.Get()));
        mode->SetTarget(target);
        modeManager->AddMode(mode);
    }
    CCombatActionBase::FinishAction();
}