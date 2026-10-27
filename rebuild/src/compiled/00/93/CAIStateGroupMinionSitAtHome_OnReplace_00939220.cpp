// CAIStateGroup_MinionSitAtHome::OnReplace() -- CAIStateGroup_MinionSitOnSeatBase::OnReplace, then ask the creature's idle scheduler for a new idle
// Leaf: ?OnReplace@CAIStateGroup_MinionSitAtHome@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00939220, 75 bytes, thiscall, plain `ret`.
// Identity (proven): CAIStateGroup_MinionSitAtHome vtable 0x0128B6C4 slot 10 (OnReplace); Ego_r.pdb ?OnReplace@CAIStateGroup_MinionSitAtHome@@MAEXXZ (donor 0x0095C600);
//   RTTI bases CAIStateGroup_MinionSitAtHome : CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat : CAIStateGroupBase.
// Callees: CAIStateGroup_UsesSeat::OnReplace() 0x009387F0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCIdleScheduler::ChooseNewIdle()
//   0x00801A50.
// Shape (family K045, representative 0x00917D60): the qualified base call, then the landed CAIStateGroup_MinionWatchFires::OnReplace
//   0x00936590 body (its model, verbatim: GetInterface on PBrain->PCreature, interface 0xD3 = bit 19 of the dword at +0x38).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    CAIBrain* PBrain;                           // +0x04
};
class CTCIdleScheduler { public:                // interface type 0xD3 component
    void ChooseNewIdle();                       // 0x00801A50
};
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {
public:
    virtual void OnReplace();                   // 0x009387F0
};
class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat { };            // does not override OnReplace
class CAIStateGroup_MinionSitAtHome : public CAIStateGroup_MinionSitOnSeatBase {
protected:
    virtual void OnReplace();
};
void CAIStateGroup_MinionSitAtHome::OnReplace()
{
    CAIStateGroup_MinionSitOnSeatBase::OnReplace();
    CTCIdleScheduler* pScheduler;
    if (PBrain->PCreature->GetInterface(&pScheduler, 0xD3))
        pScheduler->ChooseNewIdle();
}