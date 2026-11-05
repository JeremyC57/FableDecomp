// CTCCreatureModeManager::RemoveMode(unsigned long modeType) -- remove every mode of the type; when the current (front) mode was one of them it is told it is leaving and the new front mode is resumed (and the thing's look component re-picks its turning anims); the next idle mode is due in 30 seconds
// Leaf: ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073ABA0, 170 bytes (`ret 4`).
// Callees: the mode's virtual slots 1 GetModeType, 6 OnDeactivate (+0x18, as the landed ClearModes 0x0073AE70) and 5 OnActivate (+0x14,
// vtable 0x012673CC: the landed 0x0073AAC0), list<CCountedPointer<CModeBase>>::remove_if<CModeBase::CIsType> @ 0x0073E790 (the predicate
// by value: one dword), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCLook::UpdateTurningAnims @ 0x00754050,
// __ftol2 @ 0x00BFEA70, CWorld::GetWorldFrame() @ 0x0049D870 (static). Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550, 30.0f @ 0x012673C4.
// Layout: Ego_r.pdb CTCCreatureModeManager with the retail 4-byte STLport list: PModes +0x0c (sentinel node pointer; node
// {next, prev, CCountedPointer {object, block}}), TimeLeftForCurrentMode +0x10, TimeForNextIdleMode +0x14 (PDB +0x1c); CThing
// look component = interface 0x43 (bit 3 of the +0x28 word).
// Shape: the "was current" bool is preset false (bl); the list address is formed once (`lea esi`) for the front reads and the
// remove_if call; the idle delay `(long)(fps * 30.0f)` is a named local computed before GetWorldFrame (written inline in the sum
// VC7.1 calls GetWorldFrame first and folds the add into a `sub` of the negated product).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CWorld { public:
    static long __fastcall GetFrame();                        // 0x0049D870
};
class CTCLook { public:
    void UpdateTurningAnims();                          // 0x00754050
};
struct CModeBase {                                      // CTCCreatureModeManager::CModeBase
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnActivate();                          // slot 5 (+0x14) 0x0073AAC0
    virtual void OnDeactivate();                        // slot 6 (+0x18) 0x00739E60
    struct CIsType {                                    // remove_if predicate
        unsigned long Type;
        CIsType(unsigned long type) : Type(type) { }
    };
};
struct SCountBlock;
struct CModePointer {                                   // CCountedPointer<CModeBase>
    CModeBase*   m_pObject;
    SCountBlock* m_pCount;
    CModeBase* operator->() const { return m_pObject; }
};
struct SModeNode {
    SModeNode*   m_pNext;
    SModeNode*   m_pPrev;
    CModePointer m_data;                                // +0x08
};
struct CModeList {                                      // retail STLport list: the sentinel node pointer
    SModeNode* m_pHead;
    bool empty() const { return m_pHead->m_pNext == m_pHead; }
    CModePointer& front() { return m_pHead->m_pNext->m_data; }
    void remove_if(CModeBase::CIsType predicate);       // 0x0073E790
};
class CTCCreatureModeManager { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CModeList PModes;                                   // +0x0c
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    void RemoveMode(unsigned long modeType);
};

void CTCCreatureModeManager::RemoveMode(unsigned long modeType)
{
    bool removingCurrent = false;
    if (!PModes.empty() && PModes.front()->GetModeType() == modeType) {
        removingCurrent = true;
        PModes.front()->OnDeactivate();
    }
    PModes.remove_if(CModeBase::CIsType(modeType));
    if (removingCurrent && !PModes.empty()) {
        PModes.front()->OnActivate();
        if (ParentThing->HasTC(TCI_LOOK))
            ((CTCLook*)((CTCLook*)&ParentThing->GetTC(TCI_LOOK)))->UpdateTurningAnims();
    }
    long idleDelay = (long)(CMainGameComponent::GetConstantFPS() * 30.0f);
    TimeForNextIdleMode = CWorld::GetFrame() + idleDelay;
}