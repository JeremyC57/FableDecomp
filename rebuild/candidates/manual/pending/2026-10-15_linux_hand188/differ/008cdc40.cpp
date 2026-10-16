// CCreatureAction_CastSkillMultiArrow::Initialise() -- base Initialise, then make sure the owner has a CTCMultiArrow component (adding "CTCMultiArrow" and, for an AI-state owner, setting its AI cast level) before looking it up
// Grade: DIFFER(183v168) -- retail keeps the add-until-present loop unrotated (test at the top, two `jmp` back-edges, edi pushed in the prologue); ours rotates it (duplicated GetPItem + bit test at the bottom, aligned loop head) | tried: for(;;)+return, while((owner=...)), goto | next: a tail-recursive inline helper (GetOrAddMultiArrowTC) whose recursion VC turns into the jmp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDC40, length 168 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("CTCMultiArrow" @ 0x0123D364), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
// CTCMultiArrow::SetAICast(long) 0x007CBF20.
// Shape: a loop re-testing presence bit 0x88 (dword +0x30, bit 8) of a freshly fetched owner; the final lookup is an inline
// std::lower_bound over the {type, component} vector at +0x44/+0x48 (8-byte entries) whose result is unused (the loop keeps it).
// Layout: Level at retail +0x114 (PDB +0x12c); thing byte +0x10 = state (1: AI controlled).
struct CTCBase;
struct CTCParamsBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CTCMultiArrow {
    void SetAICast(long level);                 // 0x007CBF20
};

struct STCEntry { long type; CTCBase* component; };

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    STCEntry*     m_tcFirst;                    // +0x44
    STCEntry*     m_tcLast;                     // +0x48
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline STCEntry* LowerBoundTC(long interfaceType)
    {
        STCEntry* first = m_tcFirst;
        long count = m_tcLast - m_tcFirst;
        while (count > 0) {
            long half = count / 2;
            if (first[half].type < interfaceType) {
                first = first + half + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0x114 - 0x10];
    virtual void Initialise();                  // 0x00857520
};

struct CCreatureAction_CastSkillMultiArrow : CCombatActionBase {
    long Level;                                 // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastSkillMultiArrow::Initialise()
{
    CCombatActionBase::Initialise();
    CThing* owner;
again:
    owner = Owner.GetPItem();
    if (!owner->HasTC(0x88)) {
        CTCMultiArrow* tc = (CTCMultiArrow*)Owner.GetPItem()->AddTC("CTCMultiArrow", true, 0);
        if (Owner.GetPItem()->m_state == 1)
            tc->SetAICast(Level);
        goto again;
    }
    owner->LowerBoundTC(0x88);
}
