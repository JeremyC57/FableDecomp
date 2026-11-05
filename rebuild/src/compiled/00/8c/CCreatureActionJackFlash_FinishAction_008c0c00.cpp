// CCreatureAction_JackFlash::FinishAction() -- for a flagged target, push a new CModeJackOfBladesFlash (aimed at the target) on the creature's mode manager and suspend a player creature's animation; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_JackFlash@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C0C00, 270 bytes (272 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined unchecked lookup of 0x31 = CTCCreatureModeManager, bit 17 of +0x24), operator new 0x00BFEA1A (0x98 bytes),
//   NCreatureMode::CModeJackOfBladesFlash(CThing*) 0x008D7E60, CCountedPointer<CModeJackOfBladesFlash>(T*) 0x008C3260
//   (out of line), the mode's SetTarget(CThing*) 0x008D7320 (manifest "AssignCreatureToTask" is propagated),
//   CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0, CThingAICreature::SuspendAI(bool,
//   long) 0x00833A10, operator delete 0x00BFE9BC, CCombatActionBase::FinishAction 0x00859A20.
// Layout: retail CCombatActionBase is 0x114 bytes: PTarget +0xa8; CThing +0x10 the type byte (1 here: a player), +0x6c
//   flag bit 3.
// Shape: the target flag is read without a null test; the target is a named local; the mode manager comes from the
//   unchecked out-parameter lookup; the counted pointer is a named local (ctor out of line) copied by value into AddMode
//   (inline copy: object, count, addref) and released inline at the block end (count block destroy + delete);
//   `this` is spilled to the frame because edi is reused for &PCreature; the player creature is a named local so it is
//   fetched before SuspendAI's argument pushes.
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
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
    long references;
    void (__fastcall* destroy)(void*);
    void* object;
};
struct CModeBase { };
template <class T> class CCountedPointer { public:
    T* object;
    SCountBlock* count;
    CCountedPointer(T* value);                        // 0x008C3260 (CModeJackOfBladesFlash instantiation)
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : object(other.object), count(other.count)
    {
        if (count)
            ++count->references;
    }
    ~CCountedPointer()
    {
        if (count) {
            if (--count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
    }
    T* operator->() const { return object; }
};
struct CModeJackOfBladesFlash : CModeBase {
    unsigned char _pad[0x98];
    CModeJackOfBladesFlash(CThing* owner);            // 0x008D7E60
    void SetTarget(CThing* target);                   // 0x008D7320
};
void __fastcall DestroyJackOfBladesFlash(void* object);   // 0x008C2D70-family per-type deleter stored in the count block
#pragma auto_inline(off)
// Same-TU stand-in for the counted-pointer constructor 0x008C3260 (its landed body, CCountedPointer008c2d70): compiled
// above the caller so VC7.1 knows the pointer it fills does not escape (retail's LTCG knowledge) -- the object and the
// count block then stay in esi and the 12-byte frame matches.
template <> CCountedPointer<CModeJackOfBladesFlash>::CCountedPointer(CModeJackOfBladesFlash* value)
    : object(value)
{
    if (value) {
        SCountBlock* block = new SCountBlock;
        block->references = 1;
        block->destroy = DestroyJackOfBladesFlash;
        block->object = object;
        count = block;
    } else
        count = 0;
}
#pragma auto_inline(on)
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);    // 0x0073ACC0
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:                      // slot 4: 0x00859A20
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
};
class CCreatureAction_JackFlash : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_JackFlash::FinishAction()
{
    if (PTarget.GetPItem()->GetTypeFlags() & 8) {
        CThing* target = PTarget.GetPItem();
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        CCountedPointer<CModeJackOfBladesFlash> mode(new CModeJackOfBladesFlash(PCreature.GetPItem()));
        mode->SetTarget(target);
        modes->AddMode(mode);
        if (PCreature.GetPItem()->GetType() == 1) {
            CThing* creature = PCreature.GetPItem();
            ((CThingAICreature*)creature)->SuspendAI(true, 0);
        }
    }
    CCombatActionBase::FinishAction();
}