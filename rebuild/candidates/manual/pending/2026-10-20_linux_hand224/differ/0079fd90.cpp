// CTCWillLightning::CLightningBeam::IsTargetValid() -- beam target check through the home thing's interface-8 component
// Grade: DIFFER(ours v retail) -- frame packing only: the second inlined GetInterface key gets its own frame word (retail shares one word for both keys: frame 8 v 0xc, all [esp] offsets after it shift by 4)
struct CTCBase;
struct CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char m_thingType;                  // +0x10 (1: creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
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
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCOwned {                               // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32();
    virtual CThing* GetOwner();                 // slot 33 (+0x84)
    unsigned char _pad04[0x40 - 0x04];
    CBaseIntelligentPointer Owner;              // +0x40
};
struct CLightningBeam {
    long                    m_lsState;          // +0x00 ELightningState
    CBaseIntelligentPointer m_tpParticleHandle; // +0x04
    CBaseIntelligentPointer m_tcpHome;          // +0x0c
    CBaseIntelligentPointer m_tcpTarget;        // +0x14
    bool IsTargetValid();
};

bool CLightningBeam::IsTargetValid()
{
    if (IS_THING_ALIVE(m_tcpHome)) {
        CTCOwned* owned;
        if (m_tcpHome.GetPItem()->GetInterface(&owned, 8)) {
            CThing* owner;
            if (m_tcpHome.GetPItem()->m_thingType == 1) {
                CTCOwned* creatureOwned;
                m_tcpHome.GetPItem()->GetInterface(&creatureOwned, 8);
                owner = creatureOwned->Owner.GetPItem();
                if (owner == 0)
                    owner = owned->GetOwner();
            } else {
                owner = owned->GetOwner();
            }
            if (m_tcpTarget.GetPItem() != owner)
                return false;
        }
    }
    return true;
}
