// CTCCarrying::GetAllCarriedThings(std::vector<CThing*>& things) const -- append the thing held in every occupied carry slot to the caller's list
// Leaf: GetAllCarriedThings (Ego_r.pdb `?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@...@Z`, followed there by
//   RemoveAllThingsInPrimarySlot as retail's neighbour 0x006A22D0; the catalog's "COptimisedPrimitive::RemapGroups" is a propagated label)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A2220, 161 bytes (+ int3 padding; `ret 4`).
// Class: not in any vtable; the neighbour 0x006A22D0 (CTCCarrying::RemoveAllWeapons) walks the same container, and Ego_r.pdb
//   CTCCarrying has CarrySlots (CVectorMap<long, CIntelligentPointer<CThing> >: 12-byte pairs, the pointer at +4) at +0x0c.
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (twice per slot), the 4-byte vector's InsertGrow 0x00436AC0 (landed as the placeholder
//   CRetailVector_dword::InsertGrow beside PushBack 0x00436C80: where, value, empty tag, count 1, no tail).
// Shape: an indexed walk over the slots (size() re-read by the /12 reciprocal each step, the element offset strength-reduced);
//   the inline push_back stores the pointer through its placement-new null test or grows, the value homed in the dead
//   argument slot.
//   Both share that slot only with a same-TU noinline stand-in for InsertGrow (the const-ref taker of the value;
//   extern, the value gets a push-ecx word); the slot is reached as `(begin() + i)->second` (operator[] leaves the
//   _M_start loads in ecx/edx, 22 lines).
#include <new>
class CThing;
class CBaseIntelligentPointer {
public:
    void* m_pItem;
    void* m_pInfo;
    CThing* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
struct SScalarTag { SScalarTag() { } };            // user constructor: no zero-initialisation of the 1-byte temporary
template <class T> struct CArray {                 // retail CArray (STLport vector) {first, last, end}
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
// Same-TU stand-in for InsertGrow 0x00436AC0 (not the retail body).
void GrowInsert_00436ac0(void* v, void* where, const void* value);
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    GrowInsert_00436ac0(this, where, &value);
}
#pragma auto_inline(on)
template <class K, class V> struct pair { K first; V second; };
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CTCCarrying {
public:
    void*         __vftable;                       // +0x00
    unsigned char _pad04[8];
    vector<pair<long, CIntelligentPointer<CThing> > > CarrySlots;   // +0x0c CVectorMap (STLport vector first)
    void GetAllCarriedThings(CArray<CThing*>& things) const;
};

void CTCCarrying::GetAllCarriedThings(CArray<CThing*>& things) const
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i) {
        if ((CarrySlots.begin() + i)->second.GetPItem()) {
            CThing* thing = (CarrySlots.begin() + i)->second.GetPItem();
            things.push_back(thing);
        }
    }
}