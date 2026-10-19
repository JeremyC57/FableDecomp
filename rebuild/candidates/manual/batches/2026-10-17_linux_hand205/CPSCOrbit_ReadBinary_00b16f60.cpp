// CPSCOrbit::ReadBinary(CDataInputStream*) -- read centre parameter and three orbit records
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
struct CDataInputStream { void* v; unsigned long position; long sourceLength; const unsigned char* data; long chunkPosition; long remaining; void ReadWithSrcChunkOverflow(unsigned char*,long); static __forceinline bool valid(long long p){return p>=0&&p<=0x7fffffff;} __forceinline void Read(void* dst,long size){long long np=(long long)(position+size);if(!valid(np))return;if(remaining>=size){memcpy(dst,data,size);data+=size;remaining-=size;position+=size;}else ReadWithSrcChunkOverflow((unsigned char*)dst,size);} };
struct Orbit { long Type; bool Enabled; float Radius,Expand; bool CycleFlag; float CycleTime,SqueezeScale,SqueezeAngle,RotateSpeed,RotateStart,RotateSpeedRandom,RotateStartRandom; };
struct CPSCOrbit { unsigned char pad[0xc]; unsigned long CentreParam; Orbit Orbits[3]; void ReadBinary(CDataInputStream*); };
void CPSCOrbit::ReadBinary(CDataInputStream* in){unsigned long value; in->Read(&value,sizeof(value)); CentreParam=value; in->Read(Orbits,sizeof(Orbits));}
