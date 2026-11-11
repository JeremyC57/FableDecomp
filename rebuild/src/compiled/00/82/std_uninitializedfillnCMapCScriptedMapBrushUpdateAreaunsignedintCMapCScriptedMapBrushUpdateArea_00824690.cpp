// std::__uninitialized_fill_n(CMap::CScriptedMapBrushUpdateArea* first, unsigned int n, CMap::CScriptedMapBrushUpdateArea const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCScriptedMapBrushUpdateArea@CMap@@IV12@@std@@YIPAVCScriptedMapBrushUpdateArea@CMap@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00824690, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00826450 source (stdvector_Minsertoverflow_00826450.cpp) names CMap::CScriptedMapBrushUpdateArea; binder ??$__uninitialized_fill_n@PAVCScriptedMapBrushUpdateArea@CMap@@IV12@@std@@YIPAVCScriptedMapBrushUpdateArea@CMap@@PAV12@IABV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMap::CScriptedMapBrushUpdateArea.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

class CMap {
public:
class CScriptedMapBrushUpdateArea {
public:
    long first;
    CCharString second;
};
};
template CMap::CScriptedMapBrushUpdateArea* std::__uninitialized_fill_n<CMap::CScriptedMapBrushUpdateArea*,unsigned int,CMap::CScriptedMapBrushUpdateArea >(CMap::CScriptedMapBrushUpdateArea*, unsigned int, CMap::CScriptedMapBrushUpdateArea const&, std::__false_type const&);
