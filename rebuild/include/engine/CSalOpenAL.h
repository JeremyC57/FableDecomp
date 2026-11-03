#ifndef FABLE_CSAL_OPENAL_H
#define FABLE_CSAL_OPENAL_H
#include "CodaEnvironment.h"
#include "EAX40.h"
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
 int EAXSet(const GUID* guid,unsigned int property,unsigned int source,void* value,unsigned int size){
  if(!m_pEAXSet)m_pEAXSet=(int(__cdecl*)(const GUID*,unsigned int,unsigned int,void*,unsigned int))ALGetProcAddress((unsigned char*)"EAXSet");
  return m_pEAXSet(guid,property,source,value,size);
 }
 int EAXSetGlobal(const GUID* guid,unsigned int property,void* value,unsigned int size){return EAXSet(guid,property,0,value,size);}
 int EAX3SetGlobal(unsigned int property,void* value,unsigned int size){return EAXSetGlobal(&SAL_EAX30_ListenerProperties,property,value,size);}
private:
 void* m_hModule;
 unsigned char Opaque008[0xe0];
 void* (__cdecl* m_pGetProcAddress)(unsigned char*);
 unsigned char Opaque0ec[4];
 int (__cdecl* m_pEAXSet)(const GUID*,unsigned int,unsigned int,void*,unsigned int);
 unsigned char Opaque0f4[0xbc];
};
extern CSalOpenAL g_OpenAL;
#endif
