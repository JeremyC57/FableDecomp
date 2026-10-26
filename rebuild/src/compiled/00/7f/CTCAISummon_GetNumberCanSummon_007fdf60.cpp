// CTCAISummon::GetNumberCanSummon() const -- how many creatures this summoner may summon now: none unless it is time to summon, else its def's NumberToSummon capped by its own limit (minus those it already has alive) and by the global summoned-creature budget; none if that falls below the def's MinimumSummon
// Leaf: ?GetNumberCanSummon@CTCAISummon@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FDF60, 210 bytes (+ int3 padding). Ego_r.pdb `?GetNumberCanSummon@CTCAISummon@@QBEJXZ`
//   (0xe0 bytes there); callers CTCAISummon::SummonCreatures 0x007FE040, CTCNymph::GetShouldSummon 0x0080FD30,
//   CAIStateGroup_SummonerSummon::IsRunnable 0x0096E540, 0x00905600. Catalog label `CTCAISummon_GetMaxCreatureCount`.
// Callees: CTCAISummon::IsTimeToSummon() const 0x007FDE00; the inline GetAllSummonedCreatures (landed out of line at
//   0x007FDEB0: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, list::remove 0x0065DE8A,
//   CTCAISummon::AllSummonedCreatures 0x013BAF14); the global summoned-creature budget long 0x013844AC (name unknown).
// Layout: CTCAISummon (Ego_r.pdb): PSummonDef +0x0c, MySummonedCreatures +0x10 (STLport list, one head-node word in retail);
//   CSummonDef: NumberToSummon +0x30, MySummonLimit +0x38, MinimumSummon +0x3c.
// Shape: STLport `min(a, b)` = `b < a ? b : a` by reference (the operands homed in frame slots, `lea eax,[esp+N] ; jl`),
//   the two walking size() loops inline, one shared `xor eax,eax` for both zero returns (the work inside
//   `if (IsTimeToSummon())`, `return 0` last). The def is read through CDefPointer's inline operator-> (a raw member
//   pointer reloads it from `this` after the size loop instead of keeping it in ebp), and the own-limit term is
//   `PSummonDef->MySummonLimit - size()` (the inline getters load the limit before the loop).
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                              // +0x04
    CBaseIntelligentPointer();                                  // 0x00A01B10
    ~CBaseIntelligentPointer();                                 // 0x00A01C10
    void SetPItem(CThing* item);                                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {                         // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    unsigned int size() const
    {
        unsigned int n = 0;
        for (SListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void remove(const T& value);                                // 0x0065DE8A for T = CIntelligentPointer<CThing>
};

namespace std {
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
}

class CSummonDef { public:
    unsigned char _pad00[0x28];
    long  CreatureToSummon;                                     // +0x28
    long  CreatureFamilyToSummon;                               // +0x2c
    long  NumberToSummon;                                       // +0x30
    float SummonRadius;                                         // +0x34
    long  MySummonLimit;                                        // +0x38
    long  MinimumSummon;                                        // +0x3c
    float TimeBetweenSummons;                                   // +0x40
};

template <class T> class CDefPointer { public:                         // the def pointer's inline accessor (one word in retail)
    T* Def;
    T* operator->() const { return Def; }
};

extern long g_MaxTotalSummonedCreatures;                        // 0x013844AC

class CTCAISummon {
public:
    void*                                  __vfptr;             // +0x00
    CThing*                                ParentThing;         // +0x04 (CTCBase)
    unsigned char                          _pad08[4];
    CDefPointer<const CSummonDef>          PSummonDef;          // +0x0c
    CRetailList<CIntelligentPointerThing>  MySummonedCreatures; // +0x10
    long                                   LastSummonTick;      // +0x14
    static CRetailList<CIntelligentPointerThing> AllSummonedCreatures;   // 0x013BAF14
    static CRetailList<CIntelligentPointerThing>& __fastcall GetAllSummonedCreatures()
    {
        AllSummonedCreatures.remove(CIntelligentPointerThing(0));
        return AllSummonedCreatures;
    }
    long GetNumberMySummonLimit() const { return PSummonDef->MySummonLimit; }
    long GetNumberMySummonedCreatures() const { return MySummonedCreatures.size(); }
    long GetNumberCanSummon() const;
private:
    bool IsTimeToSummon() const;                                // 0x007FDE00
};

long CTCAISummon::GetNumberCanSummon() const
{
    if (IsTimeToSummon()) {
        long globalRemaining = g_MaxTotalSummonedCreatures - GetAllSummonedCreatures().size();
        long number = PSummonDef->NumberToSummon;
        if (PSummonDef->MySummonLimit > 0)
            number = std::min(number, PSummonDef->MySummonLimit - (long)MySummonedCreatures.size());
        number = std::min(number, globalRemaining);
        if (number >= PSummonDef->MinimumSummon)
            return number;
    }
    return 0;
}