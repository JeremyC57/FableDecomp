// Full target body; observable call boundary and stable opaque state.
#include <assert.h>
#include <stdio.h>
#include <string.h>
struct CSoundEventManager;
struct CSoundSample;
class CHandleBasedMemoryPool_012b4098 {public:void* GetPointerFromHandle(long handle);};
struct CSoundEventManager {unsigned char OpaqueManagerPrefix[0x360];CHandleBasedMemoryPool_012b4098 m_pool;};
struct CStreamBuffer {CSoundSample* m_pSample;long m_handle;unsigned char OpaqueBufferState[0x24];void* GetPoolData(CSoundEventManager* manager);};
static CHandleBasedMemoryPool_012b4098* ExpectedPool;
static long ExpectedHandle;static void* Result;static int Calls;
void* CHandleBasedMemoryPool_012b4098::GetPointerFromHandle(long handle){assert(this==ExpectedPool && handle==ExpectedHandle);++Calls;return Result;}
void* CStreamBuffer::GetPoolData(CSoundEventManager* manager)
{
    return manager->m_pool.GetPointerFromHandle(m_handle);
}

int main(){
 CSoundEventManager managers[2];CStreamBuffer buffer;unsigned char values[4];
 const long handles[]={-1,0,1,127,2147483647L};
 for(int owner=0;owner<2;++owner)for(int h=0;h<5;++h)for(int result=0;result<5;++result){
  memset(&buffer,0x5a,sizeof(buffer));buffer.m_pSample=0;buffer.m_handle=handles[h];
  unsigned char before[sizeof(buffer)];memcpy(before,&buffer,sizeof(buffer));
  ExpectedPool=&managers[owner].m_pool;ExpectedHandle=handles[h];Result=result?values+result-1:0;Calls=0;
  assert(buffer.GetPoolData(&managers[owner])==Result && Calls==1);assert(memcmp(before,&buffer,sizeof(buffer))==0);
 }
 puts("00c26260_TEST PASS");return 0;
}