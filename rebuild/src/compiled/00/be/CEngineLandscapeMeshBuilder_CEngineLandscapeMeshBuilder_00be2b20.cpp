// CEngineLandscapeMeshBuilder::~CEngineLandscapeMeshBuilder() -- delete the shared index buffer of every power-of-two patch-size pair, then ~CBaseClassNonCopyable
// Leaf: ??1CEngineLandscapeMeshBuilder@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2B20, 74 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): stores vtable 0x012A7FB8 first (RTTI .?AVCEngineLandscapeMeshBuilder@@ : CBaseClassNonCopyable : CBaseClass);
//   callers are that vtable's slot-0 scalar deleting dtor 0x00BE32D0 and the owner's destructor (member at +0x10, 0x00B67E16). The
//   ctor 0x00BE32A0 stores the same vtable, zeroes 0x121 = 17 x 17 dwords from +0x04 and calls BuildSharedIndexBuffers. Ego_r
//   (0x009C0780) and FableWin (0x02CAD340) spell ??1CEngineLandscapeMeshBuilder@@UAE@XZ.
// Callees: each buffer's virtual scalar deleting dtor (CIndexBuffer vtable 0x0129EF2C slot 0, `push 1 ; call [vtable]`: `delete p`
//   on a polymorphic pointer, null-tested), ~CBaseClassNonCopyable 0x0099A300.
// Layout: SharedIndexBuffers CIndexBuffer*[17][17] at +0x04 (Ego_r member, retail offset from the ctor's rep stosd). The TLC class
//   has no other member with a destructor (Ego_r's Layers / PassMappingTable CArrays are not destroyed here: the retail class
//   differs from the Ego_r one); no engine header exists for it. CIndexBuffer from engine/CIndexBuffer.h (retail opt-in
//   FABLE_RETAIL_CINDEXBUFFER, corrected in this chunk: polymorphic CMovableResource base).
// Shape: two doubling loops `for (i = 2; i <= 16; i *= 2) for (j = 2; j <= 16; j *= 2) delete SharedIndexBuffers[i][j];` (VC7.1
//   strength-reduces i*17 into shl 4 + add and doubles with add reg,reg); a user destructor (it stores its own vtable).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_CINDEXBUFFER
#include "engine/CIndexBuffer.h"

class CEngineLandscapeMeshBuilder : public CBaseClassNonCopyable {   // vtable 0x012A7FB8
public:
    CEngineLandscapeMeshBuilder();                              // 0x00BE32A0
    virtual ~CEngineLandscapeMeshBuilder();
    CIndexBuffer* SharedIndexBuffers[17][17];                   // +0x04
};

CEngineLandscapeMeshBuilder::~CEngineLandscapeMeshBuilder()
{
    for (long i = 2; i <= 16; i *= 2)
        for (long j = 2; j <= 16; j *= 2)
            delete SharedIndexBuffers[i][j];
}