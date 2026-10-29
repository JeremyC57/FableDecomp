// CInputProcessZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- Z-target button: on press (game action 0x4a) lock onto the live hero creature's current targeting target, arm the hold timer (15) and mark the input handled; while held, if the creature still targets the locked thing and the timer is armed, disarm it and deactivate the Z-target
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688EB0, length 404 bytes (`ret 8`). Vtable 0x01238028 slot 4.
// Callees: the player-interface gate @ 0x00445B80 (manifest "CTCShotDefinitionBase::CanBeHitByMeleeStrike" is propagated);
//   CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90 / IsEventButtonHeld @ 0x00687BD0 / IsEventGameAction @ 0x00687CF0;
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10; the interface map's
//   LowerBound @ 0x004365B0; the target-pointer inequality @ 0x00689270 (`__fastcall`, `GetPItem() != thing`);
//   CProcessedInput::SetHandled @ 0x00A0D390; DeactivateZTarget @ 0x00688BA0; the targeting interface's slot 33 (+0x84).
// Layout: Ego_r.pdb CInputProcessZTarget (0x40): CInputProcessGameBase (PGamePlayerInterface +0x24), PCreature +0x28,
//   PHeroCombatDef +0x30, DeactivateHoldTimer +0x34, PTarget +0x38. CThing interface bits +0x20 / map +0x44 (interface 8) as the
//   landed CTCWound ctor 0x00757680; CIntelligentPointer as the landed 0x008C99C0 (temporary vtable 0x01237F1C).
// Shape: IS_THING_ALIVE inlined (three GetPItem calls), the GetInterface out-parameter idiom; the target assignment goes through
//   a scoped stack CIntelligentPointer (destroyed before the timer store and SetHandled).
class CInputEvent;
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
struct CTCHeroTargeting {                          // interface 8 (descriptive)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetTargetThing();              // slot 33 (+0x84) (descriptive)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                               // +4
    CBaseIntelligentPointer();                     // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
bool __fastcall operator!=(const CBaseIntelligentPointer& pointer, const CThing* thing);   // 0x00689270
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CGamePlayerInterface { public:
    bool IsInputEnabled();                         // 0x00445B80 (descriptive)
};
class CWorld; class CPlayer; class CGameDefinitionManager; class CMainGameComponent;
class CInputProcessGameBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // slot 4
    unsigned char           _pad04[0x10];
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessZTarget : public CInputProcessGameBase {
public:
    CBaseIntelligentPointer PCreature;             // +0x28
    void*                   PHeroCombatDef;        // +0x30
    long                    DeactivateHoldTimer;   // +0x34
    CBaseIntelligentPointer PTarget;               // +0x38
    void DeactivateZTarget(CProcessedInput& processed);   // 0x00688BA0
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!PGamePlayerInterface->IsInputEnabled())
        return;
    if (IsEventButtonPressed(event) && IsEventGameAction(0x4a, event)) {
        CThing* target = 0;
        CTCHeroTargeting* targeting;
        if (IS_THING_ALIVE(PCreature) && PCreature.GetPItem()->GetInterface(&targeting, 8))
            target = targeting->GetTargetThing();
        {
            CIntelligentPointer pointer;
            pointer.SetPItem(target);
            PTarget.SetPItem(pointer.GetPItem());
        }
        DeactivateHoldTimer = 15;
        processed.SetHandled();
    }
    if (IsEventButtonHeld(event) && IsEventGameAction(0x4a, event)) {
        CTCHeroTargeting* targeting;
        if (IS_THING_ALIVE(PCreature) && PCreature.GetPItem()->GetInterface(&targeting, 8)) {
            if (!(PTarget != targeting->GetTargetThing()) && DeactivateHoldTimer != -1) {
                DeactivateHoldTimer = -1;
                DeactivateZTarget(processed);
            }
        }
    }
}