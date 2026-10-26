// CAddFreeSeatToList::operator()(const CIntelligentPointer<CThing>& thing) const -- appends an object thing with state-flag bit 16 or 17 (a seat) whose user-action point is free to the functor's vector
// Leaf: ??RCAddFreeSeatToList
// Grade: RELOCATION_MATCH -- chunk-422 recipe: push_back(const T&) 0x0074FAE0 receives `seat`'s address; a same-TU noinline stand-in that never keeps it lets VC7.1 keep `seat` in esi (home: the push-ecx slot) and the LowerBound key in the dead `thing` slot, as retail. Earlier forms (inner scope, function scope, inline/forceinline helper, F88 tail, LowerBound stand-in) stayed 163v159.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938DB0, 159 bytes, `ret 4`. Caller: the for_each at 0x00938E90 (landed caller of
// the for_each: CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding 0x00938BF0). Callees: GetPItem @ 0x00A01B50, LowerBound @
// 0x004365B0, IsActionPointFree @ 0x007FEBF0, vector<CThing*>::push_back(const T&) @ 0x0074FAE0.
// Tried: seat in an inner scope, seat at function scope after early returns, an inline (then __forceinline) Consider(CThing* seat)
// helper with a by-value parameter -- all give the same 163-byte body.
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
class CThing {
public:
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
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
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

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);               // 0x0074FAE0 for vector<CThing*> (out of line)
};
#pragma auto_inline(off)
// Same-TU stand-in for vector<CThing*>::push_back 0x0074FAE0 (reads the element, never keeps its address): with it VC7.1
// treats `seat` as uncaptured, keeps it in esi with its home in the push-ecx slot and puts the LowerBound key in the dead
// `thing` slot, as retail.
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage)
        *_M_finish++ = __x;
}
#pragma auto_inline(on)
}

class CAddFreeSeatToList {
public:
    std::vector<CThing*>* m_pSeats;                       // +0x00
    void operator()(const CIntelligentPointer<CThing>& thing) const;
};
void CAddFreeSeatToList::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (!thing.GetPItem() || thing.GetPItem()->m_thingType != 5)
        return;
    CThing* seat = thing.GetPItem();
    if (((unsigned char)(seat->GetStateFlags() >> 16) & 1) || ((unsigned char)(seat->GetStateFlags() >> 17) & 1)) {
        CTCUserActionPointBase* actionPoint;
        if (seat->GetInterfaceConst(0x2d, &actionPoint) && actionPoint->IsActionPointFree())
            m_pSeats->push_back(seat);
    }
}