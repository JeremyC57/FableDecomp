// CTCCarrying::GetCarriedThings(CArray<CThing*>&) -- append the thing held in every occupied carry slot to the caller's list (descriptive name; the catalog's "COptimisedPrimitive::RemapGroups" is a propagated label)
// Leaf: GetCarriedThings
// Grade: DIFFER(162v161) -- frame homing: retail homes both the pushed pointer and InsertGrow's empty tag in the dead argument slot (no frame), the lane gives the value its own push-ecx slot (tried a long cast, a typed CArray, a named local, a by-value push_back); the register choice in the loop follows from it
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A2220, 161 bytes (+ int3 padding; `ret 4`).
// Class: not in any vtable; the neighbour 0x006A22D0 (CTCCarrying::RemoveAllWeapons) walks the same container, and Ego_r.pdb
//   CTCCarrying has CarrySlots (CVectorMap<long, CIntelligentPointer<CThing> >: 12-byte pairs, the pointer at +4) at +0x0c.
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (twice per slot), the 4-byte vector's InsertGrow 0x00436AC0 (landed as the placeholder
//   CRetailVector_dword::InsertGrow beside PushBack 0x00436C80: where, value, empty tag, count 1, no tail).
// Shape: an indexed walk over the slots (size() re-read by the /12 reciprocal each step, the element offset strength-reduced);
//   the inline push_back stores the pointer through its placement-new null test or grows, the value homed in the dead
//   argument slot.
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
template <class K, class V> struct pair { K first; V second; };
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CTCCarrying {
public:
    void*         __vftable;                       // +0x00
    unsigned char _pad04[8];
    vector<pair<long, CIntelligentPointer<CThing> > > CarrySlots;   // +0x0c CVectorMap (STLport vector first)
    void GetCarriedThings(CArray<CThing*>& things);
};

void CTCCarrying::GetCarriedThings(CArray<CThing*>& things)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i) {
        if (CarrySlots[i].second.GetPItem()) {
            CThing* thing = CarrySlots[i].second.GetPItem();
            things.push_back(thing);
        }
    }
}
