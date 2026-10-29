// CEngineLandscapeMeshBuilder::CEngineLandscapeMeshBuilder() -- CBase ctor, vtable, memset of the 0x484-byte state block (rep stosd 0x121 dwords), then the out-of-line initialiser
// Leaf: ??0CEngineLandscapeMeshBuilder@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE32A0, length 39 bytes.
// Callees: 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable, 0x00BE3110 Initialise (manifest: BuildSharedIndexBuffers, propagated)
// The vtable (0x012A7FB8) is CEngineLandscapeMeshBuilder's per rtti-vtables.tsv; the manifest module label
// (C3DMeshFileBoneChunk) is propagated.
//
#include "rebuild_abi.h"
#include <string.h>
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                            // 0x0099A2F0
};
class CEngineLandscapeMeshBuilder : public CBaseClassNonCopyable { public:
    unsigned long m_state[0x121];                       // +0x04 .. +0x487, zero-filled
    CEngineLandscapeMeshBuilder();
    void Initialise();                                  // 0x00BE3110
};
CEngineLandscapeMeshBuilder::CEngineLandscapeMeshBuilder()
{
    memset(m_state, 0, sizeof(m_state));
    Initialise();
}