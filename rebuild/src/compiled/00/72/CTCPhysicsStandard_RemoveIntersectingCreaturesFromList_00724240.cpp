// CTCPhysicsStandard::RemoveIntersectingCreaturesFromList(vector<CThing*>& things) const -- drop from `things` every collidable thing that overlaps the owner creature (erase(remove_if(begin, end, CIsCreatureIsIntersecting(owner)), end))
// Leaf: ?RemoveIntersectingCreaturesFromList@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724240, 74 bytes (80 with padding), `ret 4`.
// Label correction: the catalog "Position_MoveElements" is descriptive; Ego_r.pdb names
//   `CTCPhysicsStandard::RemoveIntersectingCreaturesFromList(std::vector<CThing*>&) const` (protected), and its retail
//   caller is CTCPhysicsNavigator::Move (call site 0x00727033).
// Callees: __find_if(first, last, pred, const random_access_iterator_tag&) @ 0x00724A90 and
//   remove_copy_if(first, last, result, pred) @ 0x00724A50 (both this chunk, __fastcall); memmove @ 0x00BFEAE6.
// Layout: CTCPhysicsStandard (engine header) -- the owner thing pointer at +0x04 (the header's `owner` word, CTCBase's
//   ParentThing); the vector is STLport's 12-byte {_M_start, _M_finish, _M_end_of_storage}.
// Shape: `things.end()` is read once (esi) and serves both remove_if and erase (arguments evaluate right to left);
//   STLport remove_if (`first = find_if(first, last, pred); if (first == last) return first; next = first;
//   return remove_copy_if(++next, last, first, pred);`) and the POD erase (`__copy_trivial`: `last == first ? result
//   : memmove(result, first, n) + n`, then `_M_finish = i`) are inlined; the find_if tag sits in the dead vector
//   argument slot.
#include "engine/CTCPhysicsStandard.h"

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
class CThing;

struct CIsCreatureIsIntersecting {              // 4 bytes (PDB)
    CThing* Owner;                              // +0x00
    CIsCreatureIsIntersecting(CThing* owner) : Owner(owner) {}
    bool operator()(CThing* thing) const;       // 0x007249C0
};

struct random_access_iterator_tag { random_access_iterator_tag() {} };

CThing** __fastcall __find_if(CThing** first, CThing** last, CIsCreatureIsIntersecting pred, const random_access_iterator_tag&);   // 0x00724A90
CThing** __fastcall remove_copy_if(CThing** first, CThing** last, CThing** result, CIsCreatureIsIntersecting pred);   // 0x00724A50

inline CThing** find_if(CThing** first, CThing** last, CIsCreatureIsIntersecting pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}

inline CThing** remove_if(CThing** first, CThing** last, CIsCreatureIsIntersecting pred)
{
    first = find_if(first, last, pred);
    if (first == last)
        return first;
    else {
        CThing** next = first;
        return remove_copy_if(++next, last, first, pred);
    }
}

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

struct CThingPtrVector {                        // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;                          // +0x00
    CThing** _M_finish;                         // +0x04
    CThing** _M_end_of_storage;                 // +0x08
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
    CThing** erase(CThing** first, CThing** last)
    {
        CThing** i = (CThing**)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
};

struct CTCPhysicsStandard_Methods : CTCPhysicsStandard {
    CThing* GetOwner() const { return (CThing*)owner; }
    void RemoveIntersectingCreaturesFromList(CThingPtrVector& things) const;
};

void CTCPhysicsStandard_Methods::RemoveIntersectingCreaturesFromList(CThingPtrVector& things) const
{
    things.erase(remove_if(things.begin(), things.end(), CIsCreatureIsIntersecting(GetOwner())), things.end());
}