// CPrimitiveHandleSet_00441ee0::~CPrimitiveHandleSet_00441ee0() -- destructor of a second unidentified engine record (sibling of 0x00441CB0): destroy two vectors of primitive handles, two embedded primitive handles (last-handle notification, release) and the first handle vector
// Leaf: ??1CPrimitiveHandleSet_00441ee0
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00441EE0, 297 bytes, plain `ret`.
// Label: the catalog's "CPSCDecalRenderer::~CPSCDecalRenderer" is a propagated name (the PDB CPSCDecalRenderer has its
//   handles at +0x4c/+0x58 and no vectors); the body is the member-wise destructor of a non-polymorphic record whose
//   scalar deleting destructor is 0x00442F00; the class is unidentified, so it is named after its address (descriptive).
// Callees: free 0x00BFEA14 (STLport allocator), the handles' virtual destructor (slot 0, called with 0), the primitive's
//   virtual slots 3 (NotifyReleased) and 1 (Delete), ~CBaseClass 0x0099A2E0 (out of line).
// Headers: engine/CEnginePrimitiveHandle.h (FABLE_RETAIL_CENGINEPRIMITIVEHANDLE + _DTOR: the inline destructor is Clear(),
//   as the handle's own ??_G 0x0041C500 shows), STLport vector<CEnginePrimitiveHandle>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitiveHandle.h"

class CPrimitiveHandleSet_00441ee0 {
public:
    unsigned char                         _pad00[0x1c];
    std::vector<CEnginePrimitiveHandle>   Handles1c;   // +0x1c
    unsigned char                         _pad28[0x2c - 0x28];
    CEnginePrimitiveHandle                Handle2c;    // +0x2c
    CEnginePrimitiveHandle                Handle34;    // +0x34
    std::vector<CEnginePrimitiveHandle>   Handles3c;   // +0x3c
    std::vector<CEnginePrimitiveHandle>   Handles48;   // +0x48
    ~CPrimitiveHandleSet_00441ee0();
};

CPrimitiveHandleSet_00441ee0::~CPrimitiveHandleSet_00441ee0()
{
}