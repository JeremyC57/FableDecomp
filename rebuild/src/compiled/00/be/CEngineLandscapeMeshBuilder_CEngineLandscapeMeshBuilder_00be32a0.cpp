// CEngineLandscapeMeshBuilder::CEngineLandscapeMeshBuilder() -- CBase ctor, vtable, memset of the 0x484-byte state block (rep stosd 0x121 dwords), then the out-of-line initialiser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE32A0, length 39 bytes.
// Callees: 0x0099A2F0 CBase::CBase, 0x00BE3110 Initialise (manifest: BuildSharedIndexBuffers, propagated)
// The vtable (0x012A7FB8) is CEngineLandscapeMeshBuilder's per rtti-vtables.tsv; the manifest module label
// (C3DMeshFileBoneChunk) is propagated.
//
#include "rebuild_abi.h"
#include <string.h>
struct CBase {
    virtual void v0();
    CBase();                                            // 0x0099A2F0
};
class CEngineLandscapeMeshBuilder : public CBase { public:
    unsigned long m_state[0x121];                       // +0x04 .. +0x487, zero-filled
    CEngineLandscapeMeshBuilder();
    void Initialise();                                  // 0x00BE3110
};
CEngineLandscapeMeshBuilder::CEngineLandscapeMeshBuilder()
{
    memset(m_state, 0, sizeof(m_state));
    Initialise();
}