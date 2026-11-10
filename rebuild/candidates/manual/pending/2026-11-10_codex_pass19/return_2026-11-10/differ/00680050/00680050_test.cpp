#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <new>
static void check(bool ok){if(!ok)std::abort();}
static std::string trace;static int stringLive,animCalls,dummyCalls,staticCalls,animatedCalls,subCalls,defReleases,defDeletes,statsDeletes,blockDeletes,crcCalls;
static bool hasInfo,subSuccess,animResult,dummyResult,sharedDef;static int countMode;
static unsigned long expectedCRC;
struct CCharString{
 const char* text;char tag;bool local;
 CCharString(const char* s,char t):text(s),tag(t),local(false){}
 CCharString(const CCharString& o):text(o.text),tag(o.tag),local(true){trace+=tag;++stringLive;}
 CCharString(const char* s):text(s),tag('N'),local(true){check(std::strcmp(s,"DEFAULT")==0);trace+='N';++stringLive;}
 ~CCharString(){if(local){trace+=char(tag+('a'-'A'));--stringLive;}}
 unsigned long GetCRC()const{trace+='C';++crcCalls;unsigned long crc=0;for(const unsigned char* p=(const unsigned char*)text;*p;++p){crc^=*p;for(int i=0;i<8;++i)crc=((crc&1)?0xedb88320UL:0)^(crc>>1);}return crc;}
};
static const CCharString START_WEAPON_POINT_NAME("start",'S'),END_WEAPON_POINT_NAME("end",'E'),PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME("hold",'H'),ACTION_POINT_01("ABCD",'A');
struct CAnimFilterBase{virtual ~CAnimFilterBase(){}};
struct CAnimFilterNoGroup:CAnimFilterBase{CAnimFilterNoGroup(){trace+='F';}~CAnimFilterNoGroup(){trace+='f';}};
struct CAnimationSet{bool AnimExists(const CCharString& name,const CAnimFilterBase& filter)const{check(std::strcmp(name.text,"DEFAULT")==0&&stringLive==4);(void)filter;trace+='A';++animCalls;return animResult;}};
struct CAppearanceDef{long refs;CAnimationSet Animation;void Release(){++defReleases;check(refs>0);if(--refs==0){trace+='D';++defDeletes;}else trace+='d';}};
static CAppearanceDef appearance;
enum ENullDef{NULL_DEF};
template<class T>struct CDefPointer{T* Object;CDefPointer(ENullDef=NULL_DEF):Object(0){}~CDefPointer(){if(Object){const_cast<CAppearanceDef*>(Object)->Release();Object=0;}}T* operator->()const{return Object;}};
struct C3DMeshStats{bool HasDummyObject(unsigned long crc)const{check(crc==expectedCRC&&stringLive==3);trace+='Q';++dummyCalls;return dummyResult;}};
struct CCPPointerInfo{long refs;const C3DMeshStats* data;};
template<class T>struct CCountedPointer{
 T* Data;CCPPointerInfo* Info;
 CCountedPointer(T* d,CCPPointerInfo* i):Data(d),Info(i){if(Info)++Info->refs;}
 CCountedPointer(const CCountedPointer& o):Data(o.Data),Info(o.Info){if(Info)++Info->refs;}
 ~CCountedPointer(){if(Info&&!--Info->refs){trace+='R';++statsDeletes;check(Info->data==Data);delete Info;++blockDeletes;}Data=0;Info=0;}
 T* operator->()const{return Data;}
};
static C3DMeshStats stats;static CCPPointerInfo* infoBlock;
struct C3DMeshInfo{CCountedPointer<const C3DMeshStats> GetMeshStats()const{trace+='M';return CCountedPointer<const C3DMeshStats>(&stats,infoBlock);}};
static C3DMeshInfo meshInfo;
struct CMeshDataBank{
 const C3DMeshInfo* PeekStaticMesh(unsigned long index)const{check(index==7);trace+='T';++staticCalls;return hasInfo?&meshInfo:0;}
 const C3DMeshInfo* PeekAnimatedMesh(unsigned long index)const{check(index==7);trace+='V';++animatedCalls;return hasInfo?&meshInfo:0;}
};
struct CWorld{CMeshDataBank* bank;const CMeshDataBank* PeekMeshBank()const{return bank;}};
struct CValidateDefinitionInitBase{};struct CValidateDefinitionInit;
struct CEngineGraphic{unsigned long BankIndex;unsigned char Type;};
struct CThingObjectDef{
 CEngineGraphic Graphic;unsigned long Property;
 bool GetSubDef(CDefPointer<const CAppearanceDef>* out)const{check(out->Object==0);trace+='G';++subCalls;if(!subSuccess)return false;++appearance.refs;out->Object=&appearance;return true;}
 void Validate(const CValidateDefinitionInitBase&,const CCharString&)const;
};
void CThingObjectDef::Validate(const CValidateDefinitionInitBase& validate,const CCharString& instantiation_name)const
{
 const CValidateDefinitionInit& validation_class=reinterpret_cast<const CValidateDefinitionInit&>(validate);
 // Layout: donor01fd74a7/01fb5297 prove the reference read at+8 after original base8.
 // The derived complete extent and original member spelling remain unproven.
 const CWorld& world=**reinterpret_cast<const CWorld* const*>(reinterpret_cast<const unsigned char*>(&validation_class)+8);
 CCharString start_weapon_point_name(START_WEAPON_POINT_NAME);
 CCharString end_weapon_point_name(END_WEAPON_POINT_NAME);
 CCharString holdable_object_dummy_name(PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME);
 const CCharString& object_name=instantiation_name;
 const C3DMeshInfo* info=0;
 if(Graphic.Type==5 && Graphic.BankIndex)info=world.PeekMeshBank()->PeekStaticMesh(Graphic.BankIndex);
 else if(Graphic.Type==4 && Graphic.BankIndex)info=world.PeekMeshBank()->PeekAnimatedMesh(Graphic.BankIndex);
 CDefPointer<const CAppearanceDef> pappearance_def(NULL_DEF);
 Graphic.Type==4 && GetSubDef(&pappearance_def) && !pappearance_def->Animation.AnimExists("DEFAULT",CAnimFilterNoGroup());
 if(Property==0x100 || Property==0x200)
  info && !info->GetMeshStats()->HasDummyObject(ACTION_POINT_01.GetCRC());
}

int main(){
 check(sizeof(void*)<=8);expectedCRC=0;const unsigned char text[]={65,66,67,68};
 for(int j=0;j<4;++j){unsigned long v=expectedCRC^text[j];for(int k=0;k<8;++k)v=(v>>1)^((v&1)?0xedb88320UL:0);expectedCRC=v;}
 CMeshDataBank bank;CWorld world;world.bank=&bank;
 union InitStorage{void* alignment;unsigned char bytes[16];}storage;std::memset(storage.bytes,0,16);
 typedef const CWorld* WorldPointer;
 new(storage.bytes+8) WorldPointer(&world);
 CValidateDefinitionInitBase* storedBase=new(storage.bytes) CValidateDefinitionInitBase;
 const CValidateDefinitionInitBase& validation=*storedBase;
 const unsigned char types[]={0,4,5,9};const unsigned long properties[]={0,0x100,0x200,0x300};int cases=0;
 for(int t=0;t<4;++t)for(int ix=0;ix<2;++ix)for(int p=0;p<4;++p)for(int flags=0;flags<32;++flags)for(int mode=0;mode<3;++mode){
  hasInfo=(flags&1)!=0;subSuccess=(flags&2)!=0;animResult=(flags&4)!=0;dummyResult=(flags&8)!=0;sharedDef=(flags&16)!=0;countMode=mode;
  appearance.refs=sharedDef?1:0;infoBlock=mode==0?0:new CCPPointerInfo;if(infoBlock){infoBlock->refs=mode==2?1:0;infoBlock->data=&stats;}
  CThingObjectDef object;object.Graphic.Type=types[t];object.Graphic.BankIndex=ix?7:0;object.Property=properties[p];
  CCharString name("fixture",'I');trace="";stringLive=animCalls=dummyCalls=staticCalls=animatedCalls=subCalls=defReleases=defDeletes=statsDeletes=blockDeletes=crcCalls=0;
  object.Validate(validation,name);
  bool staticRead=t==2&&ix;bool animatedRead=t==1&&ix;bool queried=t==1;bool foundAppearance=queried&&subSuccess;bool foundInfo=(staticRead||animatedRead)&&hasInfo;bool checkedDummy=(p==1||p==2)&&foundInfo;
  check(staticCalls==(staticRead?1:0)&&animatedCalls==(animatedRead?1:0)&&subCalls==(queried?1:0));
  check(animCalls==(foundAppearance?1:0)&&dummyCalls==(checkedDummy?1:0)&&crcCalls==dummyCalls);
  check(defReleases==(foundAppearance?1:0)&&defDeletes==(foundAppearance&&!sharedDef?1:0));
  check(appearance.refs==(sharedDef?1:0));check(statsDeletes==(checkedDummy&&mode==1?1:0)&&blockDeletes==statsDeletes);
  if(infoBlock&&!(checkedDummy&&mode==1)){check(infoBlock->refs==(mode==2?1:0));delete infoBlock;}
  check(stringLive==0&&trace.substr(0,3)=="SEH"&&trace.substr(trace.size()-3)=="hes");
  if(foundAppearance){std::string::size_type at=trace.find('A');check(at!=std::string::npos&&trace.find('N')<at&&trace.find('F')<at&&trace.find('n')>at&&trace.find('f')>at);if(checkedDummy)check(trace.find('n')<trace.find('Q')&&trace.find('f')<trace.find('Q'));}
  if(checkedDummy){check(trace.find('Q')>trace.find('M')&&trace.find('Q')>trace.find('C'));if(mode==1)check(trace.find('R')>trace.find('Q'));}
  if(foundAppearance){std::string::size_type released=trace.find(sharedDef?'d':'D');check(released<trace.size()-3);if(checkedDummy)check(released>trace.find('Q'));}
  check(object.Graphic.Type==types[t]&&object.Graphic.BankIndex==(ix?7:0)&&object.Property==properties[p]);++cases;
 }
 check(cases==3072);std::puts("00680050_TEST PASS");return 0;
}
