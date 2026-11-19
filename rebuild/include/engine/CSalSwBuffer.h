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
#if defined(FABLE_RETAIL_CSALSWBUFFER_ISEMPTY) && defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS)
 // Retail opt-in (NF99, q11406, row 0x00C5B300): SDK 2.10 CodeView public const inline ?IsEmpty@CSalSwBuffer@@QBE_NXZ; the
 // CODAApiDbg SalSwBaseVoice.obj out-of-line copy calls IsCreated, then tests m_nValid (+0x1c) == 0.
 bool IsEmpty() const { return IsCreated() && m_nValid == 0; }
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
