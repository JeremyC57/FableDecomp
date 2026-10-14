// CGroundNavigatorExternalInterface::GetPNavigatorObstacles() -- the obstacle record (+0x40) of the navigation component (kind 7) of the thing at +4, when the thing holds one; else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CD7B0, length 64 bytes.
// Callees: the component map LowerBound @ 0x0040F020 (kind 7 by address). Shape: a one-word frame for the kind temporary, the mask test `test cl,cl ; jns` on +0x20,
// the inlined `find` tail, the record address by `add eax,40h`.
struct CTCBase_Retail;
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long           m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the second instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key converted into a stack temporary), then end when the key is not there
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = the player creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word (a bool return would materialise `shr ; and 1`)
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }          // free inline: the pointer comes back in eax
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CTCCreatureNavigation_Retail {
    unsigned char _pad[0x40];
    unsigned char m_obstacles[4];                  // +0x40
};

struct CGroundNavigatorExternalInterface {
    long           m_0;
    CThing_Retail* m_pThing;                       // +0x04
    void* GetPNavigatorObstacles();
};

void* CGroundNavigatorExternalInterface::GetPNavigatorObstacles()
{
    CThing_Retail* t = m_pThing;
    if (t->HasTC(7))
        return ((CTCCreatureNavigation_Retail*)GetTC(t, 7))->m_obstacles;
    return 0;
}
