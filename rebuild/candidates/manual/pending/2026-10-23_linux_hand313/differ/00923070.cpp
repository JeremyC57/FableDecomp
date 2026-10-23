// CAIStateGroup_ServeFood::IsRunnable() -- an idle, awake worker whose workplace has a live tavern: when serving, find a free table to serve (mode 1), else a table to clear or (mode 2 off-duty) a seat to eat at; on success remember the table
// Grade: DIFFER(1184v1184) -- frame 0xc v 0x10: retail shares the 0x21 lookup-type word with the tavern spill
// Layout: local model (no engine header); PTable +0x18, TaskIndex +0x1c, ServePos +0x20, ServeRHSet +0x2c, SeatIndex +0x44, Mode +0x48.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0 dead, bit 5 limbo
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
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
struct CThingCreatureBase : CThing {
    long GetLifeState();                        // 0x00661FF0
    long GetSleepState();                       // 0x00662000
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CThingPhysical;
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<const CThingPhysical>& p);   // 0x004CC340
struct CTCTavernTable {                         // interface 0xB8
    unsigned char _pad[0x0c];
    bool Occupied;                              // +0x0c
    unsigned char _pad0d[3];
    struct CEaters {
        CThing** _M_start;
        CThing** _M_finish;
        CThing** _M_end_of_storage;
        CThing** begin() { return _M_start; }
        unsigned long size() const { return _M_finish - _M_start; }
    } Eaters;                                   // +0x10 vector<CThingCreatureBase*>
    bool GetServingPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rh, long* seat, long* task, long* side, long mode, bool serve);   // 0x007B8CC0
    bool GetEatingPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rh, long* seat, long* task);   // 0x007B8B10
    void FreeSeat(long task, long seat);        // 0x007B8AA0
};
struct STableNode {
    STableNode* next;
    STableNode* prev;
    CIntelligentPointer<const CThingPhysical> Table;
};
struct CTCTavern {                              // interface 0x21
    unsigned char _pad[0x0c];
    STableNode* Tables;                         // +0x0c list sentinel
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsOnDuty();                    // slot 14 (+0x38)
    virtual CThing* GetPWorkplace();            // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
};
struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    unsigned char _pad14[4];
    CThing* PTable;                             // +0x18
    long TaskIndex;                             // +0x1c
    C3DVector ServePos;                         // +0x20
    CRightHandedSet ServeRHSet;                 // +0x2c
    long SeatIndex;                             // +0x44
    long Mode;                                  // +0x48
    bool IsRunnable();
};

bool CAIStateGroup_ServeFood::IsRunnable()
{
    if (PBrain->PCreature->GetLifeState() != 2)
        return false;
    if (PBrain->PCreature->GetSleepState() != 2)
        return false;
    CThing* workplace = GetPWorkplace();
    if (workplace == 0 || (workplace->m_flags91 & 1) || !workplace->HasTC(0x21))
        return false;
    CTCTavern* tavern = workplace->GetTC<CTCTavern>(0x21);
    long side;
    if (IsOnDuty()) {
        for (STableNode* node = tavern->Tables->next; node != tavern->Tables; node = node->next) {
            CIntelligentPointer<const CThingPhysical>& table = node->Table;
            CTCTavernTable* comp;
            if (IS_THING_ALIVE(table) && !(table.GetPItem()->m_flags91 & 0x20) && table.GetPItem()->HasTC(0xb8)
                && table.GetPItem()->GetInterface(&comp, 0xb8) && !comp->Occupied && !table.GetPItem()->HasTC(0xcf)) {
                Mode = 1;
                if (!comp->Occupied) {
                    CThingCreatureBase* creature = PBrain->PCreature;
                    if (comp->GetServingPoint(creature, &ServePos, &ServeRHSet, &SeatIndex, &TaskIndex, &side, 1, true)) {
                        PTable = node->Table.GetPItem();
                        return true;
                    }
                }
                if (0) {
                    PTable = node->Table.GetPItem();
                    return true;
                }
            }
        }
        for (STableNode* node = tavern->Tables->next; node != tavern->Tables; node = node->next) {
            CIntelligentPointer<const CThingPhysical>& table = node->Table;
            CTCTavernTable* comp;
            if (table.GetPItem() != 0 && !(table.GetPItem()->m_flags91 & 1) && !(table.GetPItem()->m_flags91 & 0x20)
                && table.GetPItem()->HasTC(0xb8) && table.GetPItem()->GetInterface(&comp, 0xb8) && !table.GetPItem()->HasTC(0xcf)) {
                for (unsigned long i = 0; i < comp->Eaters.size(); ++i) {
                    if (*(comp->Eaters.begin() + i) == PBrain->PCreature)
                        return false;
                }
                CThingCreatureBase* creature = PBrain->PCreature;
                if (comp->GetEatingPoint(creature, &ServePos, &ServeRHSet, &SeatIndex, &TaskIndex)) {
                    comp->FreeSeat(TaskIndex, SeatIndex);
                    TaskIndex = -1;
                    SeatIndex = -1;
                    return false;
                }
            }
        }
        for (STableNode* node = tavern->Tables->next; node != tavern->Tables; node = node->next) {
            CIntelligentPointer<const CThingPhysical>& table = node->Table;
            CTCTavernTable* comp;
            if (table.GetPItem() != 0 && !(table.GetPItem()->m_flags91 & 1) && !(table.GetPItem()->m_flags91 & 0x20)
                && table.GetPItem()->HasTC(0xb8) && table.GetPItem()->GetInterface(&comp, 0xb8) && !comp->Occupied
                && !table.GetPItem()->HasTC(0xcf)) {
                Mode = 1;
                if (!comp->Occupied) {
                    CThingCreatureBase* creature = PBrain->PCreature;
                    if (comp->GetServingPoint(creature, &ServePos, &ServeRHSet, &SeatIndex, &TaskIndex, &side, 1, false)) {
                        PTable = node->Table.GetPItem();
                        return true;
                    }
                }
                if (0) {
                    PTable = node->Table.GetPItem();
                    return true;
                }
            }
        }
        return false;
    }
    for (STableNode* node = tavern->Tables->next; node != tavern->Tables; node = node->next) {
        CIntelligentPointer<const CThingPhysical>& table = node->Table;
        CTCTavernTable* comp;
        if (IS_THING_ALIVE(table) && !(table.GetPItem()->m_flags91 & 0x20) && table.GetPItem()->HasTC(0xb8)
            && table.GetPItem()->GetInterface(&comp, 0xb8) && !table.GetPItem()->HasTC(0xcf)) {
            Mode = 2;
            if (!comp->Occupied) {
                CThingCreatureBase* creature = PBrain->PCreature;
                if (comp->GetServingPoint(creature, &ServePos, &ServeRHSet, &SeatIndex, &TaskIndex, &side, 2, false)) {
                    PTable = node->Table.GetPItem();
                    return true;
                }
            }
            if (0) {
                PTable = node->Table.GetPItem();
                return true;
            }
        }
    }
    return false;
}
