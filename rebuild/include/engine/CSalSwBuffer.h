#ifndef FABLE_CSAL_SW_BUFFER_H
#define FABLE_CSAL_SW_BUFFER_H
#include "engine/CCodaObjectBase.h"
class CSalSwBuffer:public CCodaObjectBase{
public: virtual ~CSalSwBuffer();
#ifdef FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
 bool IsCreated() const { return m_pvBuffer != 0; }
#endif
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView public non-virtual ?Release@CSalSwBuffer@@QAEXXZ (native 0x00C5CE40).
 void Release();
#endif
private:
 void * m_pvBuffer;
 unsigned int m_nObjects;
 unsigned int m_cbObjectSize;
 unsigned int m_nChannels;
 unsigned int m_ReadCursor;
 unsigned int m_WriteCursor;
 unsigned int m_nValid;
};
#endif
