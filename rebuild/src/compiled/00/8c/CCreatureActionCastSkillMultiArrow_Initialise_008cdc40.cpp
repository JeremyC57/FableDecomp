// CCreatureAction_CastSkillMultiArrow::Initialise() -- base Initialise, then make sure the owner has a CTCMultiArrow component (adding "CTCMultiArrow" and, for an AI-state owner, setting its AI cast level) before looking it up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDC40, length 168 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("CTCMultiArrow" @ 0x0123D364), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
// CTCMultiArrow::SetAICast(long) 0x007CBF20.
// Shape: an unrotated top-test loop (the __forceinline early-return LacksTC condition stops VC rotating it) re-testing
// presence bit 0x88 (dword +0x30, bit 8) of a freshly fetched owner; the final lookup is an inline
// std::lower_bound over the {type, component} vector at +0x44/+0x48 (8-byte entries) whose result is unused (the loop keeps it).
// Layout: Level at retail +0x114 (PDB +0x12c); thing byte +0x10 = state (1: AI controlled).
class CTCBase;
class CTCParamsBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CTCMultiArrow { public:
    void SetAICast(long level);                 // 0x007CBF20
};

struct STCEntry { long type; CTCBase* component; };

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    STCEntry*     m_tcFirst;                    // +0x44
    STCEntry*     m_tcLast;                     // +0x48
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline bool LacksTC(long interfaceType) const
    {
        if (HasTC(interfaceType)) return false;
        return true;
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
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
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

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CIntelligentPointer<CThing> Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0x114 - 0x10];
    virtual void Initialise();                  // 0x00857520
};

class CCreatureAction_CastSkillMultiArrow : public CCombatActionBase { public:
    long Level;                                 // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastSkillMultiArrow::Initialise()
{
    CCombatActionBase::Initialise();
    CThing* owner = Owner.GetPItem();
    while (owner->LacksTC(0x88)) {
        CTCMultiArrow* tc = (CTCMultiArrow*)Owner.GetPItem()->AddTC("CTCMultiArrow", true, 0);
        if (Owner.GetPItem()->m_state == 1)
            tc->SetAICast(Level);
        owner = Owner.GetPItem();
    }
    owner->LowerBoundTC(0x88);
}