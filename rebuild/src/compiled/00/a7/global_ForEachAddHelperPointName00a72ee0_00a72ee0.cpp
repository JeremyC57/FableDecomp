// _global::ForEach_AddHelperPointName_00a72ee0(const SHelperPoint* first, const SHelperPoint* last, CAddName f) -- for_each over CPhysicsMesh helper points: push_back each record's name into the functor's CCharString vector, return the functor
// Leaf: ?ForEach_AddHelperPointName_00a72ee0@@YI?AUCAddName@@PBUSHelperPoint@@0U1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72EE0, length 96 bytes (`ret 8`).
// Identity: catalog `_Move<std::_List_const_iterator<...CAnimSet...>*, std::_Temp_iterator<...> >` is propagated. The landed caller
//   CPhysicsMesh::GetAllHelperPointNames 0x00A6D050 declares this SGI `for_each<const SHelperPoint*, CAddName>` (`__fastcall`: the functor
//   result's hidden pointer in ecx, first in edx, last and the by-value functor on the stack); the records are
//   20-byte pair<CCharString, C3DHelperPoint> entries (0x14 stride).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, STLport vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape: the functor body is the inlined STLport `push_back`: placement copy at the finish pointer when there is room
//   (null-tested), else `_M_insert_overflow(finish, x, __false_type(), 1, true)` whose empty tag temporary is homed in
//   the dead functor argument slot (`lea eax,[esp+20h]`); the functor's vector pointer stays in edi and is stored
//   through the hidden result pointer on both exits.
// Name fix (NF208, 2026-11-20, FP-VECTORINSERT q6332 / q6341): the row-local CCharString / __false_type / CCharStringVector stand-ins
//   are replaced by fable_string.h and the STLport vector<CCharString> of stlport/fable_containers.h, so push_back's overflow call
//   binds the STLport protected spelling of 0x00433530 (?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@
//   @std@@IAE...) instead of ?_M_insert_overflow@CCharStringVector@@... . The overflow stays declaration-only
//   (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY), as the matched HEAD row had it; the header-less record / functor stand-ins stay.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"

struct SHelperPoint {                           // pair<CCharString, C3DHelperPoint>, 20 bytes
    CCharString   m_name;                       // +0x00
    unsigned char m_point[0x10];                // C3DHelperPoint
};

struct CAddName {                               // CPhysicsMesh's name-collecting functor (landed callers' model)
    std::vector<CCharString>* m_pNames;
    CAddName(std::vector<CCharString>& names) : m_pNames(&names) { }
    void operator()(const SHelperPoint& point) { m_pNames->push_back(point.m_name); }
};

CAddName FABLE_FASTCALL ForEach_AddHelperPointName_00a72ee0(const SHelperPoint* first, const SHelperPoint* last, CAddName f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}