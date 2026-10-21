// CTCBoss::SetHelperCreature(CThingAICreature* creature) -- remember a helper creature: ignore one already held alive, else reuse the first dead slot, else append it
// Leaf: SetHelperCreature
// Retail 0x007AF310, 384 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10
// (CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0), vector<CIntelligentPointer<CThingAICreature>>::
// _M_insert_overflow(pos, x, const __false_type&, size_type, bool) 0x007AFD10.
// Layout = Ego_r.pdb CTCBoss: HelperCreatures +0x30 (STLport vector, 8-byte elements); CThing dead flag +0x91 bit 0.
// Shape: two index loops over `size()` (signed pointer difference, unsigned compare); IS_THING_ALIVE then a fresh `[i]`
// for the identity test (`creature == v[i].GetPItem()`: retail compares the memory operand first); operator[] is STLport's
// `*(begin() + n)` (a direct `_M_start[n]` rotates eax/ecx and computes the assigned slot before GetPItem); the dead slot is assigned from an intelligent-pointer temporary (inline operator= = GetPItem +
// SetPItem); otherwise STLport push_back: placement-new copy (null-tested) or _M_insert_overflow with the tag homed in the
// dead `creature` slot.
struct CBaseObject;
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CThingAICreature : CThing { };
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>: vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
typedef CIntelligentPointer<CThingAICreature> CCreaturePointer;
struct CCreaturePointerVector {                         // STLport vector (12 bytes)
    typedef unsigned int size_type;
    CCreaturePointer* _M_start;
    CCreaturePointer* _M_finish;
    CCreaturePointer* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    CCreaturePointer* begin() { return _M_start; }
    CCreaturePointer& operator[](size_type i) { return *(begin() + i); }
    void _M_insert_overflow(CCreaturePointer* pos, const CCreaturePointer& x, const __false_type&, size_type fill_len,
                            bool atend);                // 0x007AFD10
    void push_back(const CCreaturePointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCreaturePointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CTCBoss {
public:
    unsigned char          _pad00[0x30];
    CCreaturePointerVector HelperCreatures;             // +0x30
    void SetHelperCreature(CThingAICreature* creature);
};

void CTCBoss::SetHelperCreature(CThingAICreature* creature)
{
    unsigned int i;
    for (i = 0; i < HelperCreatures.size(); ++i) {
        if (IS_THING_ALIVE(HelperCreatures[i]) && creature == HelperCreatures[i].GetPItem())
            return;
    }
    for (i = 0; i < HelperCreatures.size(); ++i) {
        if (!IS_THING_ALIVE(HelperCreatures[i])) {
            HelperCreatures[i] = CCreaturePointer(creature);
            return;
        }
    }
    HelperCreatures.push_back(CCreaturePointer(creature));
}