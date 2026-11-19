#ifndef FABLE_PASS22_CFLOATCYCLICBUFFER_H
#define FABLE_PASS22_CFLOATCYCLICBUFFER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#ifdef FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#include <string.h>
#endif
class CFloatCyclicBuffer {
public:
 float * m_pBase; // +0x0
#ifdef FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
 // Retail opt-in (NF102, family): SDK 2.10 CodeView public inlines (CODAApiDbg.lib out-of-line copies ?GetLength / ?GetWriteCursor
 // @CFloatCyclicBuffer@@QBEIXZ, ?SetWriteCursor / ?AddWriteCursor@CFloatCyclicBuffer@@QAEXI@Z; AddWriteCursor body from the Dbg copy).
 unsigned int GetLength() const { return m_uiLength; }
 unsigned int GetWriteCursor() const { return m_uiWriteCursor; }
 void SetWriteCursor(unsigned int uiCursor) { m_uiWriteCursor = uiCursor; }
 void AddWriteCursor(unsigned int uiSamples) { m_uiWriteCursor += uiSamples; if (m_uiWriteCursor >= m_uiLength) m_uiWriteCursor -= m_uiLength; }
#endif
#ifdef FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatCyclicBuffer@@QAEXXZ (CODAApi.lib COMDAT: the
 // counted-memset intrinsic over m_uiLength floats from m_pBase), expanded by the release CFloatReverbDspEngine::Clear 0x00C69B60.
 void Clear() { memset(m_pBase, 0, m_uiLength * sizeof(float)); }
#endif
#ifdef FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
 // Retail opt-in (NF105, family of q11548 / q11549): SDK 2.10 CodeView public inline ?Create@CFloatCyclicBuffer@@QAEXPAMI@Z (CODAApi.lib
 // COMDAT: m_pBase, m_uiLength, cursor 0), expanded by the release component Create members.
 void Create(float* pfBase, unsigned int uiLength) { m_pBase = pfBase; m_uiLength = uiLength; m_uiWriteCursor = 0; }
#endif
private:
 unsigned int m_uiLength; // +0x4
 unsigned int m_uiWriteCursor; // +0x8
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatCyclicBufferSizePass22[(sizeof(CFloatCyclicBuffer)==12)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
