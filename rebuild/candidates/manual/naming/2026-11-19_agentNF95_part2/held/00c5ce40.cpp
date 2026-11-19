// CSalSwBuffer::Release() -- reset the software buffer to its empty, unallocated state: the buffer pointer, object count, object size, channel count, cursors and valid count (+0x04..+0x1C) to 0
// Leaf: ?Release@CSalSwBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5CE40, thiscall, plain `ret`.
// NF95 (part 2, q11406): was the descriptive stand-in CAudioRingBuffer::Clear (its own local model). GameCODA 2.10 SalSw3DVoice.obj
//   CalcParams release relocation 0x3d (inside the inlined ReleasePlayBuffer; the Dbg object calls ?ReleasePlayBuffer@CSalSwBaseVoice@@IAEXXZ)
//   + the SDK CodeView: public non-virtual ?Release@CSalSwBuffer@@QAEXXZ, declared by engine/CSalSwBuffer.h (opt-in
//   FABLE_RETAIL_CSALSW3D_CALCPARAMS, landed by 0x00C54C10). Members: the header's CodeView names.
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "engine/CSalSwBuffer.h"

void CSalSwBuffer::Release()
{
    m_pvBuffer = 0;
    m_nObjects = 0;
    m_cbObjectSize = 0;
    m_nChannels = 0;
    m_ReadCursor = 0;
    m_WriteCursor = 0;
    m_nValid = 0;
}
