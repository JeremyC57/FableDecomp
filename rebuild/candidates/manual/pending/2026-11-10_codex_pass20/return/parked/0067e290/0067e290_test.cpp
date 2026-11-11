#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <functional>
static void Check(bool b){if(!b)abort();}
enum EEngineGraphicType {ENGINE_GRAPHIC_NONE=0,ENGINE_GRAPHIC_MESH=1,ENGINE_GRAPHIC_SPRITE=2};
class CEngineGraphic {public:unsigned char Type;int Untouched;};
class CDegradableInfo {public:float HealthPercentage;CEngineGraphic EngineGraphic;long SmashParticleEmitter;bool BlocksNavigation;};
class CPersistContext;
class CDegradableDef {
public:
 class CIsHealth1GTHealth2:public std::binary_function<CDegradableInfo,CDegradableInfo,bool>{public:bool operator()(const CDegradableInfo&a,const CDegradableInfo&b)const{return a.HealthPercentage>b.HealthPercentage;}};
 class CSetEngineGraphicType:public std::unary_function<CDegradableInfo,void>{EEngineGraphicType GraphicType;public:CSetEngineGraphicType(EEngineGraphicType t):GraphicType(t){}void operator()(CDegradableInfo&i)const{i.EngineGraphic.Type=(unsigned char)GraphicType;}};
 bool Degradable;EEngineGraphicType GraphicType;bool InitiallyBlocksNavigation;std::vector<CDegradableInfo> Degradations;
 void Transfer(CPersistContext&);
};
struct CMemoryDataInputStream {int Index;};
struct CDataOutputStream {int Index;void WriteSLONG(long);void WriteEBOOL(bool);};
static int Ops[32],Values[32],N;
static void Log(int op,int value){Ops[N]=op;Values[N++]=value;Check(N<32);}
template<class T>class CPersistTraits {public:void TransferBinaryIn(CMemoryDataInputStream& s,T& value){Log(10+s.Index,s.Index);value=(T)2;}void TransferBinaryOut(CDataOutputStream& s,T value){s.WriteSLONG((long)value);}};
template<>class CPersistTraits<bool>{public:void TransferBinaryIn(CMemoryDataInputStream&s,bool&value){Log(10+s.Index,s.Index);value=s.Index==0;}void TransferBinaryOut(CDataOutputStream&s,const bool&value){s.WriteEBOOL(value);}};
void CDataOutputStream::WriteSLONG(long value){Log(20+Index,(int)value);}
void CDataOutputStream::WriteEBOOL(bool value){Log(20+Index,value?1:0);}
static CDegradableDef* Current;
static int NextMode;
class CPersistContext {
public:
 enum EMode{MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4};
 int Mode,Modes[4],Count;CMemoryDataInputStream* PLoadStream;CDataOutputStream* PSaveStream;
 void CheckCRC(const char* text){Check(text[0]==0);Log(Count,0);PLoadStream->Index=PSaveStream->Index=Count;Mode=Modes[Count++];}
 bool IsParsingText()const{return Mode==MODE_LOAD_TEXT;}
 template<class T>void Transfer(T& value){
  CheckCRC("");CPersistTraits<T> traits;
  switch(Mode){case MODE_LOAD_BINARY:traits.TransferBinaryIn(*PLoadStream,value);break;case MODE_SAVE_BINARY:traits.TransferBinaryOut(*PSaveStream,value);break;}
 }
 template<class T>void TransferVector(std::vector<T>& value){
  CheckCRC("");
  switch(Mode){case MODE_LOAD_BINARY:GFSerialiseVectorBinaryIn(*PLoadStream,value);break;default:GFSerialiseVectorBinaryOut(*PSaveStream,value);break;}
 }
};
static CPersistContext* Active;
template<class T>void GFSerialiseVectorBinaryIn(CMemoryDataInputStream&,std::vector<T>& value){Log(30,0);for(unsigned int i=0;i<value.size();++i)value[i].HealthPercentage+=2.0f;Current->GraphicType=ENGINE_GRAPHIC_SPRITE;Active->Mode=NextMode;}
template<class T>void GFSerialiseVectorBinaryOut(CDataOutputStream&,std::vector<T>&){Log(31,0);Active->Mode=NextMode;}
void CDegradableDef::Transfer(CPersistContext& context)
{
    context.Transfer(Degradable);
    context.Transfer(GraphicType);
    context.Transfer(InitiallyBlocksNavigation);
    context.TransferVector(Degradations);
    if (context.IsParsingText()) {
        std::sort(Degradations.begin(), Degradations.end(), CIsHealth1GTHealth2());
        std::for_each(Degradations.begin(), Degradations.end(), CSetEngineGraphicType(GraphicType));
    }
}
int main(){unsigned int cases=0;int vmodes[]={0,2,3};
 for(int seed=0;seed<64;++seed)for(int vm=0;vm<3;++vm)for(int post=0;post<2;++post)for(int length=0;length<4;++length)for(int type=0;type<3;++type)for(int bits=0;bits<4;++bits){
  CDegradableDef object;object.Degradable=(bits&1)!=0;object.InitiallyBlocksNavigation=(bits&2)!=0;object.GraphicType=(EEngineGraphicType)type;
  for(int i=0;i<length;++i){CDegradableInfo v;v.HealthPercentage=(float)((i*5+seed)%7)/4.0f;v.EngineGraphic.Type=(unsigned char)(80+i);v.EngineGraphic.Untouched=100+i;v.SmashParticleEmitter=i;v.BlocksNavigation=(i&1)!=0;object.Degradations.push_back(v);}
  std::vector<CDegradableInfo> original=object.Degradations;
  CMemoryDataInputStream in;CDataOutputStream out;CPersistContext context;context.PLoadStream=&in;context.PSaveStream=&out;context.Count=0;context.Mode=4;
  int n=seed;for(int j=0;j<3;++j){context.Modes[j]=n%4;n/=4;}context.Modes[3]=vmodes[vm];Current=&object;Active=&context;NextMode=post?2:0;N=0;
  object.Transfer(context);
  int expectedOps[16],expectedValues[16],size=0;
  for(int k=0;k<4;++k){expectedOps[size]=k;expectedValues[size++]=0;if(k<3){int mode=context.Modes[k];if(mode==2){expectedOps[size]=10+k;expectedValues[size++]=k;}if(mode==3){expectedOps[size]=20+k;expectedValues[size++]=k==0?(bits&1?1:0):k==1?type:(bits&2?1:0);}}else{expectedOps[size]=vmodes[vm]==2?30:31;expectedValues[size++]=0;}}
  Check(size==N);for(int j=0;j<size;++j)Check(Ops[j]==expectedOps[j]&&Values[j]==expectedValues[j]);
  Check(object.Degradable==(context.Modes[0]==2?true:(bits&1)!=0));Check(object.InitiallyBlocksNavigation==(context.Modes[2]==2?false:(bits&2)!=0));
  int finalType=vmodes[vm]==2||context.Modes[1]==2?2:type;Check(object.GraphicType==finalType&&context.Count==4);
  Check(object.Degradations.size()==original.size());
  for(unsigned int j=0;j<object.Degradations.size();++j){const CDegradableInfo& v=object.Degradations[j];int id=(int)v.SmashParticleEmitter;Check(id>=0&&id<length);Check(v.EngineGraphic.Untouched==100+id&&v.BlocksNavigation==((id&1)!=0));Check(v.HealthPercentage==original[id].HealthPercentage+(vmodes[vm]==2?2.0f:0.0f));if(!post){Check(v.EngineGraphic.Type==finalType);if(j)Check(object.Degradations[j-1].HealthPercentage>=v.HealthPercentage);}else Check(v.EngineGraphic.Type==80+id&&id==(int)j);}
  ++cases;
 }
 Check(cases==18432);puts("0067e290_TEST PASS");return 0;
}
