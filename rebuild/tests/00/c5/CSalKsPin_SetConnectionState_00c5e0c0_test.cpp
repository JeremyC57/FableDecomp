// Full original row body, with an observable property-call fixture.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif
struct _GUID {DWORD Data1;unsigned short Data2,Data3;unsigned char Data4[8];};typedef _GUID GUID;
enum KSSTATE {KSSTATE_STOP,KSSTATE_ACQUIRE,KSSTATE_PAUSE,KSSTATE_RUN};
enum KSPROPERTY_CONNECTION {KSPROPERTY_CONNECTION_STATE,KSPROPERTY_CONNECTION_PRIORITY,KSPROPERTY_CONNECTION_DATAFORMAT,KSPROPERTY_CONNECTION_ALLOCATORFRAMING,KSPROPERTY_CONNECTION_PROPOSEDATAFORMAT,KSPROPERTY_CONNECTION_ACQUIREORDERING,KSPROPERTY_CONNECTION_ALLOCATORFRAMING_EX,KSPROPERTY_CONNECTION_STARTAT};
#ifdef _MSC_VER
struct __declspec(uuid("1d58c920-ac9b-11cf-a5d6-28db04c10000")) KSPROPSETID_Connection;
#endif
class CSalFile {public:virtual ~CSalFile(){}private:void* m_hDevice;};
class CSalKsPin:public CSalFile {public:virtual ~CSalKsPin(){}bool SetConnectionState(KSSTATE);bool SetProperty(const GUID&,unsigned long,void*,unsigned long);};
static CSalKsPin* ExpectedThis;static KSSTATE ExpectedState;static bool Result;static int Calls;
bool CSalKsPin::SetProperty(const GUID& guid,unsigned long id,void* value,unsigned long size){
 static const GUID expected={0x1d58c920,0xac9b,0x11cf,{0xa5,0xd6,0x28,0xdb,0x04,0xc1,0x00,0x00}};
 ++Calls;assert(this==ExpectedThis && id==KSPROPERTY_CONNECTION_STATE && size==4);
 assert(memcmp(&guid,&expected,sizeof(GUID))==0 && *static_cast<KSSTATE*>(value)==ExpectedState);
 *static_cast<KSSTATE*>(value)=KSSTATE_STOP;return Result;
}
bool CSalKsPin::SetConnectionState(KSSTATE state)
{
#ifdef _MSC_VER
    return SetProperty(__uuidof(KSPROPSETID_Connection), KSPROPERTY_CONNECTION_STATE, &state, sizeof(state));
#else
    // Public KS GUID value, equivalent to the compiler's UUID metadata on other compilers.
    const GUID connection = {0x1d58c920,0xac9b,0x11cf,{0xa5,0xd6,0x28,0xdb,0x04,0xc1,0x00,0x00}};
    return SetProperty(connection, KSPROPERTY_CONNECTION_STATE, &state, sizeof(state));
#endif
}
int main(){assert(sizeof(GUID)==16 && sizeof(KSSTATE)==4);CSalKsPin pin;ExpectedThis=&pin;
 for(int state=0;state<4;++state)for(int success=0;success<2;++success){
  KSSTATE original=static_cast<KSSTATE>(state);ExpectedState=original;Result=success!=0;Calls=0;
  assert(pin.SetConnectionState(original)==Result && Calls==1);assert(original==ExpectedState);
 }
 puts("00c5e0c0_TEST PASS");return 0;
}