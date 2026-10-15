// CIsImposterHit::operator()() const -- true when the watched thing is alive, has creature mode 0x25 active in its mode manager (component kind 0x42) (the imposter has been hit)
// Leaf: ??RCIsImposterHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00963810, length 108 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls on the pointer at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (the inlined component lookup), CTCCreatureModeManager::IsModeActive @ 0x00811340 (landed;
// manifest label "CPlayer::HasMode" is propagated).
// Model: the out-parameter GetInterface template of the landed CCreatureAction_JackFall::FinishAction 0x008C2490 (the
// component stays in eax: `mov eax,[eax+4] ; push ; mov ecx,eax`); kind 0x42 is bit 2 of the presence word at +0x28.
// Shape: `P.GetPItem() && !P.GetPItem()->Dead` re-calls the getter; the third call's thing serves both the presence test
// and the lookup; the two-exit `mov al,1` / `xor al,al` tail.
struct CTCBase;
struct CInterfaceMap {                             // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _flags91 : 7;
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
    CThing* m_pItem;                               // +4
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CTCCreatureModeManager {                    // component kind 0x42 here
    bool IsModeActive(long mode) const;            // 0x00811340 (landed)
};
struct CIsImposterHit {
    unsigned char           _pad00[0x14];
    CBaseIntelligentPointer PImposter;             // +0x14 CIntelligentPointer<CThing>
    bool operator()() const;
};

bool CIsImposterHit::operator()() const
{
    if (PImposter.GetPItem() && !PImposter.GetPItem()->Dead) {
        CTCCreatureModeManager* modes;
        if (PImposter.GetPItem()->GetInterface(&modes, 0x42) && modes->IsModeActive(0x25))
            return true;
    }
    return false;
}
