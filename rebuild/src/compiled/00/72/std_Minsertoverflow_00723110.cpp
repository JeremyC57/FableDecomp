// std::vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow(CGenerationPos* position, const CGenerationPos& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport 4.6 grow-and-insert of the smoke generator's emitter positions: allocate old + max(old, fill_len) entries, copy the head, construct fill_len copies of x, copy the tail unless at the end, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCGenerationPos@CTCSmokeGenerator@@V?$allocator@VCGenerationPos@CTCSmokeGenerator@@@std@@@std@@IAEXPAVCGenerationPos@CTCSmokeGenerator@@ABV34@ABU__false_type@2@I_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723110, 300 bytes, thiscall `ret 14h`.
// Identity: called by the landed vector<CGenerationPos>::push_back 0x00723270; its out-of-line callees are the landed __uninitialized_copy
//   0x00722B70 and __uninitialized_fill_n 0x00722BD0. Element: Ego_r.pdb CTCSmokeGenerator::CGenerationPos {C3DVector Pos;
//   CIntelligentPointer<CThing> PEmitter} (20 bytes); no engine header holds CTCSmokeGenerator, so the nested class is declared here
//   over the shared C3DVector / CIntelligentPointer. The catalog label "std::vector_ReserveAndCopy" is descriptive.
// Source: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated, with
//   FABLE_RETAIL_CINTELLIGENTPOINTER_COPY with _COPY_ORIGINAL_CHAIN: the element's CIntelligentPointer<CThing> copy constructor in
//   FableWin.pdb's own text (??0?$CIntelligentPointer@VCThing@@@@QAE@ABV0@@Z: `SetPointer(item.GetPointer())`, the GetPointer
//   static_cast to CThing* needs engine/CThing.h); with the SetPItem(GetPItem()) spelling every byte matches but the parked register
//   swap remains (newFinish loses its register to `this`). Out-of-line __fastcall __uninitialized_copy / __uninitialized_fill_n
//   (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N, as landed 0x00CBC130).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x00722B70, __uninitialized_fill_n 0x00722BD0, CBaseIntelligentPointer ctor 0x00A01B10
//   / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vptr 0x01238C6C), free 0x00BFEA14.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CTCSmokeGenerator {
public:
    class CGenerationPos {                                   // Ego_r.pdb, 20 bytes
    public:
        C3DVector Pos;                                       // +0x00
        CIntelligentPointer<CThing> PEmitter;                // +0x0C
    };
};
template void std::vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow(CTCSmokeGenerator::CGenerationPos*, const CTCSmokeGenerator::CGenerationPos&, const std::__false_type&, unsigned int, bool);