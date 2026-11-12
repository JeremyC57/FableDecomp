// CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& orientation, CCharString name, long& index, long start, bool checkBlocked) -- remember the user, then from `start` hand out the first action point whose name contains `name` and that is on the free list: the point is taken off the free list; one already on the blocked list, or found blocked now by the virtual test, is skipped (and a newly blocked one is appended to the blocked list) unless `checkBlocked` is false; returns true with `index` set, false when no point qualifies
// Leaf: ?GetFreeNamedActionPoint@CTCNamedUserActionPoints@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@VCCharString@@AAJJ_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FF190, 343 bytes (`ret 1Ch`, no frame pointer, no SEH; the name by value, destroyed by the
//   callee on both exits). Ego_r.pdb 0x007F2A00 / FableWin.pdb: the Leaf spelling above (public, non-const).
// Callees: CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10
//   (the inline CIntelligentPointer<CThing>(CThing*) temporary and operator=, vtable 0x01238C6C); strstr 0x00BFEB3A (CRT);
//   the STLport random-access std::__find 0x0050E540 (__fastcall, `ret 8`; its exact retail instantiation symbol is unproven: its
//   112 bytes are identical to 0x0040E170, whose landed row already carries the __find<long*,long> Leaf, so the two are different
//   4-byte-element instantiations; this TU instantiates it over long* as the vector<long> call requires). Its body below is copied
//   from the landed CTCUserActionPointBase::IsActionPointIndexBlocked 0x007FEC30 row (itself the CRegion::IsMapIncluded 0x006BBFA0
//   model), out of line as in retail; memmove 0x00BFEAE6 (the inline vector<long>::erase -> __copy_ptrs ->
//   __copy_trivial); CTCUserActionPointBase::IsActionPointIndexBlocked(long) 0x007FEC30 (protected, non-virtual; qualified because
//   the class's own virtual overload hides it); the virtual IsActionPointIndexBlocked(CThing&, C3DVector&, CRightHandedSet&, long)
//   const, vtable slot 28 (+0x70; this class's override is 0x007FF0C0); std::vector<long>::push_back 0x00485FF3 (the STLport body
//   visible but called out of line: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE); ~CCharString 0x0099EAE0.
// Layout (overlay opt-in FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS of engine/CTCNamedUserActionPoints.h): CTCUserActionPointBase
//   FreeActionPointIndexes +0x0c, BlockedActionPointIndexes +0x18, NumberOfActionPoints +0x24, PThing +0x2c; Names +0x34.
// Shape: the index is a new local copied from `start` (kept in ebp with a frame copy in the dead `thing` slot, because its address
//   is find's value and push_back's argument); the element is a named const reference (its address is formed before the name's
//   text); the free-list end is a named local (edi), compared with find's result and reused by the inline erase, whose STLport
//   form is `if (position + 1 != end()) __copy_ptrs(...)` -- the __copy_trivial layer is what puts `mov ecx,edi` before
//   `lea edx,[eax+4]` and gives the `mov eax,[start] ; cmp ; mov ebp,eax` loop entry. find's body is visible, so the empty tag
//   temporary shares the live checkBlocked slot. Both tests of checkBlocked come after their call (`blocked && checkBlocked`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCNamedUserActionPoints.h"
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)

// std::__find: the same-TU STLport 4.6 body copied from the landed row 0x007FEC30 (CTCUserActionPointBase::IsActionPointIndexBlocked).
namespace std {
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
}

bool CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing& thing, C3DVector& position, CRightHandedSet& orientation, CCharString name, long& index, long start, bool checkBlocked)
{
    PThing = CIntelligentPointer<CThing>(&thing);
    for (long i = start; i < NumberOfActionPoints; ++i) {
        const CCharString& element = Names[i];
        if (strstr(element.GetAsCharArray(), name.GetAsCharArray())) {
            std::vector<long>::iterator last = FreeActionPointIndexes.end();
            std::vector<long>::iterator pos = std::find(FreeActionPointIndexes.begin(), last, i);
            if (pos != last) {
                FreeActionPointIndexes.erase(pos);
                if (!(CTCUserActionPointBase::IsActionPointIndexBlocked(i) && checkBlocked)) {
                    CThing* owner = PThing.Get();
                    if (!(IsActionPointIndexBlocked(*owner, position, orientation, i) && checkBlocked)) {
                        index = i;
                        return true;
                    }
                    BlockedActionPointIndexes.push_back(i);
                }
            }
        }
    }
    return false;
}