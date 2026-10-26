// CTCTavern::GetFreeSeat(long) -- the first alive owned object of the tavern thing that is a seat (type flag bit 16) with a free action point, or null
// Leaf: GetFreeSeat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BABE0, 225 bytes (`ret 4`; the argument is not read).
// Callees: the interface LowerBound @ 0x0040F020 (unchecked interface 0x21 lookup) and its const twin @ 0x004365B0 (interface
// 0x2d), CBaseIntelligentPointer::GetPItem const @ 0x00A01B50 (three times per object), the object's virtual slot 48
// GetThingTypeFlags (+0xc0), CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: CThing interface bitset +0x20 (0x21 = bit 1 and 0x2d = bit 13 of the second word), map +0x44, dead flag bit 0 of
// +0x91; the interface 0x21 component is CTCThingOwner (ObjectList, the 4-byte STLport list header pointer, at +0x0c; node
// {next, prev, CIntelligentPointer<CThing> at +8}).
// Shape: the owner comes from an unchecked out-parameter lookup (on an absent interface retail reads the never-written out
// word, which is the lookup key's frame word) and is re-read from that word on every pass; the seat
// is a named local after the alive test; HasTC is the raw bitset test and GetTC the one-ternary const lookup into ecx.
class CThing;
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
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
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual unsigned long GetThingTypeFlags() const;    // slot 48 (+0xc0)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
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
struct CObjectNode {                                    // STLport list node of CIntelligentPointer<CThing>
    CObjectNode* _M_next;
    CObjectNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;                    // +0x08
};
class CTCThingOwner { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;                            // +0x0c (header node)
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                           // 0x007FEBF0
};
class CTCTavern { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CThing* GetFreeSeat(long unused);
};

CThing* CTCTavern::GetFreeSeat(long unused)
{
    CTCThingOwner* owner;
    ParentThing->GetInterface(0x21, &owner);
    for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
        CIntelligentPointer<CThing>& object = node->_M_data;
        if (object.GetPItem() && !(object.GetPItem()->m_flags91 & 1)) {
            CThing* seat = object.GetPItem();
            if (((unsigned char)(seat->GetThingTypeFlags() >> 16) & 1) && seat->HasTC(0x2d)) {
                if (((CTCUserActionPointBase*)seat->GetTC(0x2d))->IsActionPointFree())
                    return seat;
            }
        }
    }
    return 0;
}