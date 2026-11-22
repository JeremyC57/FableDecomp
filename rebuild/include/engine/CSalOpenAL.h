#ifndef FABLE_CSAL_OPENAL_H
#define FABLE_CSAL_OPENAL_H
#include "CodaEnvironment.h"
#include "EAX40.h"
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "engine/CodaPCM.h"
#include "engine/KernelStreaming.h"
#endif
extern const GUID SAL_EAX30_ListenerProperties;
class CSalHw:public CCodaObjectBase {
public:virtual ~CSalHw();
};
// Layout: SDK CSalOpenAL members; native resolver and global object prove +e8/+f0.
// This retail library version has one extra pointer before those SDK slots.
// Entire 0x1b0 native object is preserved; unmodelled API pointers remain opaque.
class CSalOpenAL:public CSalHw {
public:
 virtual ~CSalOpenAL();
 void* ALGetProcAddress(unsigned char*);
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
 int GetError() {
  if (!m_pGetError)
   m_pGetError = (int (__cdecl*)())GetALProcAddress("alGetError");
  return m_pGetError();
 }
 int EnumToCRESULT(int error) const {
  switch (error) {
  case AL_NO_ERROR: return C_OK;
  case AL_OUT_OF_MEMORY: return C_ERR_OUTOFMEMORY;
  default: return C_ERR_NORESOURCES;
  }
 }
 int CheckError() { return EnumToCRESULT(GetError()); }
 void GenBuffers(unsigned int count, unsigned int* buffers) {
  if (!m_pGenBuffers)
   m_pGenBuffers = (void (__cdecl*)(unsigned int, unsigned int*))GetALProcAddress("alGenBuffers");
  m_pGenBuffers(count,buffers);
 }
 void DeleteBuffers(unsigned int count, unsigned int* buffers) {
  if (!m_pDeleteBuffers)
   m_pDeleteBuffers = (void (__cdecl*)(unsigned int, unsigned int*))GetALProcAddress("alDeleteBuffers");
  m_pDeleteBuffers(count,buffers);
 }
 void BufferData(unsigned int buffer, int format, void* data, unsigned int size, unsigned int frequency) {
  if (!m_pBufferData)
   m_pBufferData = (void (__cdecl*)(unsigned int, int, void*, unsigned int, unsigned int))GetALProcAddress("alBufferData");
  m_pBufferData(buffer,format,data,size,frequency);
 }
#endif
 int EAXSet(const GUID* guid,unsigned int property,unsigned int source,void* value,unsigned int size){
  if(!m_pEAXSet)m_pEAXSet=(int(__cdecl*)(const GUID*,unsigned int,unsigned int,void*,unsigned int))ALGetProcAddress((unsigned char*)"EAXSet");
  return m_pEAXSet(guid,property,source,value,size);
 }
 int EAXSetGlobal(const GUID* guid,unsigned int property,void* value,unsigned int size){return EAXSet(guid,property,0,value,size);}
 int EAX3SetGlobal(unsigned int property,void* value,unsigned int size){return EAXSetGlobal(&SAL_EAX30_ListenerProperties,property,value,size);}
private:
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
 // Inferred: the retail buffer reaches the SDK-private Suspend and Resume bodies.
 // Friendship tests that access path while keeping the SDK private API private.
 friend class CSalOpenALBuffer;
 void* GetALProcAddress(const char* function_name) {
  if (m_bRouter)
   return ALGetProcAddress((unsigned char*)function_name);
  else
   return (void*)::GetProcAddress(m_hModule, function_name);
 }
 void Suspend() {
  if (!m_pSuspendContext)
   m_pSuspendContext = (void (__cdecl*)(void*))::GetProcAddress(m_hModule,"alcSuspendContext");
  m_pSuspendContext(m_pContext);
  m_bSuspended = true;
 }
 void Resume() {
  if (!m_pProcessContext)
   m_pProcessContext = (void (__cdecl*)(void*))::GetProcAddress(m_hModule,"alcProcessContext");
  m_pProcessContext(m_pContext);
  m_bSuspended = false;
 }
 bool IsSuspended() const { return m_bSuspended; } // Descriptive: native 0x00C4D810.
 HMODULE m_hModule;
 unsigned char Opaque008[0x14];
 void (__cdecl* m_pSuspendContext)(void*);
 void (__cdecl* m_pProcessContext)(void*);
 unsigned char Opaque024[8];
 void (__cdecl* m_pGenBuffers)(unsigned int, unsigned int*);
 void (__cdecl* m_pDeleteBuffers)(unsigned int, unsigned int*);
 unsigned char Opaque034[4];
 void (__cdecl* m_pBufferData)(unsigned int, int, void*, unsigned int, unsigned int);
 unsigned char Opaque03c[0xa4];
 int (__cdecl* m_pGetError)();
 unsigned char Opaque0e4[4];
 void* (__cdecl* m_pGetProcAddress)(unsigned char*);
 unsigned char Opaque0ec[4];
 int (__cdecl* m_pEAXSet)(const GUID*,unsigned int,unsigned int,void*,unsigned int);
 unsigned char Opaque0f4[0x0c];
 void* m_pContext;
 bool m_bRouter;
 unsigned char Opaque105[0xab];
 bool m_bSuspended; // Descriptive: native Suspend and Resume write +0x1B0.
#else
 void* m_hModule;
 unsigned char Opaque008[0xe0];
 void* (__cdecl* m_pGetProcAddress)(unsigned char*);
 unsigned char Opaque0ec[4];
 int (__cdecl* m_pEAXSet)(const GUID*,unsigned int,unsigned int,void*,unsigned int);
 unsigned char Opaque0f4[0xbc];
#endif
};
extern CSalOpenAL g_OpenAL;
#endif
