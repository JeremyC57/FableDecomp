// CCombatAction_ControlledStrafeJump::FinishAction() -- base FinishAction, then if the jump added the roll mode, remove mode 70 from the creature's mode manager and clear the flag
// Leaf: ?FinishAction@CCombatAction_ControlledStrafeJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C120, 112-byte slot (97 bytes of code). FinishAction is vtable slot 4 (protected).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
//          (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31 = bit 17 of the
//          bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Shape: the landed GetInterface out-parameter template (0x008CB4A0 family): one `push ecx` frame word for
// the lookup key, the interface map addressed once through esi. AddedRollMode (a bool at retail +0x12c,
// PDB +0x144) is cleared whether or not the creature still has a mode manager.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCombatAction_ControlledStrafeJump : public CCombatActionBase { public:   // retail sizeof 0x130
    float RequiredFacing_X, RequiredFacing_Y, RequiredFacing_Z;    // +0x114
    float OriginalFacing_X, OriginalFacing_Y, OriginalFacing_Z;    // +0x120
    bool AddedRollMode;                     // +0x12c
    virtual void FinishAction();
};

void CCombatAction_ControlledStrafeJump::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (AddedRollMode) {
        CTCCreatureModeManager* pModes;
        if (PCreature.GetPItem()->GetInterface(&pModes, 0x31))
            pModes->RemoveMode(70);
        AddedRollMode = false;
    }
}