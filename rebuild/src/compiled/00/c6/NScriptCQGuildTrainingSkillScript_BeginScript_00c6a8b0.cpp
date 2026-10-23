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