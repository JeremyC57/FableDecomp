// CIsImposterHit::operator()() const -- true when the watched thing is alive, has creature mode 0x25 active in its mode manager (component kind 0x42) (the imposter has been hit)
// Leaf: ??RCIsImposterHit
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00963810, length 108 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls on the pointer at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (the inlined component lookup), CTCCreatureModeManager::IsModeActive @ 0x00811340 (landed;
// manifest label "CPlayer::HasMode" is propagated).
// Model: the out-parameter GetInterface template of the landed CCreatureAction_JackFall::FinishAction 0x008C2490 (the
// component stays in eax: `mov eax,[eax+4] ; push ; mov ecx,eax`); kind 0x42 is bit 2 of the presence word at +0x28.
// Shape: `P.GetPItem() && !P.GetPItem()->IsDead()` re-calls the getter; the third call's thing serves both the presence test
// and the lookup; the two-exit `mov al,1` / `xor al,al` tail.
class CTCBase;
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
class CTCCreatureModeManager { public:                    // component kind 0x42 here
    bool IsModeActive(long mode) const;            // 0x00811340 (landed)
};
struct CIsImposterHit {
    unsigned char           _pad00[0x14];
    CIntelligentPointer<CThing> PImposter;             // +0x14 CIntelligentPointer<CThing>
    bool operator()() const;
};

bool CIsImposterHit::operator()() const
{
    if (PImposter.GetPItem() && !PImposter.GetPItem()->IsDead()) {
        CTCCreatureModeManager* modes;
        if (PImposter.GetPItem()->GetTC(TCI_ENTITY_EVENTS, &modes) && modes->IsModeActive(0x25))
            return true;
    }
    return false;
}