// CCreatureAction_GetInBed::PerformAction() -- push a new NCreatureMode::CModeSleepBed(creature, AnimGroup, SleepAnimName) onto the creature's mode manager, then when the bed is alive and has a CTCBed component start its get-in animation (BedGetInAnim, BedSleepAnim) or, with no animation, just mark the bed as occupied
// Leaf: ?PerformAction@CCreatureAction_GetInBed@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848DB0, length 343 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager, bit 17 of +0x24, untested; 0x35 = CTCBed, bit 21 of +0x24, tested), operator new @ 0x00BFEA1A (0x34-byte
// mode, 12-byte count block), CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CAnimFilterOfGroup(const CCharString&) @ 0x0073E8F0,
// CCharString copy ctor @ 0x0099EC30, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0, CTCBed::
// StartGetInBedAnimation(long, long) @ 0x007469C0; count-block destroy fn 0x007180B0; CModeSleepBed vtable 0x01275760.
// Layout: retail CCreatureActionBase is 0xa8 bytes (the PDB header is 12 larger): PBed +0xa8, AnimGroup +0xb0, SleepAnimName +0xb4,
// BedGetInAnim +0xb8, BedSleepAnim +0xbc; CTCBed +0x0c Empty (the landed 0x007469C0 naming).
// Shape: GetTC written as the end/hit ternary leaves the bed component in ecx for the call; the mode ctor is inline (base ctor, vtable, filter, name copy, two zeroed longs); the counted pointer is built in the
// argument area with its null-checked count block (the mode was never dereferenced); IS_THING_ALIVE on PBed then HasTC/GetTC.
void __fastcall Destroy_007180b0(void* object);   // 0x007180B0
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAnimFilterOfGroup { public:                     // 12 bytes
    unsigned char _body[0xc];
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
};
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
struct CModeBase {                              // retail sizeof 0x1c
    virtual void v00();
    unsigned char _pad04[0x1c - 4];
    CModeBase(CThing* thing);                   // 0x0073EAC0
};
struct CModeSleepBed : CModeBase {              // retail sizeof 0x34, vtable 0x01275760
    CAnimFilterOfGroup AnimFilter;              // +0x1c
    CCharString SleepAnimName;                  // +0x28
    long GetInAnim;                             // +0x2c
    long SleepAnim;                             // +0x30
    CModeSleepBed(CThing* creature, const CCharString& group, const CCharString& animName)
        : CModeBase(creature), AnimFilter(group), SleepAnimName(animName), GetInAnim(0), SleepAnim(0) {}
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};
class CTCBed { public:
    unsigned char _pad[0xc];
    bool Empty;                                 // +0x0c
    void StartGetInBedAnimation(long getInAnim, long sleepAnim);   // 0x007469C0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_GetInBed : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PBed;               // +0xa8
    CCharString AnimGroup;                      // +0xb0
    CCharString SleepAnimName;                  // +0xb4
    long BedGetInAnim;                          // +0xb8
    long BedSleepAnim;                          // +0xbc
    void PerformAction();
};

void CCreatureAction_GetInBed::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CModeSleepBed* mode = new CModeSleepBed(PCreature.GetPItem(), AnimGroup, SleepAnimName);
    modeManager->AddMode(CCountedPointer<CModeBase>(mode));
    if (IS_THING_ALIVE(PBed)) {
        CThing* bedThing = PBed.GetPItem();
        if (bedThing->HasTC(TCI_BED)) {
            CTCBed* bed = ((CTCBed*)&bedThing->GetTC(TCI_BED));
            if (BedGetInAnim)
                bed->StartGetInBedAnimation(BedGetInAnim, BedSleepAnim);
            else
                bed->Empty = false;
        }
    }
}