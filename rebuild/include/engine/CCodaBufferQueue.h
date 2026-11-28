#ifndef FABLE_CCODABUFFERQUEUE_H
#define FABLE_CCODABUFFERQUEUE_H
#include "engine/CODA_PlatformTypes.h"
#include "engine/CodaSourceTypes.h"
class CCodaBufferQueue {
public:
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
 unsigned int Advance();
 unsigned int GetPlay() const { return m_nPlay; }
 bool IsReady() const { return m_ppBuffer[m_nPlay] != 0; }
 CREFTIME GetPlayTime() const { return m_pPlayTime[m_nPlay]; }
#ifndef FABLE_RETAIL_CCODASOURCE_EAX
 CSalBuffer* GetPlayBuffer() const { return IsCreated() ? m_ppBuffer[m_nPlay] : 0; }
#endif
#endif
 unsigned int GetSize()const{return m_nBuffers;}
 bool IsCreated()const{return GetSize()!=0;}
 bool IsEmpty()const{return IsCreated() && m_Format.ffFormat==0;}
#ifdef FABLE_RETAIL_CCODASOURCE_EAX
 // Retail opt-in (w24C, rows 0x00C44090 / 0x00C44100): the buffer at the play index, or null for an uncreated queue (native
 // `mov eax,[ecx+64h] ; mov edx,[ecx+54h] ; mov eax,[edx+eax*4]` behind its own size test, 0x00C440A9). Descriptive name.
 CSalBuffer* GetPlayBuffer()const{if(GetSize())return m_ppBuffer[m_nPlay];return 0;}
#endif
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
