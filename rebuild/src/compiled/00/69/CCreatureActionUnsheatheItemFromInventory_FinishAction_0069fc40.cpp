// CCreatureAction_UnsheatheItemFromInventory::FinishAction() -- make sure the action was performed, run the base FinishAction, hand the creature the queued follow-up action (telling the follow-up when the creature refuses it), then notify the creature's mode manager (interface type 0x31)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FC40, length 136 bytes.
// Callees: CCreatureActionBase::GetPerformedAction @ 0x006925A0 (`mov al,[ecx+62h] ; ret`, manifest "GetCanItemBeQuickAssigned"
// is propagated), CCreatureActionBase::PerformAction (vtable slot 3, +0x0c), CCreatureActionBase::FinishAction @ 0x00694430
// (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice),
// CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, the follow-up action's vtable slot 52 (+0xd0)
// when refused, CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset
// dword at +0x24), CTCCreatureModeManager method @ 0x0073A840 (manifest "CNavQuadTreeNode::RemoveNeighbours" is propagated).
// Layout (Ego_r.pdb, retail offsets 0xc lower than the PDB build): PUnsheatheItem CIntelligentPointer<CThing> at +0xa8,
// PFollowUpAction CCountedPointer<CCreatureActionBase> at +0xb0 (object) / +0xb4 (count). The follow-up is re-read from
// the member for the refusal call (`mov ecx,[esi+0b0h]`). Modelled locally (the generated headers flatten the bases).
struct CTCBase;
struct CCreatureActionBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCreatureModeManager {
    void OnUnsheatheFinished();             // 0x0073A840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CCountedPointer {                    // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Object;            // +0
    long* RefCount;                         // +4
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void OnRejectedByCreature();    // slot 52 (+0xd0)
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    bool GetPerformedAction();              // 0x006925A0 (PerformedAction byte at +0x62)
    void FinishAction();                    // 0x00694430
};

struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    CBaseIntelligentPointer PUnsheatheItem; // +0xa8
    CCountedPointer PFollowUpAction;        // +0xb0
    void FinishAction();
};

void CCreatureAction_UnsheatheItemFromInventory::FinishAction()
{
    if (!GetPerformedAction())
        PerformAction();
    CCreatureActionBase::FinishAction();
    if (PFollowUpAction.Object) {
        if (!PCreature.GetPItem()->SetCurrentAction(PFollowUpAction.Object))
            PFollowUpAction.Object->OnRejectedByCreature();
    }
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31))
        modes->OnUnsheatheFinished();
}