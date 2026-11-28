#ifndef FABLE_CSAL_SW_BUFFER_H
#define FABLE_CSAL_SW_BUFFER_H
#include "engine/CCodaObjectBase.h"
class CSalSwBuffer:public CCodaObjectBase{
public: virtual ~CSalSwBuffer();
#ifdef FABLE_RETAIL_CSALSWBUFFER_STATIC_CTOR_PASS30
 // The SDK static buffer initializer calls the original out-of-line constructor.
 CSalSwBuffer();
#endif
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
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
public:
    bool IsValid() const { return IsCreated() && !IsEmpty(); }
#ifdef FABLE_RETAIL_CSALSWBUFFER_CURSOR_PORT30
    unsigned int GetReadPos() const { return m_ReadCursor; }
#endif
    unsigned int GetNumValid() const { return m_nValid; }
    unsigned int GetNumChannels() const { return m_nChannels; }
    unsigned int GetNumObjects() const { return m_nObjects; }
    unsigned int GetObjectSize() const { return m_cbObjectSize; }
    void* GetBuffer() const { return m_pvBuffer; }
    void CopyChannel(unsigned int, unsigned int);
    void* GetWriteBuffer() const { return (unsigned char*)m_pvBuffer + m_WriteCursor * m_cbObjectSize; }
    void Add(unsigned int nObjects);
    const void* GetReadBuffer() const { return (unsigned char*)m_pvBuffer + m_ReadCursor*m_cbObjectSize; }
    void Remove(unsigned int);
    void ZeroChannel(unsigned int, unsigned int);
    void Invalidate();
private:
#endif
 void * m_pvBuffer;
 unsigned int m_nObjects;
 unsigned int m_cbObjectSize;
 unsigned int m_nChannels;
 unsigned int m_ReadCursor;
 unsigned int m_WriteCursor;
 unsigned int m_nValid;
};
#endif
