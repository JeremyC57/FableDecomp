// CCombatAction_FallStunned::Initialise() -- push a fresh CModeDazed (not yet started) onto the creature's mode manager, then the base CCombatActionBase::Initialise
// Leaf: Initialise
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ED950, 192 bytes incl. padding (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, twice), CVectorMap::LowerBound @ 0x0040F020
//          (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24; the result is not tested, so a missing
//          manager leaves the slot unwritten), operator new @ 0x00BFEA1A (0x84-byte CModeDazed, 12-byte count block),
//          CModeDazed::CModeDazed(CThing*) @ 0x00740160, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>)
//          @ 0x0073ACC0, CCombatActionBase::Initialise @ 0x00857520; count-block destroy fn 0x007180B0.
// Layout: retail CCombatActionBase model (PCreature +0x08); CModeDazed +0x6c a started flag.
// Shape: the by-value counted pointer is built in the argument area by the inline CCountedPointer(T*) ctor of the
//          landed 0x00718A00 (a user copy ctor makes the argument build in place); its null test folds away because the new mode was already dereferenced (`[eax+6Ch]`).
void __fastcall Destroy_007180b0(void* object);   // 0x007180B0

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
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_007180b0), m_pObject(object) { }
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
};

struct CModeBase { };
struct CModeDazed : CModeBase {
    unsigned char _pad[0x6c];
    bool          Started;                      // +0x6c
    unsigned char _pad6d[0x84 - 0x6d];
    CModeDazed(CThing* creature);               // 0x00740160
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
};

class CCombatAction_FallStunned : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_FallStunned::Initialise()
{
    CTCCreatureModeManager* modeManager;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CModeDazed* mode = new CModeDazed(PCreature.Get());
    mode->Started = false;
    modeManager->AddMode(CCountedPointer<CModeBase>(mode));
    CCombatActionBase::Initialise();
}