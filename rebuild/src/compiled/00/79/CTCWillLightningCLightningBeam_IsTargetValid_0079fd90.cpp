// CTCWillLightning::CLightningBeam::IsTargetValid() -- beam target check through the home thing's interface-8 component
// Grade: see STATUS.tsv
class CTCBase;
class CThing;
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte {type, component} entries):
// compiled above the caller so VC7.1 knows the key's address does not escape, which lets both inlined lookup keys share
// one frame word (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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
    CIntelligentPointer<CThing> Owner;              // +0x40
};
struct CLightningBeam {
    long                    m_lsState;          // +0x00 ELightningState
    CIntelligentPointer<CThing> m_tpParticleHandle; // +0x04
    CIntelligentPointer<CThing> m_tcpHome;          // +0x0c
    CIntelligentPointer<CThing> m_tcpTarget;        // +0x14
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