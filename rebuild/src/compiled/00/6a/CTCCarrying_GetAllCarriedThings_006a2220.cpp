// CTCCarrying::GetAllCarriedThings(std::vector<CThing*>& things) const -- append the thing held in every occupied carry slot to the caller's list
// Leaf: ?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Name: Ego_r.pdb `?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@...@Z`, followed there by
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
// NF159 (q11671): the local CArray / SScalarTag / InsertGrow stand-ins replaced by the shared STLport vector (stlport/fable_containers.h):
//   Ego_r.pdb 0x006C6970 + FableWin.pdb 0x02217EE0 ?GetAllCarriedThings@CTCCarrying@@QBEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z.
//   The overflow body is visible but out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), the same-TU noinline shape the stand-in gave:
//   std::vector<CThing*>::_M_insert_overflow 0x00436AC0 (?_M_insert_overflow@?$vector@PAVCThing@@...@std@@IAEXPAPAVCThing@@ABQAV3@ABU__true_type@2@I_N@Z).
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "stlport/fable_containers.h"                // std::vector<CThing*> (STLport 4.6, 12 bytes)
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
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
    void GetAllCarriedThings(std::vector<CThing*>& things) const;
};

void CTCCarrying::GetAllCarriedThings(std::vector<CThing*>& things) const
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i) {
        if ((CarrySlots.begin() + i)->second.GetPItem()) {
            CThing* thing = (CarrySlots.begin() + i)->second.GetPItem();
            things.push_back(thing);
        }
    }
}