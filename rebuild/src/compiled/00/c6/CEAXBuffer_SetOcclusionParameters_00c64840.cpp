// CEAXBuffer::SetOcclusionParameters(const EAXOCCLUSIONPROPERTIES* p) -- validate and store the EAX 4.0 source occlusion block.
// Leaf: ?SetOcclusionParameters@CEAXBuffer@@QAEJPBU_EAXOCCLUSIONPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C64840, 245 bytes.
// Name (owner decision 2026-11-03 D3): a CEAXBuffer (pass 11 CEaxSource) method; the EAX 4.0 source dispatcher 0x00C68510 calls it
//   for EAXSOURCE_OCCLUSIONPARAMETERS (16-byte payload) under this name. Adjacent 0x00C647B0 takes the 8-byte obstruction block,
//   0x00C64940 the 8-byte exclusion block. Float pools 0x0122DEDC = 0, 0x0122DED8 = 1, 0x01237F0C = 10. Earlier labels:
//   CEntity::SetVectorProperty_XYZW (propagated), CAudioOcclusionState::SetOcclusion (descriptive).
// Fields: dirty bits +4 (m_dwEAX3Changes), listener +0x14 (its m_lEAX4ErrorCode +0x3d4 through SetEAX4ErrorCode), occlusion
//   record +0x30 (m_Source.eax4BP.lOcclusion..flOcclusionDirectRatio).
#include "engine/CEAXBuffer.h"

long CEAXBuffer::SetOcclusionParameters(const EAXOCCLUSIONPROPERTIES* p)
{
    if (p->lOcclusion < -10000 || p->lOcclusion > 0 || p->flOcclusionLFRatio < 0.0f || p->flOcclusionLFRatio > 1.0f ||
        p->flOcclusionRoomRatio < 0.0f || p->flOcclusionRoomRatio > 10.0f || p->flOcclusionDirectRatio < 0.0f ||
        p->flOcclusionDirectRatio > 10.0f) {
        m_lpEAXListener->SetEAX4ErrorCode(-2);
        return -3;
    }
    if (p->lOcclusion != m_Source.eax4BP.lOcclusion || p->flOcclusionLFRatio != m_Source.eax4BP.flOcclusionLFRatio ||
        p->flOcclusionRoomRatio != m_Source.eax4BP.flOcclusionRoomRatio ||
        p->flOcclusionDirectRatio != m_Source.eax4BP.flOcclusionDirectRatio) {
        m_Source.eax4BP.lOcclusion = p->lOcclusion;
        m_Source.eax4BP.flOcclusionLFRatio = p->flOcclusionLFRatio;
        m_Source.eax4BP.flOcclusionRoomRatio = p->flOcclusionRoomRatio;
        m_Source.eax4BP.flOcclusionDirectRatio = p->flOcclusionDirectRatio;
        m_dwEAX3Changes |= 0x400000;
    }
    return 0;
}
