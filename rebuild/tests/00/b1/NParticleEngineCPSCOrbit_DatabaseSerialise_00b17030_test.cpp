// Self-contained original classes/template bodies; controlled external persistence/string operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define private public
#define protected public
// 00b17030 NParticleEngine::CPSCOrbit::DatabaseSerialise
// Leaf: ?DatabaseSerialise@CPSCOrbit@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Original FableWin direct call signatures independently confirm only Expand/CycleFlag/CycleTime use explicit defaults.
// Retail: Fable.exe00b17030,991bytes,ret8.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/NParticleEngine_CPSCOrbit.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
namespace NParticleEngine {
void CPSCOrbit::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    if (context.IsSaving()) {
        context.Transfer("CentreParam", CentreParam);
    } else {
        CCharString temp;
        context.Transfer("CentreParam", temp);
        CentreParam = temp.GetCRC();
    }
    for (long c_orbit = 0; c_orbit < 3; ++c_orbit) {
        COrbitSettings& orbit=Orbits[c_orbit];
        CCharString postfix = GFIntToCharString(c_orbit);
        orbit.RotateSpeed *= 1.0f / 15.0f;
        orbit.RotateSpeedRandom *= 1.0f / 15.0f;
        context.Transfer("Type" + postfix, orbit.Type);
        context.Transfer("Enabled" + postfix, orbit.Enabled);
        context.Transfer("Radius" + postfix, orbit.Radius);
        context.Transfer((const char*)("Expand" + postfix), orbit.Expand,0.0f);
        context.Transfer((const char*)("CycleFlag" + postfix), orbit.CycleFlag,false);
        context.Transfer((const char*)("CycleTime" + postfix), orbit.CycleTime, 5.0f);
        context.Transfer("SqueezeScale" + postfix, orbit.SqueezeScale);
        context.Transfer("SqueezeAngle" + postfix, orbit.SqueezeAngle);
        context.Transfer("RotateSpeed" + postfix, orbit.RotateSpeed);
        context.Transfer("RotateStart" + postfix, orbit.RotateStart);
        context.Transfer("RotateSpeedRandom" + postfix, orbit.RotateSpeedRandom);
        context.Transfer("RotateStartRandom" + postfix, orbit.RotateStartRandom);
        orbit.RotateSpeed *= 15.0f;
        orbit.RotateSpeedRandom *= 15.0f;
    }
}
}

#include "engine/NParticleEngine_CEmitterDatabaseSerialiseInfo.h"
#include "engine/CSymbolMap.h"
#undef private
#undef protected
using namespace NParticleEngine;


CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
NParticleEngine::CPSCBase::~CPSCBase(){}
namespace NParticleEngine {template<class K,class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback(){} }
namespace NParticleEngine {template<class K,class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&,V,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long){abort();}}
NParticleEngine::CPSCOrbit::COrbitState::COrbitState(){}
NParticleEngine::CPSCOrbit::CPSCOrbit():CentreParam(0),LifeTimer(0),LifeTimeSecs(0){}
NParticleEngine::CPSCOrbit::~CPSCOrbit(){}
CSymbolMap::CSymbolMap():CheckForDuplicateSymbols(false){}CSymbolMap::~CSymbolMap(){}
CDataStream::~CDataStream(){}CDataOutputStream::~CDataOutputStream(){}
CDataInputStream::CDataInputStream():StreamPos(0),StreamSize(0),CurrentSourcePtr(0),SourceChunkStreamPos(0),SourceChunkBytesRemaining(0){}
class FixtureOutput:public CDataOutputStream {public:void Write(const void*,long){abort();}void SetPosition(unsigned long){abort();}unsigned long GetPosition()const{abort();}void Close(){abort();}};
template<class T>void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext&,CCharString&){abort();}
template<class T>void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext&,CStringParser&){abort();}

void NParticleEngine::CPSCBase::OnCreate(){abort();}
void NParticleEngine::CPSCBase::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&){abort();}
void NParticleEngine::CPSCBase::Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,bool&){abort();}
void NParticleEngine::CPSCBase::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&,const unsigned long&){abort();}
void NParticleEngine::CPSCBase::TransferUpdateParametersToDialog(const CSymbolMap&){abort();}
void NParticleEngine::CPSCBase::UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,C3DVector,C3DVector,float){abort();}
void NParticleEngine::CPSCBase::PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&){abort();}
void NParticleEngine::CPSCBase::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&){abort();}
bool NParticleEngine::CPSCBase::AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&){abort();}
bool NParticleEngine::CPSCBase::GetParticleLifeSecs(float&){abort();}
bool NParticleEngine::CPSCBase::GetNumberOfParticles(float&,const float){abort();}
bool NParticleEngine::CPSCBase::GetDecalLifeSecs(float&){abort();}
bool NParticleEngine::CPSCBase::GetNumberOfDecals(float&,const float){abort();}
void NParticleEngine::CPSCBase::EnterTimelineEnd(){abort();}
long NParticleEngine::CPSCBase::GetParticleCount(){abort();}
long NParticleEngine::CPSCBase::GetDecalCount(){abort();}
bool NParticleEngine::CPSCBase::Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&){abort();}
void NParticleEngine::CPSCBase::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&){abort();}
bool NParticleEngine::CPSCBase::GetUserPoint(C3DVector&) const{abort();}
float NParticleEngine::CPSCBase::GetApproximateSplineLength(){abort();}
CCharString NParticleEngine::CPSCOrbit::GetClassTypeName() const{abort();}
EPSCType NParticleEngine::CPSCOrbit::GetComponentType() const{abort();}
EPSCSubType NParticleEngine::CPSCOrbit::GetComponentSubType() const{abort();}
CCharString NParticleEngine::CPSCOrbit::GetGUIName() const{abort();}
CPSCBase* NParticleEngine::CPSCOrbit::DatabaseClone() const{abort();}
void NParticleEngine::CPSCOrbit::InterpolateTo(const CPSCBase&,const float){abort();}
void NParticleEngine::CPSCOrbit::DatabaseCopy(const CPSCBase&){abort();}
void NParticleEngine::CPSCOrbit::ReadBinary(CDataInputStream&){abort();}
void NParticleEngine::CPSCOrbit::WriteBinary(CDataOutputStream&) const{abort();}
void NParticleEngine::CPSCOrbit::CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,const unsigned long&){abort();}
bool NParticleEngine::CPSCOrbit::GetUpdateParam(C3DVector&,unsigned long,float,bool,const CParticleUpdateParameters<unsigned long,unsigned long>&,unsigned long){abort();}
void NParticleEngine::CPSCOrbit::StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&){abort();}
void NParticleEngine::CPSCOrbit::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float){abort();}
void CDataInputStream::Close(){abort();}
void CDataInputStream::SetPosition(unsigned long){abort();}
void CDataInputStream::ReadCompressed(void*,long){abort();}
void CDataInputStream::ReadZlibCompressed(void*,long){abort();}
CCharString CDataInputStream::ReadNullTerminatedString(){abort();}
CWideString CDataInputStream::ReadNullTerminatedWString(){abort();}
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&){abort();}
bool CDataInputStream::NeedsBufferedRead(long){abort();}
void CDataInputStream::InternalReadUnbuffered(unsigned char*,long){abort();}
void CMemoryDataInputStream::ReadCompressed(void*,long){abort();}
CWideString CMemoryDataInputStream::ReadNullTerminatedWString(){abort();}
static int StringObjects,DataBlocks;
static void ReleaseData(CCharStringData*p){if(p&&--p->NoRefs==0){delete[]p->Data.PData;delete p;--DataBlocks;}}
CCharString::CCharString():PStringData(0){++StringObjects;}
CCharString::CCharString(const char*text,long length):PStringData(0){++StringObjects;if(length<0)length=(long)strlen(text);if(length){PStringData=new CCharStringData;++DataBlocks;PStringData->NoRefs=1;PStringData->Data.PData=new char[length+1];memcpy(PStringData->Data.PData,text,length);PStringData->Data.PData[length]=0;PStringData->Data.StringLength=length;PStringData->Data.DataLength=length+1;PStringData->Data.UseFastExtend=false;}}
CCharString::CCharString(const CCharString&other):PStringData(other.PStringData){++StringObjects;if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){ReleaseData(PStringData);--StringObjects;}
const CCharString& CCharString::operator=(const CCharString&other){if(other.PStringData)++other.PStringData->NoRefs;ReleaseData(PStringData);PStringData=other.PStringData;return*this;}
CCharString::operator const char*()const{return GetAsCharArray();}
const char CCharString::NullChar=0;
CCharString __fastcall GFIntToCharString(long value){assert(value>=0&&value<3);char s[2]={(char)('0'+value),0};return CCharString(s);}
CCharString __fastcall operator+(const char*left,const CCharString&right){char text[128];assert(strlen(left)+strlen(right.GetAsCharArray())<sizeof(text));strcpy(text,left);strcat(text,right.GetAsCharArray());return CCharString(text);}
static const char*Keys[] = {"Type","Enabled","Radius","Expand","CycleFlag","CycleTime","SqueezeScale","SqueezeAngle","RotateSpeed","RotateStart","RotateSpeedRandom","RotateStartRandom"};
static CPSCOrbit*ExpectedOrbit;static CPersistContext*ExpectedContext;static CMemoryDataInputStream*ExpectedInput;static CDataOutputStream*ExpectedOutput;
static int Step,Operations,CrcCalls,Mode,Seed,MissingMask;static bool Defaults;static const char*LoadedName;
static unsigned long InitialCentre;static CPSCOrbit::COrbitSettings Original[3];
static void*Field(int step){if(step==0)return &ExpectedOrbit->CentreParam;int i=(step-1)/12,k=(step-1)%12;CPSCOrbit::COrbitSettings&o=ExpectedOrbit->Orbits[i];switch(k){case 0:return&o.Type;case 1:return&o.Enabled;case 2:return&o.Radius;case 3:return&o.Expand;case 4:return&o.CycleFlag;case 5:return&o.CycleTime;case 6:return&o.SqueezeScale;case 7:return&o.SqueezeAngle;case 8:return&o.RotateSpeed;case 9:return&o.RotateStart;case 10:return&o.RotateSpeedRandom;default:return&o.RotateStartRandom;}}
static int Kind(int step){if(step==0)return Mode==1||Mode==3?0:4;int k=(step-1)%12;return k==0?1:k==1||k==4?2:3;}
static float InitialFloat(int step){int i=(step-1)/12,k=(step-1)%12;const CPSCOrbit::COrbitSettings&o=Original[i];switch(k){case 2:return o.Radius;case 3:return o.Expand;case 5:return o.CycleTime;case 6:return o.SqueezeScale;case 7:return o.SqueezeAngle;case 8:return o.RotateSpeed/15;case 9:return o.RotateStart;case 10:return o.RotateSpeedRandom/15;default:return o.RotateStartRandom;}}
static float InputFloat(int step){int k=(step-1)%12;return k==8||k==10?(float)(1U<<((Seed+step)%4)):(float)(Seed+step)+.25f;}
static long InputLong(int step){return 1000+Seed*100+step;}
static bool InputBool(int step){return((Seed+step)&1)!=0;}
static bool Missing(int step){return step>0&&(MissingMask&(1<<((step-1)%12)))!=0;}
static void Operation(){assert(ExpectedContext->Mode==Mode&&Operations==0&&Step>=1&&Step<=37);++Operations;}
static void CheckPre(int step){if(step==0)return;int i=(step-1)/12,k=(step-1)%12;bool use_default=Mode==0&&Defaults;switch(Kind(step)){case 1:assert(*(long*)Field(step)==(use_default?0:Original[i].Type));break;case 2:assert(*(bool*)Field(step)==(use_default?false:k==1?Original[i].Enabled:Original[i].CycleFlag));break;case 3:assert(*(float*)Field(step)==(use_default?(k==5?5.0f:0.0f):InitialFloat(step)));break;}}
void CPersistContext::CheckCRC(const char*name){assert(this==ExpectedContext);if(Step)assert(Operations==(Mode==4?0:1));assert(Step<37);char key[64];if(Step==0)strcpy(key,"CentreParam");else{int i=(Step-1)/12,k=(Step-1)%12;sprintf(key,"%s%d",Keys[k],i);}assert(strcmp(name,key)==0);++Step;Operations=0;}
static void TextOperation(CPersistContext*self,CPersistContext_TransferableComponentGenericType&item,bool loading){assert(self==ExpectedContext&&Mode==(loading?0:1));Operation();int step=Step-1;switch(Kind(step)){case 0:{CPersistContext_TransferableComponent<unsigned long>&w=static_cast<CPersistContext_TransferableComponent<unsigned long>&>(item);assert(w.Object==Field(step)&&*w.Object==InitialCentre);break;}case 1:{CPersistContext_TransferableComponent<long>&w=static_cast<CPersistContext_TransferableComponent<long>&>(item);assert(w.Object==Field(step));CheckPre(step);if(loading&&!Missing(step))*w.Object=InputLong(step);break;}case 2:{CPersistContext_TransferableComponent<bool>&w=static_cast<CPersistContext_TransferableComponent<bool>&>(item);assert(w.Object==Field(step));CheckPre(step);if(loading&&!Missing(step))*w.Object=InputBool(step);break;}case 3:{CPersistContext_TransferableComponent<float>&w=static_cast<CPersistContext_TransferableComponent<float>&>(item);assert(w.Object==Field(step));CheckPre(step);if(loading&&!Missing(step))*w.Object=InputFloat(step);break;}default:{CPersistContext_TransferableComponent<CCharString>&w=static_cast<CPersistContext_TransferableComponent<CCharString>&>(item);assert(loading&&w.Object->PStringData==0);*w.Object=CCharString(LoadedName);break;}}}
void CPersistContext::TransferObjectSaveText(const char*,CPersistContext_TransferableComponentGenericType&i){TextOperation(this,i,false);}
void CPersistContext::TransferObjectLoadText(const char*,CPersistContext_TransferableComponentGenericType&i){TextOperation(this,i,true);}
unsigned long __fastcall CCRC::Calc(unsigned long seed,const void*data,unsigned long length){assert(seed==0&&!(Mode==1||Mode==3)&&Step==1&&CrcCalls==0);++CrcCalls;const char*expected=Mode==4?"":LoadedName;assert(length==strlen(expected)&&memcmp(data,expected,length)==0);if(length==0){assert(data==&CCharString::NullChar);return 0;}return 0x12345000UL+(unsigned long)strlen(expected);}
template<>void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream&is,unsigned long&value){assert(&is==ExpectedInput);value=is.ReadULONG();}
template<>void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream&is,long&value){assert(&is==ExpectedInput);value=is.ReadSLONG();}
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*dst,long count){assert(this==ExpectedInput&&Mode==2);Operation();int step=Step-1;switch(Kind(step)){case 1:{assert(count==sizeof(long));long v=InputLong(step);memcpy(dst,&v,count);break;}case 2:{assert(count==1);unsigned char v=InputBool(step)?1:0;memcpy(dst,&v,1);break;}case 3:{assert(count==sizeof(float));float v=InputFloat(step);memcpy(dst,&v,count);break;}default:abort();}StreamPos+=count;}
CCharString CMemoryDataInputStream::ReadNullTerminatedString(){assert(this==ExpectedInput&&Mode==2&&Kind(Step-1)==4);Operation();return CCharString(LoadedName);}
void CDataOutputStream::WriteSLONG(long value){assert(this==ExpectedOutput&&Mode==3);Operation();int s=Step-1;assert(Kind(s)==0?value==(long)InitialCentre:Kind(s)==1&&value==*(long*)Field(s));}
void CDataOutputStream::WriteEBOOL(bool value){assert(this==ExpectedOutput&&Mode==3&&Kind(Step-1)==2);Operation();assert(value==*(bool*)Field(Step-1));}
void CDataOutputStream::WriteFloat(float value){assert(this==ExpectedOutput&&Mode==3&&Kind(Step-1)==3);Operation();assert(value==InitialFloat(Step-1));}
void CDataOutputStream::WriteNullTerminatedString(const CCharString&){abort();}
static void VectorEquals(const C3DVector&v,float x,float y,float z){assert(v.X==x&&v.Y==y&&v.Z==z);}
int main(){
 CPSCOrbit orbit;CPersistContext context;CMemoryDataInputStream input;FixtureOutput output;CSymbolMap symbols;CEmitterDatabaseSerialiseInfo info(symbols,symbols);ExpectedOrbit=&orbit;ExpectedContext=&context;ExpectedInput=&input;ExpectedOutput=&output;int cases=0;
 const char*names[]={"","OrbitCentre","0"};
 for(Mode=0;Mode<5;++Mode)for(int defaults=0;defaults<2;++defaults)for(int text=0;text<3;++text)for(Seed=0;Seed<8;++Seed)for(int missing=0;missing<14;++missing){
  Defaults=defaults!=0;LoadedName=names[text];MissingMask=missing==0?0:missing==13?4095:1<<(missing-1);InitialCentre=orbit.CentreParam=0x7000+Seed;
  context.PDefinitionManager=0;context.PSymbolMap=0;context.PParser=0;context.ErrorCallback=0;context.PStringToClean=0;context.UseDefaults=Defaults;context.UseCRCCheck=true;context.SingleTransfer=false;context.Finished=false;context.Mode=(CPersistContext::EMode)Mode;context.Binary=Mode==2||Mode==3;context.PStringToSaveTo=0;context.PLoadStream=&input;context.PSaveStream=&output;
  for(int i=0;i<3;++i){CPSCOrbit::COrbitSettings&o=orbit.Orbits[i];o.Type=Seed*10+i;o.Enabled=((Seed+i)&1)!=0;o.CycleFlag=((Seed+i)&2)!=0;o.Radius=Seed+i+1.25f;o.Expand=Seed+i+2.5f;o.CycleTime=Seed+i+3.75f;o.SqueezeScale=Seed+i+4.25f;o.SqueezeAngle=Seed+i+5.5f;o.RotateSpeed=15.0f*(1U<<((Seed+i)%4));o.RotateStart=Seed+i+6.75f;o.RotateSpeedRandom=15.0f*(1U<<((Seed+i+1)%4));o.RotateStartRandom=Seed+i+7.25f;Original[i]=o;orbit.OrbitStates[i].NextAngle=101+i;orbit.OrbitStates[i].RotateSpeedRandomValue=201+i;orbit.OrbitStates[i].FromPosition=C3DVector(i+1,i+2,i+3);orbit.OrbitStates[i].ToPosition=C3DVector(i+4,i+5,i+6);orbit.OrbitStates[i].NextPosition=C3DVector(i+7,i+8,i+9);orbit.OrbitStates[i].FromTangent=C3DVector(i+10,i+11,i+12);orbit.OrbitStates[i].ToTangent=C3DVector(i+13,i+14,i+15);}
  orbit.LifeTimer=991;orbit.LifeTimeSecs=29.5f;orbit.ParticleSystem=0;orbit.UpdateStarted=true;orbit.Initialised=false;Step=Operations=CrcCalls=0;int strings=StringObjects,blocks=DataBlocks;CPSCBase*base=&orbit;base->DatabaseSerialise(context,info);
  assert(Step==37&&Operations==(Mode==4?0:1)&&CrcCalls==((Mode==1||Mode==3)?0:1));assert(StringObjects==strings&&DataBlocks==blocks);assert(orbit.CentreParam==(Mode==1||Mode==3?InitialCentre:Mode==4||!*LoadedName?0:0x12345000UL+strlen(LoadedName)));
  for(int step=1;step<37;++step){int i=(step-1)/12,k=(step-1)%12;bool assigned=Mode==2||(Mode==0&&!Missing(step));bool cleared=Mode==4||(Mode==0&&Missing(step)&&Defaults);switch(Kind(step)){case 1:assert(*(long*)Field(step)==(assigned?InputLong(step):cleared?0:Original[i].Type));break;case 2:assert(*(bool*)Field(step)==(assigned?InputBool(step):cleared?false:k==1?Original[i].Enabled:Original[i].CycleFlag));break;default:{float expected=assigned?InputFloat(step):cleared?(k==5?5.0f:0.0f):InitialFloat(step);if(k==8||k==10)expected*=15;assert(*(float*)Field(step)==expected);break;}}}
  assert(orbit.LifeTimer==991&&orbit.LifeTimeSecs==29.5f&&orbit.ParticleSystem==0&&orbit.UpdateStarted&&!orbit.Initialised&&&info.GraphicSymbols==&symbols&&&info.MeshSymbols==&symbols);for(int i=0;i<3;++i){assert(orbit.OrbitStates[i].NextAngle==101+i&&orbit.OrbitStates[i].RotateSpeedRandomValue==201+i);VectorEquals(orbit.OrbitStates[i].FromPosition,(float)i+1,(float)i+2,(float)i+3);VectorEquals(orbit.OrbitStates[i].ToPosition,(float)i+4,(float)i+5,(float)i+6);VectorEquals(orbit.OrbitStates[i].NextPosition,(float)i+7,(float)i+8,(float)i+9);VectorEquals(orbit.OrbitStates[i].FromTangent,(float)i+10,(float)i+11,(float)i+12);VectorEquals(orbit.OrbitStates[i].ToTangent,(float)i+13,(float)i+14,(float)i+15);}assert(context.Mode==Mode&&context.UseDefaults==Defaults&&context.UseCRCCheck&&!context.SingleTransfer&&!context.Finished&&context.PParser==0&&context.PSymbolMap==0&&context.PDefinitionManager==0&&context.PLoadStream==&input&&context.PSaveStream==&output);
  ++cases;
 }
 assert(cases==3360);puts("00b17030_TEST PASS");return 0;
}