// CRetailTree_erase008b1dc0::~CRetailTree_erase008b1dc0() -- std::map/set destructor: erase all nodes through 0x008B1DC0, reset the head, free it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B28F0, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je F ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Erase ; (re-read head) mov [eax+8],eax ; mov dword ptr [edx+4],0 ; mov [eax+0Ch],eax ; mov dword ptr [esi+4],0 ; F: mov esi,[esi] ; test esi,esi ; je R ; push esi ; call free ; add esp,4 ; R: pop esi ; ret
// Destructor of the retail red-black tree {head, size}: when the tree holds nodes the recursive
// erase @ 0x008B1DC0 (a __fastcall member taking the root, head->parent) frees them, then the
// head's left/parent/right are reset through `head` re-read per store and size is zeroed; finally
// the head node itself is released with free @ 0x00BFEA14 (null-checked). Only the erase callee
// distinguishes the instantiations, so the class is named by it. One of 108 bodies.
// Leaf: ??1CRetailTree_erase008b1dc0@@QAE@XZ
// NF72 (round 7, q10892): the erase call @ 0x008B1DC0 binds the STLport
// ?_M_erase@?$_Rb_tree@W4ECombatSequenceOnStopModuleType@@U?$pair@$$CBW4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@U?$_Select1st@U?$pair@$$CBW4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@@3@U?$less@W4ECombatSequenceOnStopModuleType@@@3@V?$allocator@U?$pair@$$CBW4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4ECombatSequenceOnStopModuleType@@P6IXAAVCThingCreatureBase@@0@Z@std@@@2@@Z
// (ported this round, rows/008b1dc0.cpp) through the shared stlport/fable_map.h: the stand-in now derives from std::map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>,
// whose inline ~map / ~_Rb_tree (clear) / ~_Rb_tree_base give this body (NF50 form, 0x006311D0). Its own name stays a stand-in: the
// address has no caller and no data reference, so ~_Rb_tree vs. ~map vs. an owner destructor is not provable.
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CThingCreatureBase;
enum ECombatSequenceOnStopModuleType;
struct CRetailTree_erase008b1dc0 : std::map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> {   // stand-in: a map<ECombatSequenceOnStopModuleType, ...> destructor
    ~CRetailTree_erase008b1dc0();
};
CRetailTree_erase008b1dc0::~CRetailTree_erase008b1dc0()
{
}
