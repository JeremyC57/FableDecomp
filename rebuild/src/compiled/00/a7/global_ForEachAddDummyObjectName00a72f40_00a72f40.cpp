// _global::ForEach_AddDummyObjectName_00a72f40(const SDummyObject* first, const SDummyObject* last, CAddName f) -- for_each over CPhysicsMesh dummy objects: push_back each record's name into the functor's CCharString vector, return the functor
// Leaf: ?ForEach_AddDummyObjectName_00a72f40@@YI?AUCAddName@@PBUSDummyObject@@0U1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72F40, length 96 bytes (`ret 8`).
// Identity: catalog `_Move<std::_List_const_iterator<...CAnimSet...>*, std::_Temp_iterator<...> >` is propagated. The landed caller
//   CPhysicsMesh::GetAllDummyObjectNames 0x00A6D090 declares this SGI `for_each<const SDummyObject*, CAddName>` (`__fastcall`: the functor
//   result's hidden pointer in ecx, first in edx, last and the by-value functor on the stack); the records are
//   56-byte pair<CCharString, C3DDummyObject> entries (0x38 stride).
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

struct SDummyObject {                           // pair<CCharString, C3DDummyObject>, 56 bytes
    CCharString   m_name;                       // +0x00
    unsigned char m_object[0x34];               // C3DDummyObject
};

struct CAddName {                               // CPhysicsMesh's name-collecting functor (landed callers' model)
    std::vector<CCharString>* m_pNames;
    CAddName(std::vector<CCharString>& names) : m_pNames(&names) { }
    void operator()(const SDummyObject& object) { m_pNames->push_back(object.m_name); }
};

CAddName FABLE_FASTCALL ForEach_AddDummyObjectName_00a72f40(const SDummyObject* first, const SDummyObject* last, CAddName f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}