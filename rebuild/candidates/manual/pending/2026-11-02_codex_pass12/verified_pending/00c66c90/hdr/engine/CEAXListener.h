#ifndef FABLE_CEAXLISTENER_H
#define FABLE_CEAXLISTENER_H
#include "EAXNativeTypes.h"
#include "CodaEnvironment.h"
class CEAXBuffer;
class CEAXBufferList:public CCodaObjectBase {
public:
 virtual ~CEAXBufferList();
 CEAXBufferList* lpNext;
 CEAXBuffer* lpEAXBuffer;
};
// Layout: native ctor00c604e0, allocation0x4f0, vtable012b9340.
// Original SDK public/private names, with EAX1/2 conversion fields absent natively.
class CEAXListener:public CCodaObjectBase {
public:
 virtual ~CEAXListener();
 int GetEAX4FXSlot(unsigned long,unsigned long,void*,unsigned long,unsigned long*);
 void AddToChangesList(IDENTIFIER,unsigned long,void*,long);
 void SetEAX4ErrorCode(long code){m_lEAX4ErrorCode=code;}
 unsigned long m_ListenerSlotID;
 CEAXBufferList* m_lpBufferList;
 LISTENER m_Listener,m_Listener_Defer;
 SOURCE m_Source;
 DESTINATION m_Dest;
 unsigned long m_dwFlags,m_dwFlags_Defer,m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 _CHANGES* lpChanges;
 float m_flDecayHFFactor;
 bool m_bEnsoniq;
 EAXVERSION m_EAXVersion;
private:
 long m_lEAX4ErrorCode;
 float m_flEAX4UnsizedDecayTime[4],m_flEAX4UnsizedDecayTimeSize[4];
 float m_flEAX4UnsizedReflections[4],m_flEAX4UnsizedReflectionsSize[4];
 float m_flEAX4UnsizedReflectionsDelay[4],m_flEAX4UnsizedReflectionsDelaySize[4];
 float m_flEAX4UnsizedReverb[4],m_flEAX4UnsizedReverbSize[4];
 float m_flEAX4UnsizedReverbDelay[4],m_flEAX4UnsizedReverbDelaySize[4];
 float m_flEAX4UnsizedEchoTime[4],m_flEAX4UnsizedEchoTimeSize[4];
 float m_flEAX4UnsizedModulationTime[4],m_flEAX4UnsizedModulationTimeSize[4];
 float m_flEAX3UnsizedDecayTime,m_flEAX3UnsizedDecayTimeSize;
 float m_flEAX3UnsizedReflections,m_flEAX3UnsizedReflectionsSize;
 float m_flEAX3UnsizedReflectionsDelay,m_flEAX3UnsizedReflectionsDelaySize;
 float m_flEAX3UnsizedReverb,m_flEAX3UnsizedReverbSize;
 float m_flEAX3UnsizedReverbDelay,m_flEAX3UnsizedReverbDelaySize;
 float m_flEAX3UnsizedEchoTime,m_flEAX3UnsizedEchoTimeSize;
 float m_flEAX3UnsizedModulationTime,m_flEAX3UnsizedModulationTimeSize;
};
#endif
