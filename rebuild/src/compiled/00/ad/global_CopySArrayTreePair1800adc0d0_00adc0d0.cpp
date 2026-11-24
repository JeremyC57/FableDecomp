// _global::Copy_SArrayTreePair18_00adc0d0(const SArrayTreePair18_00adc0d0* first, const SArrayTreePair18_00adc0d0* last, SArrayTreePair18_00adc0d0* result, const random_access_iterator_tag&, int*) -- STLport __copy<SArrayTreePair18_00adc0d0*>: counted forward assignment of 24-byte elements, returns the advanced result
// Leaf: Copy_SArrayTreePair18_00adc0d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADC0D0, length 79 bytes (+ int3 padding), `ret 0Ch`.
// Callees: STLport vector<unsigned long>::operator= @ 0x0045BC09 (Groups, through the inlined generated CArray assignment);
//   the map's _Rb_tree::operator= @ 0x0045C79E (GroupToIndexMap, `lea [esi+0Ch]` / `lea [edi+0Ch]`).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x2aaaaaab ; sar 2 (/24)), then the element's implicit operator= inlined memberwise (each member with its own assignment is a `push src ; mov ecx,dst ; call`, a plain word a direct move),
// both pointers stepped by 0x18; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is a {12-byte vector, 12-byte tree} pair (0x0045BC09 is a vector<4-byte>::operator= body, 0x0045C79E the landed CRetailTree_0045c811::operator=).
// NF70 (round 7, land1117fu): the stand-in tree struct(s) CRetailTree_0045c811 are now the STLport std::map<unsigned long, long> from stlport/fable_map.h; the implicit map/set operator= is inline and calls the instance's out-of-line _Rb_tree::operator=.
// NF387 (T3 nf383 0x009B7870): the element is Ego_r CVertexGroupBlock {CArray<unsigned long> Groups +0; map<unsigned long,long>
//   GroupToIndexMap} (the landed 0x00ADA240 / 0x00ADC120 models): Groups is the shared engine/CArray.h CArray<unsigned long>, whose
//   generated assignment calls STLport vector<unsigned long>::operator= 0x0045BC09 (FABLE_RETAIL_STLP_VECTOR_ASSIGN: declared, called;
//   NF383 R2: 14 exclusive vector-context FableWin callers); was the body-less local SArray0C_0045bc09.
// Layout (NF390, ruling R2 (c)): ghidra_out/struct_layouts_egor.tsv, Ego_r.pdb CVertexGroupBlock (32 bytes, 2 members):
//   +0  CArray<unsigned long> Groups;  +16 map<unsigned long,long,std::less<unsigned long>,...> GroupToIndexMap.
//   Ego_r's CArray<unsigned long> / vector<unsigned long> / that map are 16 bytes each (its later STL); retail's STLport 4.6 vector
//   and _Rb_tree map are 12 (engine/CArray.h line 3), so the retail element is 24 bytes (`imul 0x2AAAAAAB ; sar 2`) with
//   GroupToIndexMap at +0x0C (the second call's `lea [x+0Ch]`). Members carry the PDB names; the element keeps its landed
//   type name (the self spelling, Copy_SArrayTreePair18_00adc0d0, is queued for its own respell + census).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_map.h"
#include "engine/CArray.h"
struct SArrayTreePair18_00adc0d0 {               // 24 bytes; implicit memberwise operator=
    CArray<unsigned long> Groups;                // +0x00 CVertexGroupBlock::Groups (Ego_r.pdb +0)
    std::map<unsigned long, long> GroupToIndexMap;   // +0x0c CVertexGroupBlock::GroupToIndexMap (Ego_r.pdb +16)
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SArrayTreePair18_00adc0d0* __fastcall Copy_SArrayTreePair18_00adc0d0(const SArrayTreePair18_00adc0d0* first, const SArrayTreePair18_00adc0d0* last, SArrayTreePair18_00adc0d0* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}