// NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter() -- count the imposters still usable, pick one of them with the world's random seed and return its thing; null when none is usable
// Leaf: GetRandomImposter
// Grade: DIFFER(ours v retail) -- register allocation only: the second loop keeps the index in edi and the element address in ebp (retail ebp / edi); tried unsigned/signed index and a named element reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7F80, 223 bytes (`ret`).
// Callees: the imposter predicate 0x008E8A10 (true for a dead/missing imposter or one no longer in creature mode 0x3f;
// manifest "CAIStateGroup_VersusScorpionKing::FindChargeEvent" is a byte-twin label); CModeBase::GetWorld 0x0073A680
// (`mov eax,[game] ; mov eax,[eax+24h]`; manifest "CViewBase::GetDisplayEngine" is propagated); CWorld::GetPRandomSeed 0x0049D810
// (manifest "CVertexBufferWin32::DoSizeof" is propagated); GFROR13 0x00497890 (`__fastcall`); CBaseIntelligentPointer::GetPItem
// 0x00A01B50.
// Layout (retail, see the ctors 0x00741EE0 / 0x008E78C0): Imposters +0x80 (12-byte STLport vector of CIntelligentPointer<CThing>).
// Shape: the count loop runs over cached begin/end pointers with the predicate object in the dead `push ecx` frame word; the
// pick is the landed WorldRandomRange inline (0x006E8730: LCG step, ROR13, `range ? seed % range : 0`); the second loop indexes
// the vector and re-reads size() each step, counting down the pick over the live imposters (IS_THING_ALIVE re-calls GetPItem).
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;               // +0x91 bit 0: dead
};

extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CWorld {
    unsigned long* GetPRandomSeed();       // 0x0049D810
};

struct CBaseIntelligentPointer {           // vptr + item
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;              // 0x00A01B50
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CIsImposterGone {                   // empty predicate object
    bool operator()(const CIntelligentPointerThing& imposter) const;   // 0x008E8A10
};

template <class T> struct vector {         // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    unsigned char _body[0x64];
    CWorld* GetWorld();                    // 0x0073A680
};

namespace NCreatureMode {

struct CModeBriarRoseConcentrate : CModeStandBase {   // vtable 0x01268548
    unsigned char                    _members[0x18];  // Filter +0x68, Timer +0x74, PTarget +0x78
    vector<CIntelligentPointerThing> Imposters;       // +0x80
    CThing* GetRandomImposter();
};

CThing* CModeBriarRoseConcentrate::GetRandomImposter()
{
    long usable = 0;
    CIsImposterGone gone;
    CIntelligentPointerThing* last = Imposters._M_finish;
    for (CIntelligentPointerThing* it = Imposters._M_start; it != last; ++it)
        if (!gone(*it))
            ++usable;
    if (usable > 0) {
        unsigned long* seed = GetWorld()->GetPRandomSeed();
        long pick = WorldRandomRange(*seed, usable);
        for (unsigned long i = 0; i != Imposters.size(); ++i) {
            if (IS_THING_ALIVE(Imposters[i])) {
                if (pick-- == 0)
                    return Imposters[i].GetPItem();
            }
        }
    }
    return 0;
}

}
