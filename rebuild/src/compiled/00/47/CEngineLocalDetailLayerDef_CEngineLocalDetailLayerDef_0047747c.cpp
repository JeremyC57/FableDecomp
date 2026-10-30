// CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef() -- destroy the layer's Objects vector (range destroy of the 0x88-byte detail-object defs, then the storage) and its SpacingFromLayer float vector, then the def base destructor as the tail jump
// Leaf: ??1CEngineLocalDetailLayerDef@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047747C, 47 bytes, thiscall; size-optimised def TU (`pop ecx` after free).
// Identity: the only callers are CEngineLocalDetailLayerDef's scalar deleting destructor 0x00477460 (landed under that
//   name) and 0x00479974 / 0x00479B89; Ego_r.pdb CEngineLocalDetailLayerDef has vector<float> SpacingFromLayer (+0x40,
//   retail +0x3C behind the 0x3C-byte def base) and vector<CEngineLocalDetailObjectDef> Objects (+0x50, retail +0x48 with
//   12-byte vectors), and the name set holds ??1CEngineLocalDetailLayerDef@@UAE@XZ.  The chunk label
//   "CTCAssassinRush::FinishMove" was propagated; the landed name was the placeholder CDerived_0047747c.
// Callees: std::_Destroy<CEngineLocalDetailObjectDef*> 0x004774B9 (__fastcall, last loaded first), the Objects vector's
//   out-of-line storage release 0x004774AB (`if (_M_start) free(_M_start)`, its only caller), free 0x00BFEA14 (the float
//   vector's storage, inlined), the def base destructor 0x00430300 (landed as CDefWithBuffer::~CDefWithBuffer).
// Shape: members destroyed in reverse order (Objects +0x48, then SpacingFromLayer +0x3c), then the base; no vtable
//   store of its own (`__declspec(novtable)`, as the landed model).  The two vector members keep the landed stand-in
//   shapes (the STLport ~vector / ~_Vector_base split that leaves one storage release out of line is not modelled
//   here); the range destroy now binds the renamed std::_Destroy symbol.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

class CEngineLocalDetailObjectDef;               // 0x88-byte polymorphic element

namespace std {
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);   // 0x004774B9
}

struct CObjectDefVector {                        // retail std::vector<CEngineLocalDetailObjectDef> (12 bytes)
    CEngineLocalDetailObjectDef* _M_start;
    CEngineLocalDetailObjectDef* _M_finish;
    CEngineLocalDetailObjectDef* _M_end_of_storage;
    void ReleaseStorage();                       // 0x004774AB
    __forceinline ~CObjectDefVector() { std::_Destroy(_M_start, _M_finish); ReleaseStorage(); }
};

struct CFloatVector {                            // retail std::vector<float> (12 bytes)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    ~CFloatVector() { if (_M_start) free(_M_start); }
};

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class __declspec(novtable) CEngineLocalDetailLayerDef : public CParentDefClassBase {
public:
    CFloatVector     SpacingFromLayer;           // +0x3c
    CObjectDefVector Objects;                    // +0x48
    ~CEngineLocalDetailLayerDef();
};

CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef()
{
}
