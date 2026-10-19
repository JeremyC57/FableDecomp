// CCreatureAction_ShowTrophy::FinishAction() -- kill the live trophy emitter, tell the shown trophy's CTCTrophy component (interface 0x39) that the owner showed it, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845850, length 176 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x39 = CTCTrophy, bit 25 of +0x24), the CTCTrophy member @ 0x006DD740 taking the showing creature
// (manifest label "$E2" is a propagation; descriptive name), CCreatureActionBase::FinishAction @ 0x00694430.
// Shape: two "present and not dead" (thing byte +0x91 bit 0) guards; the trophy's component lookup is the UNCHECKED
// out-parameter GetInterface in an inline accessor (the absent path reads the never-written out pointer from the
// lookup key's `push ecx` word).
// Layout: Ego_r.pdb PTrophyBeingShown +0xb4, ShowingTrophyDeedHandle +0xbc, PostedDeed +0xc0, PEmitter +0xc1 (packed);
// retail CCreatureActionBase is 0xa8, so the members sit 12 bytes lower (PEmitter at the odd offset +0xb5).
struct CTCBase;
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
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool silent);                     // 0x004C9B80
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

struct CTCTrophy {
    void OnShownByCreature(CThing* creature);   // 0x006DD740
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCreatureActionBase {
public:
    virtual void FinishAction();                // 0x00694430
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTrophyBeingShown;  // +0xa8 (PDB +0xb4)
    unsigned long           ShowingTrophyDeedHandle;   // +0xb0
    bool                    PostedDeed;         // +0xb4
    CBaseIntelligentPointer PEmitter;           // +0xb5 (packed)
    virtual void FinishAction();
    __forceinline CTCTrophy* GetTrophy() const
    {
        CTCTrophy* trophy;
        PTrophyBeingShown.GetPItem()->GetInterface(&trophy, 0x39);
        return trophy;
    }
};
#pragma pack(pop)

void CCreatureAction_ShowTrophy::FinishAction()
{
    if (PEmitter.GetPItem() && !(PEmitter.GetPItem()->m_flags91 & 1))
        PEmitter.GetPItem()->Kill(true);
    if (PTrophyBeingShown.GetPItem() && !(PTrophyBeingShown.GetPItem()->m_flags91 & 1))
        GetTrophy()->OnShownByCreature(PCreature.GetPItem());
    CCreatureActionBase::FinishAction();
}