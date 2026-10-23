// BEGIN exact source snapshot SHA256 b63a9f441ecd1f3810c2e17cba5aa6ad5254180e60f51e43ec04d238960e5d9d
// 00c6a8b0 catalog BeginScript/CQ_GuildTrainingSkillScript is generated, unverified.
// Inferred BufferState::Configure: compare cached6 configuration words, reset scratch,
// release old allocation, copy10 words, query output size, allocate16-aligned storage,
// build it and mark ready. All names are descriptive; no recovered owner/header available.
struct Configuration {unsigned long Ignored,A,B,C,D,E,F,G,H,I,J;};
struct Backend {virtual void Begin()=0;};
extern Backend* Backend_0143ccdc;extern int CacheEnabled_0143cce0;
void __cdecl ReleaseStorage(void*); //00c3c370
void* __cdecl AllocateStorage(unsigned long); //00c3c360
struct ResetState {unsigned char Data[0x24];void Reset();}; //00c75000
struct BufferState {
 unsigned char BeforeCurrent[0x18];int Current;unsigned char BeforeSaved[4];int Saved;
 unsigned char BeforeConfig[8];unsigned long A,B,C,D,E,F,G,H,I,J;
 unsigned char BeforeReset[0x25c-0x54];ResetState Scratch;void* Allocation;void* Aligned;
 unsigned char BeforeReady[0x294-0x288];bool Ready;
 bool ReallocateStateBuffers(); //00c775e0; inferred name: reallocates3 equal zeroed byte buffers
 void Build(const Configuration*,void*,unsigned long*); //00c6a460
 bool Configure(const Configuration*);
};
bool BufferState::Configure(const Configuration* in){
 if(CacheEnabled_0143cce0>0&&in->B==B&&in->A==A&&in->C==C&&in->D==D&&in->E==E&&in->F==F)return true;
 Ready=false;Scratch.Reset();if(Allocation){ReleaseStorage(Allocation);Allocation=0;}
 A=in->A;B=in->B;C=in->C;D=in->D;E=in->E;F=in->F;G=in->G;H=in->H;I=in->I;J=in->J;
 Backend_0143ccdc->Begin();
 if(!ReallocateStateBuffers())return false;
 unsigned long size=0;Build(in,0,&size);Allocation=AllocateStorage(size+15);
 if(!Allocation)return false;
 Aligned=(void*)(((unsigned long)Allocation+15)&~15UL);Build(in,Aligned,&size);
 Saved=Current;Current=-1;Ready=true;return true;
}

// END exact source snapshot
#include <stdio.h>
static unsigned char storage[128];static int resets,frees,builds,begins;static bool stateAllocationSucceeds=true,alloc=true;
int CacheEnabled_0143cce0;struct TestBackend:Backend{void Begin(){++begins;}}backend;Backend* Backend_0143ccdc=&backend;
void ResetState::Reset(){++resets;}void __cdecl ReleaseStorage(void*){++frees;}
void* __cdecl AllocateStorage(unsigned long n){return alloc&&n==47?storage+1:0;}
bool BufferState::ReallocateStateBuffers(){return stateAllocationSucceeds;}void BufferState::Build(const Configuration*,void* p,unsigned long* n){++builds;*n=32;if(p&&((unsigned long)p&15))builds=99;}
int main(){BufferState s;Configuration c;c.A=1;c.B=2;c.C=3;c.D=4;c.E=5;c.F=6;c.G=7;c.H=8;c.I=9;c.J=10;s.Allocation=0;s.Current=42;
 if(!s.Configure(&c)||!s.Ready||s.Current!=-1||s.Saved!=42||builds!=2||s.J!=10)return 1;
 CacheEnabled_0143cce0=1;c.J=11;if(!s.Configure(&c)||builds!=2||s.J!=10)return 2;
 c.A=12;stateAllocationSucceeds=false;if(s.Configure(&c)||s.Ready||frees!=1||s.Allocation)return 3;
 stateAllocationSucceeds=true;alloc=false;CacheEnabled_0143cce0=0;if(s.Configure(&c)||s.Ready||s.Allocation)return 4;
 puts("00c6a8b0_TEST PASS");return 0;}