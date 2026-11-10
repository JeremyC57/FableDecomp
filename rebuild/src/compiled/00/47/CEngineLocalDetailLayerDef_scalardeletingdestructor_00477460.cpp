// CEngineLocalDetailLayerDef::`scalar deleting destructor'(unsigned int flags) -- run ~CEngineLocalDetailLayerDef, free the def when bit 0 of flags is set
// Leaf: ??_GCEngineLocalDetailLayerDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00477460, 28 bytes, `ret 4`.
// Identity (proven): engine/CEngineLocalDetailLayerDef.h names slot 0 0x00477460 (scalar deleting) and ~ 0x0047747C
//   (??1CEngineLocalDetailLayerDef@@UAE@XZ, Ego_r/FableWin name sets); the body calls 0x0047747C and then operator delete.
// Callees: CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef 0x0047747C, operator delete 0x00BFE9BE.
// Shape: VC7.1 emits the `??_G` with the class's vftable in the TU that defines the virtual destructor (retail places it right
//   before 0x0047747C); the destructor here is the 0x0047747C row's source (out of line, called). Size TU.
#include "fable_compiler.h"
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
    FABLE_FORCEINLINE ~CObjectDefVector() { std::_Destroy(_M_start, _M_finish); ReleaseStorage(); }
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

class CEngineLocalDetailLayerDef : public CParentDefClassBase {
public:
    CFloatVector     SpacingFromLayer;           // +0x3c
    CObjectDefVector Objects;                    // +0x48
    virtual ~CEngineLocalDetailLayerDef();
};

CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef()
{
}
