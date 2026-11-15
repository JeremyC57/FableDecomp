#ifndef FABLE_PASS22_CFLOATCYCLICBUFFER_H
#define FABLE_PASS22_CFLOATCYCLICBUFFER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CFloatCyclicBuffer {
public:
 float * m_pBase; // +0x0
private:
 unsigned int m_uiLength; // +0x4
 unsigned int m_uiWriteCursor; // +0x8
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatCyclicBufferSizePass22[(sizeof(CFloatCyclicBuffer)==12)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
