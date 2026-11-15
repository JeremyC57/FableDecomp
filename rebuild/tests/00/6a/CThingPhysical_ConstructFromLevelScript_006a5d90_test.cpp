#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#include <strings.h>
#define _stricmp strcasecmp
#define __declspec(x)
#endif
#define private public
#define protected public
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
#define FABLE_RETAIL_C3DANGLES_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C3DAngles.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCParamsPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#undef private
#undef protected
// Controlled external interfaces preserve original signatures. Host fields have native host widths.
class CBaseObject {public:long References;CBaseObject():References(0){}virtual ~CBaseObject(){assert(References==0);}};
class CThingBaseDef {public:float Health;bool Damageable;};
class CWorldMap {public:virtual ~CWorldMap(){}virtual C2DCoordI GetMapPos(long)const;};
class CTCPhysicsBase {public:virtual ~CTCPhysicsBase(){}virtual void ConstructFromParams(const CTCParamsBase&);};
class CThing:public CBaseObject {
public:
 CThingBaseDef* PBaseDef;CTCPhysicsBase* PhysicsTC;long CurrentMapNumber;bool Drawable;
 CThing():PBaseDef(0),PhysicsTC(0),CurrentMapNumber(0),Drawable(false){}
 virtual bool ConstructFromLevelScript(const char*);
 CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}long GetCurrentMapNumber()const{return CurrentMapNumber;}
 const CThingBaseDef&PeekBaseDef()const{return *PBaseDef;}CWorldMap&GetWorldMap();
};
class CThingPhysical:public CThing {
public:
 float Scale;CIntelligentPointer<CThingPhysical>PLastCreator,POriginalCreator;float MaxHealth,Health;long CanComeBetweenCameraAndObservedThing;
 unsigned char Damageable:1,Killable:1,RespondToHits:1,EvadeAllHits:1,NegateAllHits:1,AbleToBeEngagedInCombat:1;
 bool ConstructFromLevelScript(const char*);
 CThingPhysical():Scale(0),MaxHealth(0),Health(0),CanComeBetweenCameraAndObservedThing(0){Damageable=Killable=RespondToHits=EvadeAllHits=NegateAllHits=AbleToBeEngagedInCombat=0;}
};

CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs)
    : Position(position), Rhs(rhs)
{
}

bool CThingPhysical::ConstructFromLevelScript(const char* script)
{
    if (CThing::ConstructFromLevelScript(script)) {
    Scale = 1.0f;
    Health = MaxHealth = PeekBaseDef().Health;
    CanComeBetweenCameraAndObservedThing = -1;
    CStringParser parser(script);
    bool positionRead = false;
    C3DVector position(0.0f, 0.0f, 0.0f);
    float angleXY = 0.0f;
    float angleYZ = 0.0f;
    float angleXZ = 0.0f;
    while (parser.NextItemExists()) {
        CCharString token = parser.ReadAsString();
        if (token == "AngleXY") { angleXY = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "AngleYZ") { angleYZ = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "AngleXZ") { angleXZ = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosX") { position.X = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosY") { position.Y = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "PosZ") { position.Z = parser.ReadAsFloat(); positionRead = true; }
        else if (token == "ObjectScale") Scale = parser.ReadAsFloat();
        else if (token == "CanComeBetweenCameraAndHero") CanComeBetweenCameraAndObservedThing = parser.ReadAsInteger();
        else if (token == "Health") Health = parser.ReadAsFloat();
        else if (token == "EndThing") break;
        parser.SkipPastString(";");
    }
    if (GetPhysicsTC() && positionRead) {
        position.X += GetWorldMap().GetMapPos(GetCurrentMapNumber()).X;
        position.Y += GetWorldMap().GetMapPos(GetCurrentMapNumber()).Y;
        GetPhysicsTC()->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet(C3DAngles(angleXY, angleYZ, angleXZ))));
    }
    PLastCreator = CIntelligentPointer<CThingPhysical>(0);
    POriginalCreator = CIntelligentPointer<CThingPhysical>(0);
    Drawable = true;
    Killable = true;
    RespondToHits = true;
    EvadeAllHits = false;
    NegateAllHits = false;
    Damageable = PeekBaseDef().Damageable;
    AbleToBeEngagedInCombat = true;
    return true;
    }
    return false;
}

static long g_strings,g_parserLive,g_baseCalls,g_parserCalls,g_setCalls,g_physicsCalls,g_mapCalls,g_rhsCalls,g_paramsLive;
static bool g_baseOK;static unsigned g_pos,g_count,g_commentCount;static const char*g_tokens[128];
static C3DVector g_physPos;static float g_angles[3];static C3DVector g_physUp,g_physForward;
const char CCharString::NullChar=0;
CCharString::CCharString():PStringData(0){++g_strings;}
CCharString::CCharString(const char*t,long length):PStringData(0){++g_strings;if(t){unsigned long n=length<0?(unsigned long)strlen(t):(unsigned long)length;PStringData=new CCharStringData;PStringData->Data.PData=new char[n+1];memcpy(PStringData->Data.PData,t,n);PStringData->Data.PData[n]=0;PStringData->Data.StringLength=n;PStringData->Data.DataLength=n;PStringData->Data.UseFastExtend=false;PStringData->NoRefs=1;}}
CCharString::CCharString(const CCharString&o):PStringData(o.PStringData){++g_strings;if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&!--PStringData->NoRefs){delete[]PStringData->Data.PData;delete PStringData;}--g_strings;}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){++g_paramsLive;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){--g_paramsLive;}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){++g_setCalls;if(PData)--reinterpret_cast<CBaseObject*>(PData)->References;PData=reinterpret_cast<CBaseObjectPointer*>(p);if(p)++p->References;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(PData)--reinterpret_cast<CBaseObject*>(PData)->References;}
CStringParser::CStringParser(const char*text,void(__fastcall*callback)(const CCharString&),const CCharString&,long offset){assert(!strcmp(text,"script"));++g_parserCalls;++g_parserLive;g_pos=0;String=text;ErrorCallback=callback;BaseLineOffset=offset;memset(&CursorPos,0,sizeof(CursorPos));for(unsigned i=0;i<g_commentCount;++i){CStringParserCommentBoundaries x;x.PStart="/*";x.PEnd="*/";Comments.push_back(x);}}
bool CStringParser::NextItemExists(){return g_pos<g_count;}
CCharString CStringParser::ReadAsString(){assert(g_pos<g_count);return CCharString(g_tokens[g_pos++]);}
float CStringParser::ReadAsFloat(){assert(g_pos<g_count&&g_tokens[g_pos]);return(float)atof(g_tokens[g_pos++]);}
long CStringParser::ReadAsInteger(){assert(g_pos<g_count&&g_tokens[g_pos]);return atol(g_tokens[g_pos++]);}
bool CStringParser::SkipPastString(const CCharString&s){const char*needle=s.GetAsCharArray();while(g_pos<g_count){const char*t=g_tokens[g_pos++];if(t&&!strcmp(t,needle))return true;}return false;}
// CStringParser's actual implicit destructor runs the complete real shared STLport list destructor.
bool CThing::ConstructFromLevelScript(const char*){++g_baseCalls;return g_baseOK;}
static CWorldMap g_map;
CWorldMap&CThing::GetWorldMap(){return g_map;}
C2DCoordI CWorldMap::GetMapPos(long n)const{++g_mapCalls;return C2DCoordI(n*16,n*8-4);}
CRightHandedSet::CRightHandedSet(const C3DAngles&a){++g_rhsCalls;g_angles[0]=a.AngleXY;g_angles[1]=a.AngleYZ;g_angles[2]=a.AngleXZ;Up=C3DVector(a.AngleXY,a.AngleYZ,a.AngleXZ);Forward=C3DVector(a.AngleXZ+1,a.AngleXY+2,a.AngleYZ+4);}
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase&p){++g_physicsCalls;const CTCParamsPhysicsBase&v=static_cast<const CTCParamsPhysicsBase&>(p);g_physPos=v.Position;g_physUp=v.Rhs.GetUp();g_physForward=v.Rhs.GetForward();assert(g_paramsLive==1);}
static void add(const char*t){assert(g_count<128);g_tokens[g_count++]=t;}
static void entry(const char*k,const char*v){add(k);add(v);add(";");}
static unsigned flags(const CThingPhysical&t){return t.Damageable|(t.Killable<<1)|(t.RespondToHits<<2)|(t.EvadeAllHits<<3)|(t.NegateAllHits<<4)|(t.AbleToBeEngagedInCombat<<5);}
static void setflags(CThingPhysical&t,unsigned x){t.Damageable=x&1;t.Killable=(x>>1)&1;t.RespondToHits=(x>>2)&1;t.EvadeAllHits=(x>>3)&1;t.NegateAllHits=(x>>4)&1;t.AbleToBeEngagedInCombat=(x>>5)&1;}
static bool same(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){
 unsigned cases=0;
 CTCPhysicsBase physics;
 for(unsigned mask=0;mask<64;++mask)for(unsigned phys=0;phys<2;++phys)for(unsigned base=0;base<2;++base)for(unsigned damage=0;damage<2;++damage)for(unsigned fi=0;fi<4;++fi)for(unsigned mode=0;mode<6;++mode)for(unsigned creator=0;creator<2;++creator)for(unsigned mi=0;mi<4;++mi){
  CThingBaseDef def;def.Health=64.0f+16.0f*mi;def.Damageable=damage!=0;
  CThingPhysical oldCreator,t;t.PBaseDef=&def;t.PhysicsTC=phys?&physics:0;t.CurrentMapNumber=(long)mi-2;t.Scale=-2;t.MaxHealth=-4;t.Health=-8;t.CanComeBetweenCameraAndObservedThing=7;t.Drawable=(fi&1)!=0;unsigned initialFlags=(fi*17)&63;setflags(t,initialFlags);
  if(creator){t.PLastCreator.Set(&oldCreator);t.POriginalCreator.Set(&oldCreator);}assert(oldCreator.References==(creator?2:0));
  g_count=0;g_pos=99;g_commentCount=(mask+mode+fi)%4;g_baseOK=base!=0;g_physicsCalls=g_mapCalls=g_rhsCalls=0;g_baseCalls=g_parserCalls=0;long sets=g_setCalls;float expectedScale=1,expectedHealth=def.Health;long expectedCamera=-1;float a[3]={0,0,0};C3DVector p(0,0,0);bool use=false;
  if(mode==3){add("EndThing");entry("PosX","999");entry("Health","999");}
  else{
   if(mode==4){entry("Unknown","123");add(0);add(";");entry("angleXY","88");}
   static const char*keys[6]={"AngleXY","AngleYZ","AngleXZ","PosX","PosY","PosZ"};static const char*values[6]={"1.25","-2.5","3.75","4.5","-5.25","6.75"};static const float nums[6]={1.25f,-2.5f,3.75f,4.5f,-5.25f,6.75f};
   for(unsigned j=0;j<6;++j)if(mask&(1u<<j)){entry(keys[j],values[j]);if(j<3)a[j]=nums[j];else if(j==3)p.X=nums[j];else if(j==4)p.Y=nums[j];else p.Z=nums[j];use=true;}
   if(mode==1||mode==5){entry("ObjectScale","1.5");entry("Health","25.5");entry("CanComeBetweenCameraAndHero","1");expectedScale=1.5f;expectedHealth=25.5f;expectedCamera=1;}
   if(mode==2){entry("Health","50");entry("ObjectScale","0.5");entry("Health","-8");entry("CanComeBetweenCameraAndHero","2");entry("PosX","-1.25");expectedScale=0.5f;expectedHealth=-8;expectedCamera=2;p.X=-1.25f;use=true;}
   if(mode>=4){add("EndThing");entry("Health","999");}
  }
  bool got=t.CThingPhysical::ConstructFromLevelScript("script");assert(got==g_baseOK&&g_baseCalls==1);assert(g_strings==0&&g_paramsLive==0);assert(g_parserCalls==(base?1:0));
  if(!base){assert(g_pos==99&&t.Scale==-2&&t.Health==-8&&t.MaxHealth==-4&&t.CanComeBetweenCameraAndObservedThing==7&&flags(t)==initialFlags&&t.Drawable==((fi&1)!=0));assert(t.PLastCreator.Get()==(creator?&oldCreator:0)&&t.POriginalCreator.Get()==(creator?&oldCreator:0));assert(g_setCalls==sets);}
  else{assert(t.Scale==expectedScale&&t.Health==expectedHealth&&t.MaxHealth==def.Health&&t.CanComeBetweenCameraAndObservedThing==expectedCamera);assert(flags(t)==(damage?39u:38u)&&t.Drawable);assert(!t.PLastCreator.Get()&&!t.POriginalCreator.Get()&&oldCreator.References==0);assert(g_setCalls-sets==4);}
  bool called=base&&phys&&use;assert(g_physicsCalls==(called?1:0)&&g_rhsCalls==(called?1:0)&&g_mapCalls==(called?2:0));
  if(called){p.X+=t.CurrentMapNumber*16;p.Y+=t.CurrentMapNumber*8-4;assert(same(p,g_physPos));assert(g_angles[0]==a[0]&&g_angles[1]==a[1]&&g_angles[2]==a[2]);assert(same(g_physUp,C3DVector(a[0],a[1],a[2]))&&same(g_physForward,C3DVector(a[2]+1,a[0]+2,a[1]+4)));}
  ++cases;
 }
 assert(cases==98304&&g_strings==0&&g_paramsLive==0);
 unsigned eq=0;const char*strings[]={"","A","a","AngleXY","AngleYZ","AngleXZ","PosX","Health","EndThing","EndThingX","Z","\177","\200","\377"};
 for(unsigned i=0;i<14;++i)for(unsigned j=0;j<14;++j){CCharString x(strings[i]);bool got=x==strings[j];assert(got==(strcmp(strings[i],strings[j])==0));assert((x.PStringData->Data==strings[j])==got);++eq;}
 {CCharString x;assert(x=="");assert(!(x=="Health"));}
 assert(g_strings==0);assert(eq==196);
 printf("006a5d90_TEST PASS\n");return 0;
}