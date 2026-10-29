// CCreatureAction_SheatheItemToInventory::FinishAction() -- perform the action if it never ran, base FinishAction, hand the follow-up action to the creature (cancelling it when refused), then let the mode manager (interface type 0x31) run its front mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F670, length 136 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetPerformedAction @ 0x006925A0 (`mov al,[ecx+62h]`: the PerformedAction byte; the manifest
// "CTCInventoryBase::GetCanItemBeQuickAssigned" label is propagated), PerformAction = vtable slot 3 (`call [eax+0Ch]`),
// CCreatureActionBase::FinishAction @ 0x00694430 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0 (bool), the follow-up action's vtable slot 52 (`call [edx+0D0h]`),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::UpdateFrontMode @ 0x0073A840 (landed as CTCCreatureModeManager::func_0073a840).
// Shape: the follow-up action is the object word of a counted pointer at +0xa8 (the block word at +0xac, see 0x006DD980), read again from
// the member for the virtual call (`mov ecx,[esi+0A8h]`); the mode-manager call sits inside the GetInterface if, so the miss path
// returns directly. `this` rides in esi until the lookup re-uses it for the map.
class CTCBase;
class CCreatureActionBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseObject* GetPItem() const;               // 0x00A01B50
};
class CTCCreatureModeManager { public:             // interface type 0x31 component
    void UpdateFrontMode();                 // 0x0073A840
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    virtual void FinishAction();            // slot 4, 0x00694430
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void Cancel();                  // slot 52 (+0xd0)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x62 - 0x10];      // +0x10
    bool PerformedAction;                   // +0x62
    unsigned char _pad63[0xa8 - 0x63];
    bool GetPerformedAction() const;        // 0x006925A0
};
struct SCountedBlock;
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    CCreatureActionBase* m_pNextAction;     // +0xa8 CCountedPointer<CCreatureActionBase>: object word
    SCountedBlock* m_pNextActionBlock;      // +0xac                                      count block
    virtual void FinishAction();
};

void CCreatureAction_SheatheItemToInventory::FinishAction()
{
    if (!GetPerformedAction())
        PerformAction();
    CCreatureActionBase::FinishAction();
    if (m_pNextAction) {
        if (!((CThing*)PCreature.GetPItem())->SetCurrentAction(m_pNextAction))
            m_pNextAction->Cancel();
    }
    CTCCreatureModeManager* pModes;
    if (((CThing*)PCreature.GetPItem())->GetInterface(&pModes, 0x31))
        pModes->UpdateFrontMode();
}