// CTCCarrying::RemoveAllThingsInPrimarySlot(vector<CThing*>& removed, bool flag) -- take the thing out of every primary carry slot, appending each to the caller's list
// Leaf: ?RemoveAllThingsInPrimarySlot@CTCCarrying@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A22D0, length 155 bytes (+ int3 padding), thiscall `ret 8`.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDef<T>(long, CDefPointer<T>*)
//   @ 0x006A3060, CTCCarrying::RemoveThingInCarrySlot(long, bool) @ 0x006A16A0, the 4-byte vector's
//   _M_insert_overflow @ 0x00436AC0 (landed placeholder "InsertGrow": where, value, empty tag, 1, true), the
//   definition's virtual slot 1 on the last release.
// Identity: Ego_r.pdb `?RemoveAllThingsInPrimarySlot@CTCCarrying@@QAEXAAV?$vector@PAVCThing@@...@std@@_N@Z`
//   (name set only), the donor neighbour of GetAllCarriedThings as retail's 0x006A2220. The catalog's
//   "CTCCarrying_RemoveAllWeapons" is a descriptive auto label.
// Layout: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc = the retail sorted vector of
//   12-byte {slot, pointer} pairs (the landed CTCCarrying models).
// Shape: an iterator walk (end re-read each step, the empty-range test before the ebx/edi pushes); the landed
//   IsSlotPrimarySlot (0x006A1330) inlined -- def pointer in the dead `removed` slot, the flag read into bl before
//   the count-local release -- then RemoveThingInCarrySlot and the inline push_back (placement-new null test,
//   or _M_insert_overflow with the value homed in the same dead slot). Sharing that one slot needs the
//   overflow stand-in to read the value (not pass its address on): with `&__x` escaping, the pushed value
//   gets its own `push ecx` frame word (DIFFER 164v155).
#include <new>
class CThing;

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    typedef SCarrySlotEntry* iterator;
    iterator begin() { return m_pFirst; }
    iterator end() { return m_pLast; }
};

struct CCarrySlotDef {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long          m_refCount;                       // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;                        // +0x30
};

template <class T> struct CDefPointer {             // the landed IsSlotPrimarySlot 0x006A1330 model
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        T* p = m_p;
        long count = p->m_refCount - 1;
        p->m_refCount = count;
        if (count == 0)
            p->Destroy();
    }
    T* operator->() const { return m_p; }
};

struct CDefinitionManager {
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_006a22d0();                           // stand-in callee of the stand-in body below
// Stand-in body (the real one is landed out of line at 0x006A3060): it writes the out slot only, so VC7.1 knows
// the def pointer's address does not escape the call (the landed 0x006A1330 idiom).
#pragma auto_inline(off)
__declspec(noinline) void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_006a22d0();
    if (!def)
        return;
    *(void**)out = def;
}
#pragma auto_inline(on)

struct CGameDefinitionManager {
    static CDefinitionManager* Get();               // 0x0044C6B0
};

namespace std {
struct __true_type { __true_type() { } };           // user constructor: no zero fill of the 1-byte temporary
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend);          // 0x00436AC0
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new ((void*)this->_M_finish) T(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
}
using std::vector;
// Same-TU stand-in for _M_insert_overflow 0x00436AC0 (not the retail body): it reads the value only through its
// const reference, so the value's address does not escape and VC7.1 packs it into the def's dead-argument slot.
void GrowInsert_00436ac0(void* v, void* where, void* value);
#pragma auto_inline(off)
template <class T, class A>
__declspec(noinline) void vector<T, A>::_M_insert_overflow(T* __position, const T& __x, const std::__true_type&,
                                                           unsigned int __fill_len, bool __atend)
{
    GrowInsert_00436ac0(this, __position, __x);
}
#pragma auto_inline(on)

struct CTCCarrying {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    CCarrySlotMap CarrySlots;                       // +0x0c
    bool IsSlotPrimarySlot(long slot)               // 0x006A1330 (inlined here)
    {
        CDefPointer<CCarrySlotDef> def;
        CGameDefinitionManager::Get()->GetDef(slot, &def);
        return def->IsPrimary;
    }
    CThing* RemoveThingInCarrySlot(long slot, bool flag);    // 0x006A16A0
    void RemoveAllThingsInPrimarySlot(vector<CThing*>& removed, bool flag);
};

void CTCCarrying::RemoveAllThingsInPrimarySlot(vector<CThing*>& removed, bool flag)
{
    for (CCarrySlotMap::iterator it = CarrySlots.begin(); it != CarrySlots.end(); ++it) {
        if (IsSlotPrimarySlot(it->first))
            removed.push_back(RemoveThingInCarrySlot(it->first, flag));
    }
}