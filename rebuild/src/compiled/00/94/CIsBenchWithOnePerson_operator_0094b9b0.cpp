// CIsBenchWithOnePerson::operator()(const CIntelligentPointer<CThing>& thing) const -- true for an object thing with state-flag bit 17 (a bench) whose user-action point (interface 0x2D) is free with exactly one point in use
// Leaf: ??RCIsBenchWithOnePerson
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094B9B0, 147 bytes, `ret 4`. Callers: the find_if at 0x0094BA90 (CAIStateGroup_SitInPub),
// 0x0094BA69. Callees: GetPItem @ 0x00A01B50, LowerBound @ 0x004365B0, IsActionPointFree @ 0x007FEBF0,
// GetNumberOfActionPointsInUse @ 0x007FEC10.
// Model: CThing keeps the interface-present bitset at +0x20 (HasTC returns the masked word, so a constant type tests the
// byte/word in place: `test ch,20h`, `test dword ptr [eax+24h],200000h`) and the sorted {type, component} CVectorMap at +0x44
// (LowerBound const @ 0x004365B0, key in the dead argument slot); slot 48 (+0xC0) returns the kind-flag word, read as
// `(unsigned char)(flags >> n) & 1` (`shr eax,n ; test al,1`); +0x10 is the thing type byte, +0x91 a flag byte.
// Same model as the landed CIsFreeHopscotchBoard (0x00941BE0) / CThingFilter_HasFreeActionPoint (0x006A8B40) / 0x00938BF0.
// Shape: `return free && count == 1;` -- true leaves eax == 1 after `cmp eax,1`, false is `xor eax,eax`; the outer false is `xor al,al`.
struct CTCBase;
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
struct CThing {
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
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                     // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                     // 0x007FEBF0
    long GetNumberOfActionPointsInUse();          // 0x007FEC10
};

class CIsBenchWithOnePerson {
public:
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
};
bool CIsBenchWithOnePerson::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.GetPItem() && thing.GetPItem()->m_thingType == 5 && ((unsigned char)(thing.GetPItem()->GetStateFlags() >> 17) & 1)) {
        CTCUserActionPointBase* actionPoint;
        if (thing.GetPItem()->GetInterfaceConst(0x2d, &actionPoint))
            return actionPoint->IsActionPointFree() && actionPoint->GetNumberOfActionPointsInUse() == 1;
    }
    return false;
}