#ifndef FABLE_CEAXBUFFER_H
#define FABLE_CEAXBUFFER_H
#include "CEAXListener.h"
// Layout: native ctor00c61610, allocation0x3c0, vtable012b9344.
// SDK names and exact access/signatures; native state excludes EAX1/2 conversions.
class CEAXBuffer:public CCodaObjectBase {
public:
 virtual ~CEAXBuffer();
 int GetEAX4Source(unsigned long,void*,unsigned long,unsigned long*);
 int CommitEAX3();
 unsigned long m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 bool m_bEAX4Active;
 EAXVERSION m_EAXVersion;
private:
 int Set3DBuffer(unsigned long,void*,unsigned long);
 CEAXListener* m_lpEAXListener;
 SOURCEBP m_Source;
 DESTINATIONBP m_Dest;
 BUFFER m_Buffer,m_Buffer_Defer;
 unsigned long m_dwFlags,m_dwFlags_Defer;
 long m_BufferID;
 // Remaining native0x4c storage differs from later SDK, and is untouched by these rows.
 unsigned char Opaque374[0x4c];
};
#endif
