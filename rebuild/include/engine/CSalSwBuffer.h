#ifndef FABLE_CSAL_SW_BUFFER_H
#define FABLE_CSAL_SW_BUFFER_H
#include "engine/CCodaObjectBase.h"
class CSalSwBuffer:public CCodaObjectBase{
public: virtual ~CSalSwBuffer();
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
