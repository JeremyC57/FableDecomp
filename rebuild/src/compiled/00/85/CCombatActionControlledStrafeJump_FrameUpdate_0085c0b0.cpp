// CCombatAction_ControlledStrafeJump::FrameUpdate() -- base FrameUpdate, then once the action is ready for interruption, drop the roll mode (70) the jump added and clear the flag
// Leaf: ?FrameUpdate@CCombatAction_ControlledStrafeJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085C0B0, 112-byte slot (108 bytes of code). FrameUpdate is vtable slot 56.
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call), virtual IsActionReadyForInterruption
//          (slot 27, `call [eax+6Ch]`, CCombatActionBase @ 0x00856170), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
//          (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31 = bit 17 of the
//          bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0.
// Shape: the landed GetInterface out-parameter template (0x008CB4A0 family): one `push ecx` frame word for
// the lookup key, the interface map addressed once through esi. AddedRollMode (a bool at retail +0x12c,
// PDB +0x144) is cleared whether or not the creature still has a mode manager.
class CTCBase;
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
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
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual bool IsActionReadyForInterruption() const;   // slot 27: 0x00856170
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
public:
    virtual void FrameUpdate();   // 0x00859D80
};

class CCombatAction_ControlledStrafeJump : public CCombatActionBase { public:   // retail sizeof 0x130
    float RequiredFacing_X, RequiredFacing_Y, RequiredFacing_Z;    // +0x114
    float OriginalFacing_X, OriginalFacing_Y, OriginalFacing_Z;    // +0x120
    bool AddedRollMode;                     // +0x12c
    void FrameUpdate();
};

void CCombatAction_ControlledStrafeJump::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (IsActionReadyForInterruption() && AddedRollMode) {
        CTCCreatureModeManager* pModes;
        if (PCreature.GetPItem()->GetInterface(&pModes, 0x31))
            pModes->RemoveMode(70);
        AddedRollMode = false;
    }
}