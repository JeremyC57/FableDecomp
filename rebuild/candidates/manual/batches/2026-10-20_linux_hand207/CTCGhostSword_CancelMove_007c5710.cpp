// CTCGhostSword::CancelMove() -- stop the interface-0x9B timer component of every live ghost-sword minion once (copy its target into its current value and set its stopped flag)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C5710, length 175 bytes (+ int3 padding). __thiscall, `ret`.
//         Ego_r.pdb: `?CancelMove@CTCGhostSword@@QAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE pair, then the lookup thing),
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x9B, word 4 bit 27).
// Shape: an unsigned index loop over Minions (`size()` re-read each pass, `jb`), each element reached through the
//        STLport `*(begin() + i)` operator[] (the scaled index kept in edi, begin() re-read for the third GetPItem);
//        the timer is the out-parameter GetInterface; the stop is the landed CTCSummonSpell::CancelMove (0x0079A820) step.
// Layout: retail CTCGhostSword: PDef +0x0C, Minions +0x10 (12-byte STLport vector of CIntelligentPointer<CThingAICreature>).
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
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

struct CBaseIntelligentPointer {                    // CIntelligentPointer<CThingAICreature>: 8 bytes
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCMinionTimer {                             // interface 0x9B component
    unsigned char _pad00[0x0c];
    bool  Stopped;                                  // +0x0C
    unsigned char _pad0d[7];
    long  Current;                                  // +0x14
    long  Target;                                   // +0x18
};

struct CTCGhostSword {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void* PDef;                                     // +0x0C CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CBaseIntelligentPointer> Minions; // +0x10
    void CancelMove();
};

void CTCGhostSword::CancelMove()
{
    for (unsigned long i = 0; i < Minions.size(); ++i) {
        if (IS_THING_ALIVE(Minions[i])) {
            CTCMinionTimer* timer;
            if (Minions[i].GetPItem()->GetInterface(&timer, 0x9b)) {
                if (!timer->Stopped) {
                    timer->Current = timer->Target;
                    timer->Stopped = true;
                }
            }
        }
    }
}
