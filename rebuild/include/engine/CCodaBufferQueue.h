#ifndef FABLE_CCODABUFFERQUEUE_H
#define FABLE_CCODABUFFERQUEUE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CodaSourceTypes.h"
class CCodaBufferQueue {
public:
 unsigned int GetSize()const{return m_nBuffers;}
 bool IsCreated()const{return GetSize()!=0;}
 bool IsEmpty()const{return IsCreated() && m_Format.ffFormat==0;}
private:
 CSalBuffer **m_ppBuffer;
 CREFTIME * m_pPlayTime;
 unsigned int m_nBuffers;
 unsigned int m_nAdd;
 unsigned int m_nPlay;
 void * m_hLastBuffer;
 CQUEUEFORMAT m_Format;
};
#endif
