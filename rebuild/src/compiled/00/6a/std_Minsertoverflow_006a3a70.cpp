// std::vector<std::pair<long, CIntelligentPointer<CThing> > >::_M_insert_overflow(pair* position, const pair& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport 4.6 regrowth of the (key, intelligent pointer) vector: allocate old + max(old, fill_len) entries, copy the head, construct fill_len copies of x (the one-element copy inline), copy the tail unless at the end, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@V?$allocator@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@IAEXPAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@2@ABU32@ABU__false_type@2@I_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A3A70, 285 bytes, thiscall `ret 14h`, no EH frame.
// Identity: the landed push_back 0x006A3CD0 calls it as `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`; its out-of-line
//   callees are this element's __uninitialized_copy 0x006A31D0 and __uninitialized_fill_n 0x006A3220 (landed). The catalog label
//   "Vector_ReserveAndFill" is descriptive.
// Source: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated, with
//   FABLE_RETAIL_CINTELLIGENTPOINTER_COPY with _COPY_ORIGINAL_CHAIN: the element's CIntelligentPointer<CThing> copy constructor in
//   FableWin.pdb's own text (??0?$CIntelligentPointer@VCThing@@@@QAE@ABV0@@Z: `SetPointer(item.GetPointer())`, the GetPointer
//   static_cast to CThing* needs engine/CThing.h); with the SetPItem(GetPItem()) spelling every byte matches but the parked register
//   swap remains (newFinish loses its register to the destroy cursor). Out-of-line __fastcall __uninitialized_copy / __uninitialized_fill_n
//   (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N, as landed 0x00CBC130).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x006A31D0, __uninitialized_fill_n 0x006A3220, CBaseIntelligentPointer ctor 0x00A01B10
//   / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vptr 0x01238C6C), free 0x00BFEA14.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
template void std::vector<std::pair<long, CIntelligentPointer<CThing> > >::_M_insert_overflow(std::pair<long, CIntelligentPointer<CThing> >*, const std::pair<long, CIntelligentPointer<CThing> >&, const std::__false_type&, unsigned int, bool);