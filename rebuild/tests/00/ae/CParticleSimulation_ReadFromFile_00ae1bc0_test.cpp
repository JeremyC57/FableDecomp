// Self-contained full-row behavior fixture; engine dependencies are test doubles.
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
struct C4DVector { float X,Y,Z,W; };
struct CMovableResourceMemoryPool { bool allow; int calls; CMovableResourceMemoryPool(bool a):allow(a),calls(0){} };
// Fixture ABI: original32-bit long/unsigned long parameters/fields use int/unsigned
// int on both hosts; the production Read BODY below is copied verbatim.
#ifndef _MSC_VER
typedef long long __int64;
#endif
class CDataInputStream {
 std::vector<unsigned char> data;
 void ReadWithSrcChunkOverflow(unsigned char* dst,int no_bytes) {
   ++overflow; if(StreamPos+(unsigned int)no_bytes>data.size())std::exit(90);
   std::memcpy(dst,&data[StreamPos],no_bytes);StreamPos+=no_bytes;
   CurrentSourcePtr=&data[0]+StreamPos;SourceChunkBytesRemaining=(int)data.size()-(int)StreamPos;
 }
public:
 unsigned int StreamPos; int StreamSize; const unsigned char* CurrentSourcePtr;
 unsigned int SourceChunkStreamPos; int SourceChunkBytesRemaining; int overflow;
 CDataInputStream(const std::vector<unsigned char>&d,unsigned long end):data(d),StreamPos(0),StreamSize((int)d.size()),CurrentSourcePtr(&data[0]),SourceChunkStreamPos(0),SourceChunkBytesRemaining((int)(end<d.size()?end:d.size())),overflow(0){}
    void Read(void* dst,int no_bytes) {
        if(no_bytes>0) {
            __int64 overflow_check=(__int64)(StreamPos+no_bytes);
            if(overflow_check<0 || overflow_check>0x7fffffff)return;
            if(no_bytes<=SourceChunkBytesRemaining) {
                memcpy(dst,CurrentSourcePtr,no_bytes);
                CurrentSourcePtr+=no_bytes;
                SourceChunkBytesRemaining-=no_bytes;
                StreamPos+=no_bytes;
            } else ReadWithSrcChunkOverflow((unsigned char*)dst,no_bytes);
        }
    }
 float ReadFloat(){float f;Read(&f,4);return f;}
 long ReadSLONG(){int n;Read(&n,4);return n;}
 unsigned long ReadULONG(){unsigned int n;Read(&n,4);return n;}
 bool ReadEBOOL(){unsigned char n;Read(&n,1);return n!=0;}
};
class CParticleSimulation {
 std::vector<C4DVector> p,l,s; std::vector<float> a;
public:
 float Timestep,TimestepMultiplier,GravityStrength,WindStrength,DraggingStrength,GlobalDamping;
 bool TimestepChanged,DraggingEnable,DraggingRotational,AccelerationEnable,BoundingBoxValid;
 unsigned long Size; C4DVector *Positions,*LastPositions,*SavedPositions; float* SimulationAlphas; int vectorCalls;
 CParticleSimulation():Timestep(-1),TimestepMultiplier(-1),GravityStrength(-1),WindStrength(-1),DraggingStrength(-1),GlobalDamping(-1),TimestepChanged(false),DraggingEnable(false),DraggingRotational(false),AccelerationEnable(false),BoundingBoxValid(true),Size(0),Positions(0),LastPositions(0),SavedPositions(0),SimulationAlphas(0),vectorCalls(0){}
 bool AllocateArrays(CMovableResourceMemoryPool& pool){++pool.calls;if(!pool.allow)return false;p.resize(Size+1);l.resize(Size+1);s.resize(Size+1);a.resize(Size+1);Positions=&p[0];LastPositions=&l[0];SavedPositions=&s[0];SimulationAlphas=&a[0];return true;}
 void ReadVectorArray(C4DVector* out,unsigned long n,CDataInputStream& in,float w){++vectorCalls;for(unsigned long i=0;i<n;++i){out[i].X=in.ReadFloat();out[i].Y=in.ReadFloat();out[i].Z=in.ReadFloat();out[i].W=w;}}
 bool ReadFromFile(CDataInputStream& in,CMovableResourceMemoryPool& pool);
};
bool CParticleSimulation::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc)
{
    Timestep = is.ReadFloat();
    TimestepChanged = is.ReadULONG() != 0;
    TimestepMultiplier = is.ReadFloat();
    Size = is.ReadULONG();
    if (!AllocateArrays(alloc))
        return false;
    ReadVectorArray(Positions, Size, is, 0.0f);
    std::copy(Positions, Positions + Size, LastPositions);
    std::copy(Positions, Positions + Size, SavedPositions);
    is.Read(SimulationAlphas, Size * sizeof(float));
    GravityStrength = is.ReadFloat();
    WindStrength = is.ReadFloat();
    DraggingEnable = is.ReadEBOOL();
    DraggingRotational = is.ReadEBOOL();
    DraggingStrength = is.ReadFloat();
    AccelerationEnable = is.ReadEBOOL();
    GlobalDamping = is.ReadFloat();
    BoundingBoxValid = false;
    return true;
}

static void ck(bool ok,int line){if(!ok){std::printf("FAIL line %d\n",line);std::exit(1);}}
#define CHECK(x) ck((x),__LINE__)
static void word(std::vector<unsigned char>& d,unsigned int x){unsigned char b[4];std::memcpy(b,&x,4);d.insert(d.end(),b,b+4);}
static void flt(std::vector<unsigned char>& d,float x){unsigned char b[4];std::memcpy(b,&x,4);d.insert(d.end(),b,b+4);}
static std::vector<unsigned char> payload(unsigned int n,bool flags){std::vector<unsigned char>d;flt(d,.5f);word(d,flags?2:0);flt(d,1.5f);word(d,n);for(unsigned int i=0;i<n;++i){flt(d,float(i)+1.25f);flt(d,float(i)+2.5f);flt(d,-float(i)-.75f);}for(unsigned int i=0;i<n;++i)flt(d,.25f*float(i+1));flt(d,9.5f);flt(d,-2.5f);d.push_back(flags?3:0);d.push_back(flags?0:5);flt(d,4.25f);d.push_back(flags?7:0);flt(d,.75f);return d;}
static void run(unsigned int n,unsigned long end,bool flags,bool allow){std::vector<unsigned char>d=payload(n,flags);CDataInputStream in(d,end);CMovableResourceMemoryPool pool(allow);CParticleSimulation sim;bool result=sim.ReadFromFile(in,pool);CHECK(result==allow);CHECK(pool.calls==1);CHECK(sim.Timestep==.5f);CHECK(sim.TimestepChanged==flags);CHECK(sim.TimestepMultiplier==1.5f);CHECK(sim.Size==n);if(!allow){CHECK(in.StreamPos==16);CHECK(sim.vectorCalls==0);CHECK(sim.Positions==0&&sim.LastPositions==0&&sim.SavedPositions==0&&sim.SimulationAlphas==0);CHECK(sim.GravityStrength==-1&&sim.WindStrength==-1&&sim.GlobalDamping==-1);CHECK(sim.BoundingBoxValid);return;}CHECK(sim.vectorCalls==1);CHECK(in.StreamPos==d.size());CHECK(in.overflow==(end<d.size()?1:0));CHECK(!sim.BoundingBoxValid);CHECK(sim.GravityStrength==9.5f&&sim.WindStrength==-2.5f);CHECK(sim.DraggingEnable==flags&&sim.DraggingRotational==!flags);CHECK(sim.DraggingStrength==4.25f&&sim.AccelerationEnable==flags&&sim.GlobalDamping==.75f);for(unsigned int i=0;i<n;++i){CHECK(sim.Positions[i].X==float(i)+1.25f&&sim.Positions[i].Y==float(i)+2.5f&&sim.Positions[i].Z==-float(i)-.75f&&sim.Positions[i].W==0);CHECK(std::memcmp(&sim.Positions[i],&sim.LastPositions[i],sizeof(C4DVector))==0);CHECK(std::memcmp(&sim.Positions[i],&sim.SavedPositions[i],sizeof(C4DVector))==0);CHECK(sim.SimulationAlphas[i]==.25f*float(i+1));}}
static void read_boundaries(){
 std::vector<unsigned char>d(16);for(unsigned int i=0;i<16;++i)d[i]=(unsigned char)(i+1);
 for(int n=-2;n<=8;++n)for(unsigned int initial=0;initial<4;++initial){
  CDataInputStream is(d,16);unsigned char output[16];std::memset(output,0x55,16);
  is.StreamPos=initial;const unsigned char* before=is.CurrentSourcePtr;is.Read(output,n);
  CHECK(is.StreamPos==initial+(n>0?(unsigned int)n:0));CHECK(is.SourceChunkBytesRemaining==16-(n>0?n:0));
  CHECK(is.CurrentSourcePtr==before+(n>0?n:0));CHECK(is.overflow==0);
  for(int j=0;j<16;++j)CHECK(output[j]==(j<n?d[j]:0x55));
 }
 {CDataInputStream is(d,16);unsigned char output[4]={0x55,0x55,0x55,0x55};is.StreamPos=0x7ffffffdU;is.Read(output,4);CHECK(is.StreamPos==0x7ffffffdU&&is.SourceChunkBytesRemaining==16&&output[0]==0x55);}
 {CDataInputStream is(d,16);unsigned char output[4]={0};is.StreamPos=0xfffffffeU;is.Read(output,4);CHECK(is.StreamPos==2&&is.SourceChunkBytesRemaining==12&&output[0]==1&&output[3]==4);}
}
int main(){CHECK(sizeof(float)==4&&sizeof(unsigned int)==4);read_boundaries();run(0,999,true,true);run(2,999,true,true);run(2,999,false,true);run(2,0,true,true);run(2,16,true,true);run(2,40,true,true);run(2,48,false,true);run(3,999,true,false);run(0,999,false,false);std::puts("00ae1bc0_TEST PASS");return 0;}