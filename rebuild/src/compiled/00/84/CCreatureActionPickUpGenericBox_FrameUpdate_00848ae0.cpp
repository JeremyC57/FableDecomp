// CCreatureAction_PickUpGenericBox::FrameUpdate() -- base FrameUpdate; finish the action when the box is gone or dead, or when its CTCCarryable (type 0x47) component has a carrier and the creature cannot quick-assign it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848AE0, length 143 bytes.
// Model: rebuild/include/engine/CCreatureAction_PickUpGenericBox.h is the flattened PDB layout and sits high against the bytes (CCreatureActionBase is 0xb4 bytes in the PDB, 0xa8 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CCreatureActionBase::FrameUpdate @ 0x00694020 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the box
// pointer at +0xa8, three times), CVectorMap::LowerBound @ 0x004365B0 (a second instantiation of the interface-map lookup; inlined
// GetInterface for type 0x47 = bit 7 of the presence byte at +0x28, `test cl,cl ; jns`), the action's own check @ 0x006925A0 (manifest
// "CTCInventoryBase::GetCanItemBeQuickAssigned", called on `this`), this action's virtual slot 4 (`call [edx+10h]`, FinishAction).
// Shape: IS_THING_ALIVE inlined; the GetInterface result is ignored (absent arm reads the key's frame word); the FinishAction call is
// duplicated into the dead/absent arm placed after the main path.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_flags91;                // +0x91 bit 0: dead
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCCarrying;
class CTCCarryable {                        // interface type 0x47 (vtable 0x0123B35C slot 21: `push 47h ; pop eax`)
public:
    unsigned char _pad00[0x10];
    CTCCarrying* PCarryingTC;               // +0x10 (Ego_r.pdb CTCCarryable; the carrier, null when not carried)
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[0xa8 - 4];
    void FrameUpdate();                     // 0x00694020
    bool GetCanItemBeQuickAssigned();       // 0x006925A0
};
struct CCreatureAction_PickUpGenericBox : CCreatureActionBase {
    CBaseIntelligentPointer PBox;           // +0xa8 the box being picked up
    void FrameUpdate();
};

void CCreatureAction_PickUpGenericBox::FrameUpdate()
{
    CCreatureActionBase::FrameUpdate();
    if (IS_THING_ALIVE(PBox)) {
        CTCCarryable* carryable;
        PBox.GetPItem()->GetInterface(&carryable, 0x47);
        if (carryable->PCarryingTC && !GetCanItemBeQuickAssigned())
            FinishAction();
    } else {
        FinishAction();
    }
}