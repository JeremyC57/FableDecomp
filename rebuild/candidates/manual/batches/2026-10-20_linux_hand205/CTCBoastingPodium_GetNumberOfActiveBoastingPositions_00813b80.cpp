// CTCBoastingPodium::GetNumberOfActiveBoastingPositions() const -- count the podium's boasting positions whose position component reports itself activated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813B80, length 198 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0
// (inlined GetInterface for CTCBoastingPosition, type 0x71 = bit 17 of CThing+0x2C), CTCBoastingPosition::IsPositionActivated
// (long&) const @ 0x00781D80 (?IsPositionActivated@CTCBoastingPosition@@QBE_NAAJ@Z).
// Layout: Ego_r.pdb CTCBoastingPodium BoastingPositions +0x0C (CVectorMap<long, CIntelligentPointer<CThing>>: a retail 12-byte
// vector of 12-byte {key, intelligent pointer} pairs).
// Shape: `for (unsigned long i = 0; i < size(); ++i)` with size() re-evaluated (the divide by 12 at the loop tail) and the element
// reached through `*(begin() + i)`; the unchecked out-parameter GetInterface shares its key word with the never-initialised
// position pointer (held in edi across iterations, as retail does); the activation cost is an unused out local.
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
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
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SBoastingPositionEntry {                 // pair<long, CIntelligentPointer<CThing> >
    long                    first;
    CBaseIntelligentPointer second;
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCBoastingPosition {
public:
    bool IsPositionActivated(long& cost) const; // 0x00781D80
};
class CTCBoastingPodium {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CRetailVector<SBoastingPositionEntry> BoastingPositions;   // +0x0c
    long GetNumberOfActiveBoastingPositions() const;
};

long CTCBoastingPodium::GetNumberOfActiveBoastingPositions() const
{
    long count = 0;
    for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
        CTCBoastingPosition* position;
        BoastingPositions[i].second.GetPItem()->GetInterface(&position, 0x71);
        long cost;
        if (position->IsPositionActivated(cost))
            ++count;
    }
    return count;
}
