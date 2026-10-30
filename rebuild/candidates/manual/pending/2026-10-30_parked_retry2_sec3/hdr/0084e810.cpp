// NCameraModes::CBase::AddSubComponent(CCountedPointer<CSubComponentBase> component, bool replace) -- adds a sub-component to the camera mode; with `replace`, first erases the existing sub-component of the same type; a null component is ignored
// Grade: DIFFER(192v176) -- migrated onto engine/CCountedPointer.h (identical bytes to the local model); residue: retail keeps the by-value pointer's Data in ebp and Info in esi from entry (copy and release from registers), ours re-reads both words from the argument slot
// Leaf: ?AddSubComponent@CBase@NCameraModes@@QAEXV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E810, 176 bytes, __thiscall, `ret 0Ch` (the 8-byte counted pointer by value + the bool).
// Name: FableWin.pdb ?AddSubComponent@CBase@NCameraModes@@QAEXV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@_N@Z, as
//   the landed callers CSACCombat::SetUpPhysics 0x008732D0 / CStringAndCage::SetUpPhysics 0x00873250 bind it; the manifest
//   label "ScriptEventList::InsertEvent" is propagated and wrong.
// Callees: the sub-component's GetType() (virtual slot 2, +8); STLport __find_if(first, last, CIsOfType, const
//   random_access_iterator_tag&) 0x0084F010 (`__fastcall`, the unrolled-by-4 random-access search; manifest
//   "StdVector_LinearSearch"); vector<CCountedPointer<CSubComponentBase> >::erase(iterator) 0x0084F570 (landed) and
//   _M_insert_overflow(pos, const T&, const __false_type&, size_type, bool) 0x0084FAB0; the count block's destroy function
//   (+4, object in ecx) and operator delete 0x00BFE9BC (the by-value parameter's destructor).
// Layout: retail CBase SubComponents vector at +0x48 (Ego_r.pdb +0x58; CameraManager PDB +0x50 / retail +0x40, the landed
//   SetUpPhysics rows); Ego_r.pdb NCameraModes::CIsOfType { ESubComponentType Type; } is the 4-byte predicate.
// Shape: the type is read before the replace test (a virtual call, so it is always made); the `find_if != end()` test and the
//   erase each run their own search (the second reuses the end pointer still in edx); both empty tags (the find's iterator
//   tag and push_back's __false_type) sit in the dead `replace` argument slot; push_back is STLport's inline fast path with
//   the placement-new null check and the counted pointer's copy constructor (`++Info->References`).
void __cdecl operator delete(void* block);            // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
struct __false_type {
    __false_type() { }
};

#include "engine/CCountedPointer.h"

namespace NCameraModes {

enum ESubComponentType { SUB_COMPONENT_TYPE_NULL };

class CSubComponentBase {
public:
    virtual void v00();
    virtual void v01();
    virtual ESubComponentType GetType() const;       // slot 2 (+8)
};

typedef CCountedPointer<CSubComponentBase> CSubComponentPointer;

struct CIsOfType {                                   // Ego_r.pdb, 4 bytes
    ESubComponentType Type;
    CIsOfType(ESubComponentType type) : Type(type) { }
};

CSubComponentPointer* __fastcall __find_if(CSubComponentPointer* first, CSubComponentPointer* last, CIsOfType pred,
                                           const random_access_iterator_tag& tag);   // 0x0084F010

inline CSubComponentPointer* find_if(CSubComponentPointer* first, CSubComponentPointer* last, CIsOfType pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}

struct CSubComponentVector {                         // STLport vector<CCountedPointer<CSubComponentBase> >, 12 bytes in retail
    CSubComponentPointer* _M_start;
    CSubComponentPointer* _M_finish;
    CSubComponentPointer* _M_end_of_storage;
    CSubComponentPointer* begin() { return _M_start; }
    CSubComponentPointer* end() { return _M_finish; }
    CSubComponentPointer* erase(CSubComponentPointer* position);                 // 0x0084F570
    void _M_insert_overflow(CSubComponentPointer* position, const CSubComponentPointer& x, const __false_type&,
                            unsigned int fillLength, bool atEnd);                // 0x0084FAB0
    void push_back(const CSubComponentPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CSubComponentPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CBase {
public:
    unsigned char _pad00[0x48];
    CSubComponentVector SubComponents;               // +0x48
    void AddSubComponent(CSubComponentPointer component, bool replace);
};

void CBase::AddSubComponent(CSubComponentPointer component, bool replace)
{
    if (component.Data) {
        ESubComponentType type = component.Data->GetType();
        if (replace) {
            CSubComponentPointer* found = find_if(SubComponents.begin(), SubComponents.end(), CIsOfType(type));
            if (found != SubComponents.end())
                SubComponents.erase(find_if(SubComponents.begin(), SubComponents.end(), CIsOfType(type)));
        }
        SubComponents.push_back(component);
    }
}

}
