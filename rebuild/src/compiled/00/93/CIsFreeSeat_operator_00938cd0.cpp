// CIsFreeSeat::operator()(const CIntelligentPointer<CThing>&) const -- true for an object thing with kind-flag bit 16 or bit 17 (a chair or a bench) whose user-action point is free
// Leaf: ??RCIsFreeSeat@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe @ 0x00938CD0, 145 bytes, thiscall (`this` unused: empty predicate) `ret 4`, bool in al.
// Identity (proven): its only callers are the find_if bodies at 0x00938D89 and 0x00938E69; the landed 0x00938E50 is
//   std::find_if<_List_iterator<CIntelligentPointer<CThing>, _Const_traits<...> >, CIsFreeSeat> and binds this address
//   as ??RCIsFreeSeat@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z (FableWin.pdb has the same symbol).
// Model: the landed CIsFreeBench::operator() 0x00918050 (bench bit 17, interface 0x2D) with the chair bit 16 as an
//   alternative: the thing is fetched once into esi for both slot-48 flag reads.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times), the const CVectorMap LowerBound @ 0x004365B0,
//   CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0 (its bool returned as is).
class CTCBase;
// A thing's component table: the presence bitset at +0x20 (bit N of dword N/32) and the sorted {type, component}
// CVectorMap at +0x44 searched by LowerBound (non-const copy 0x0040F020, const copy 0x004365B0).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);         // 0x0040F020
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetStateFlags();        // slot 48 (+0xC0): a word of 1-bit kind flags
    unsigned char _pad04[0xc];
    unsigned char m_thingType;                    // +0x10 (5: object)
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                   // +0x44
    unsigned char _pad50[0x41];
    unsigned char m_flags91;                      // +0x91
    bool HasTC(long type) const { return (m_presentBits[type >> 5] & (1UL << (type & 31))) != 0; }
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
    template <class T> __forceinline bool GetInterfaceConst(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CTCUserActionPointBase { public:
    bool IsActionPointFree();                     // 0x007FEBF0
    long GetNumberOfActionPointsInUse();          // 0x007FEC10
};

class CIsFreeSeat {
public:
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
};
bool CIsFreeSeat::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.GetPItem() && thing.GetPItem()->m_thingType == 5) {
        CThing* seat = thing.GetPItem();
        if (((unsigned char)(seat->GetStateFlags() >> 16) & 1) || ((unsigned char)(seat->GetStateFlags() >> 17) & 1)) {
            CTCUserActionPointBase* actionPoint;
            if (thing.GetPItem()->GetInterfaceConst(0x2d, &actionPoint))
                return actionPoint->IsActionPointFree();
        }
    }
    return false;
}