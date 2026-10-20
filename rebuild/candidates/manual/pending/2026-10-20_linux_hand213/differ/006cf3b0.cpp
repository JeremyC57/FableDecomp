// CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn() -- as the wedding fresco starts fading in, the wife stops following the hero: she is removed from his CTCFollowed list and from his CTCRegionFollower list when she is on them
// Grade: DIFFER(ours v retail) 206v206 -- in all four calls the ParentThing argument is loaded (ecx/edx) before the component load `mov esi,[eax+4]`; retail loads the component first and the parent into eax/ecx after it
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF3B0, 206 bytes (+2 int3). __fastcall, `ret`.
// Callees: CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetMainPlayer() 0x00449970; CPlayer::GetHeroThing() 0x00487DC0;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCFollowed::IsFollower(const CThing*) 0x006D4D70
//          (manifest "IsAIFollower"); CTCFollowed::RemoveFollower(CThing*) 0x006D5180; CTCRegionFollower::IsRegionFollower(CThing*)
//          0x006AEB60; CTCRegionFollower::RemoveRegionFollower(CThing*) 0x006AEBA0 (a list<CIntelligentPointer<CThing>>::remove
//          @ 0x0065DE8A on its +0x0c list; the manifest label "CCreatureActionBase::AddDependentThing" is propagated).
// Shape: hero fetched into a named local (edi) and tested alive; two independent `if (GetInterface(&tc, K))` blocks
//        (0xbc = CTCFollowed, bitset word 5 bit 28; 0x7a = CTCRegionFollower, word 3 bit 26) sharing the one `push ecx`
//        key word, each with the component in esi and `if (tc->Is..(ParentThing)) tc->Remove..(ParentThing)`.
// Layout: CTCWife ParentThing +0x04; retail CThing bitset +0x20, interface map +0x44, Dead +0x91.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const unsigned long& type);   // 0x0040F020 (the other TU copy is 0x004365B0; the key by address)
    __forceinline CTCBase* Find(long type)      // the inlined map `find`: lower bound on a key temporary, end when absent
    {
        SEntry* it = LowerBound((unsigned long)type);
        if (it == last || type < it->type)
            it = last;
        return it->component;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)m_interfaces.Find(interfaceType);
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CTCFollowed {
    bool IsFollower(const CThing* thing) const; // 0x006D4D70
    void RemoveFollower(CThing* thing);         // 0x006D5180
};
struct CTCRegionFollower {
    bool IsRegionFollower(CThing* thing);       // 0x006AEB60
    void RemoveRegionFollower(CThing* thing);   // 0x006AEBA0
};
__forceinline bool HasTC(CThing* thing, long type)
{
    if (thing->m_presentBits[type >> 5] & (1UL << (type & 31)))
        return true;
    return false;
}
template <class T> __forceinline T* GetTC(CThing* thing, long type)
{
    return (T*)thing->m_interfaces.Find(type);
}
struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    CPlayerManager* GetPlayerManager();          // 0x00686D40 (CTCBase)
    void InitWifeAtEndOfMarriageFrescoStartFadeIn();
};
void CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (!hero || (hero->Flags91 & 1))
        return;
    if (HasTC(hero, 0xbc)) {
        CTCFollowed* followed = GetTC<CTCFollowed>(hero, 0xbc);
        if (followed->IsFollower(ParentThing))
            followed->RemoveFollower(ParentThing);
    }
    if (HasTC(hero, 0x7a)) {
        CTCRegionFollower* regionFollower = GetTC<CTCRegionFollower>(hero, 0x7a);
        if (regionFollower->IsRegionFollower(ParentThing))
            regionFollower->RemoveRegionFollower(ParentThing);
    }
}
