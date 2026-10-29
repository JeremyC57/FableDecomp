#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
int main();
// 00830150 CThingAICreature::DrawGetEditDrawInfo -- editor label and original object colour
// Leaf: ?DrawGetEditDrawInfo@CThingAICreature@@UBE_NMPAVCEditEngineDrawInfo@@@Z
// Flags:
#define CBaseObject CBaseObject_Layout
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass CClass_Layout
#include "engine/CClass.h"
#undef CClass
#define CResourceList CResourceList_Layout
#include "engine/CResourceList.h"
#undef CResourceList
#define CDefinitionManager CDefinitionManager_Layout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CThingPhysical CThingPhysical_Layout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObject_Layout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBase_Layout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreature_Layout
#include "engine/CThingAICreature.h"
#undef CThingAICreature
class CBaseClass { public:virtual ~CBaseClass(); };
class CBaseClassNonCopyable:public CBaseClass { protected:virtual ~CBaseClassNonCopyable(); };
class CBaseObject:public CBaseClass {
private:unsigned char unobservedObjectPointer[4];
protected:virtual ~CBaseObject();
};
class CFailedAllocationHandler { public:virtual ~CFailedAllocationHandler(); };
class CResourceList:public CFailedAllocationHandler {
public:unsigned char unobservedResourceListState[sizeof(CResourceList_Layout)-sizeof(CFailedAllocationHandler)];
};
class CDefString;
class CDefinitionManager:public CResourceList {
public:
    unsigned char unobservedDefinitionManagerState[sizeof(CDefinitionManager_Layout)-sizeof(CResourceList)];
    CDefString GetDefNameFromGlobalIndex(long)const;
};
class CGameDefinitionManager:public CDefinitionManager { };
extern const char CUnprovenGlobal_0129aaf4[];
#pragma pack(push,1)
template<class T>class CBasicString {
friend class CCharString;
protected:T*PData;unsigned char unobservedBasicStringState[9];
public:const T*PeekData()const{return PData;}
};
class CCharStringData {
friend class CCharString;
public:CBasicString<char>Data;protected:long NoRefs;
};
class CCharString {
protected:
    CCharStringData*PStringData;
public:
    CCharString();
    CCharString(const char*,long=-1);
    CCharString(const CCharString&);
    ~CCharString();
    const CCharString& operator=(const CCharString&);
    const char* GetAsCharArray()const { return PStringData==0?CUnprovenGlobal_0129aaf4:PStringData->Data.PeekData(); }
};
// Layout: receiver-only table projection, no full object-size claim.
class CDefStringTable { public:CCharString GetString(long)const; };
extern CDefStringTable GDefStringTable;
class CDefString {
private:
    long TablePos;
public:
    operator CCharString()const { return GDefStringTable.GetString(TablePos); }
};
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() { }
    C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z) { }
};
class CRGBColour {
public:
    unsigned char B,G,R,A;
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) {
        R=r;A=a;G=g;B=b;
    }
};
class CEditEngineDrawInfo {
public:
    C3DVector Pos;float Radius;CCharString Fullname;CRGBColour Colour;
    // Layout: remaining original draw-record fields occupy16 unread bytes.
    unsigned char unobservedDrawTail[16];
};
class CClass:public CBaseObject {
public:unsigned char unobservedClassState[sizeof(CClass_Layout)-sizeof(CBaseObject)];
};
class CThing:public CClass {
friend int ::main();
protected:
    unsigned char unobservedThingPrefix[0x74-sizeof(CClass)];
    CCharString ScriptName;
    unsigned char unobservedThingLinks[0x8c-0x78];
    unsigned short DefGlobalIndex;
    unsigned char unobservedThingTail[0x98-0x8e];
public:
    const CGameDefinitionManager& PeekDefinitionManager()const;
};
class CThingPhysical:public CThing {
public:
    unsigned char unobservedPhysicalState[sizeof(CThingPhysical_Layout)-sizeof(CThing)];
    virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*)const;
};
class CThingGameObject:public CThingPhysical {
public:unsigned char unobservedGameObjectState[sizeof(CThingGameObject_Layout)-sizeof(CThingPhysical)];
};
class CThingBuilding:public CThingGameObject {
public:virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*)const;
};
class CThingCreatureBase:public CThingGameObject {
public:unsigned char unobservedCreatureState[sizeof(CThingCreatureBase_Layout)-sizeof(CThingGameObject)];
};
class CThingAICreature:public CThingCreatureBase {
public:
    unsigned char unobservedAICreatureState[sizeof(CThingAICreature_Layout)-sizeof(CThingCreatureBase)];
    virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*)const;
};
#pragma pack(pop)
bool CThingAICreature::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info)const {
    if(CThingPhysical::DrawGetEditDrawInfo(time,info)) {
        info->Fullname=CCharString("")+PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex)+" - "+ScriptName.GetAsCharArray();
        info->Colour.Initialise(255,0,0,255);
        return true;
    }
    return false;
}

const char CUnprovenGlobal_0129aaf4[]="";
CDefStringTable GDefStringTable;
static int fixture_live_strings=0;
static int fixture_peek_calls=0;
static int fixture_name_calls=0;
static int fixture_table_calls=0;
static int fixture_base_calls=0;
static bool fixture_base_result=true;
static float fixture_last_time=0;
static CEditEngineDrawInfo* fixture_last_info=0;
static CGameDefinitionManager fixture_manager;
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CBaseObject::~CBaseObject() { }
CFailedAllocationHandler::~CFailedAllocationHandler() { }
static long fixture_length(const char* text) { long n=0;while(text[n])++n;return n; }
static bool fixture_same(const char* a,const char* b) { while(*a && *a==*b){++a;++b;}return *a==*b; }
CCharString::CCharString():PStringData(0) { }
CCharString::CCharString(const char* text,long length):PStringData(0) {
    if(length<0)length=fixture_length(text);
    PStringData=new CCharStringData;
    PStringData->Data.PData=new char[length+1];
    for(long i=0;i<length;++i)PStringData->Data.PData[i]=text[i];
    PStringData->Data.PData[length]=0;
    PStringData->NoRefs=1;++fixture_live_strings;
}
CCharString::CCharString(const CCharString& other):PStringData(other.PStringData) { if(PStringData)++PStringData->NoRefs; }
CCharString::~CCharString() {
    if(PStringData && --PStringData->NoRefs==0){delete[] PStringData->Data.PData;delete PStringData;--fixture_live_strings;}
}
const CCharString& CCharString::operator=(const CCharString& other) {
    if(PStringData==other.PStringData)return *this;
    if(other.PStringData)++other.PStringData->NoRefs;
    if(PStringData && --PStringData->NoRefs==0){delete[]PStringData->Data.PData;delete PStringData;--fixture_live_strings;}
    PStringData=other.PStringData;return *this;
}
static CCharString fixture_join(const char* left,const char* right) {
    char buffer[256];long n=0;
    while(*left)buffer[n++]=*left++;
    while(*right)buffer[n++]=*right++;
    buffer[n]=0;
    return CCharString(buffer,n);
}
CCharString __fastcall operator+(const CCharString& left,const CCharString& right) { return fixture_join(left.GetAsCharArray(),right.GetAsCharArray()); }
CCharString __fastcall operator+(const CCharString& left,const char* right) { return fixture_join(left.GetAsCharArray(),right); }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index)const { ++fixture_name_calls;CDefString result;memcpy(&result,&index,sizeof(index));return result; }
CCharString CDefStringTable::GetString(long index)const { if(this!=&GDefStringTable)return CCharString("BAD_TABLE");++fixture_table_calls;return CCharString(index==1?"House":(index==2?"Creature":"")); }
const CGameDefinitionManager& CThing::PeekDefinitionManager()const { ++fixture_peek_calls;return fixture_manager; }
bool CThingPhysical::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info)const { ++fixture_base_calls;fixture_last_time=time;fixture_last_info=info;return fixture_base_result; }
struct FixtureGuardedDraw { unsigned int before;CEditEngineDrawInfo info;unsigned int after; };
static bool fixture_guards(const FixtureGuardedDraw& value) {
    if(value.before!=0x12345678U || value.after!=0x76543210U)return false;
    for(unsigned int i=0;i<16;++i)if(value.info.unobservedDrawTail[i]!=0xa5)return false;
    return value.info.Pos.X==1 && value.info.Pos.Y==2 && value.info.Pos.Z==3 && value.info.Radius==4;
}
int main() {
    if(sizeof(CBasicString<char>)!=13 || sizeof(CCharStringData)!=17 || sizeof(CEditEngineDrawInfo)!=40)return 1;
    {
        CThingAICreature item;
        FixtureGuardedDraw output;
        output.before=0x12345678U;output.after=0x76543210U;
        for(unsigned int i=0;i<16;++i)output.info.unobservedDrawTail[i]=0xa5;
        output.info.Pos.X=1;output.info.Pos.Y=2;output.info.Pos.Z=3;output.info.Radius=4;
        item.DefGlobalIndex=1;item.ScriptName=CCharString("Alpha");
        output.info.Fullname=CCharString("seed");output.info.Colour.B=7;output.info.Colour.G=8;output.info.Colour.R=9;output.info.Colour.A=10;
        fixture_base_result=false;
        if(item.DrawGetEditDrawInfo(2.5f,&output.info) || !fixture_same(output.info.Fullname.GetAsCharArray(),"seed") || output.info.Colour.B!=7 || fixture_peek_calls || fixture_name_calls || fixture_table_calls)return 2;
        if(fixture_base_calls!=1 || fixture_last_time!=2.5f || fixture_last_info!=&output.info || !fixture_guards(output))return 3;
        fixture_base_result=true;
        if(!item.DrawGetEditDrawInfo(3.5f,&output.info) || !fixture_same(output.info.Fullname.GetAsCharArray(),"House - Alpha"))return 4;
        if(output.info.Colour.R!=255 || output.info.Colour.G!=0 || output.info.Colour.B!=0 || output.info.Colour.A!=255 || !fixture_guards(output))return 5;
        if(fixture_peek_calls!=1 || fixture_name_calls!=1 || fixture_table_calls!=1)return 6;
        item.ScriptName=CCharString();
        if(!item.DrawGetEditDrawInfo(4.5f,&output.info) || !fixture_same(output.info.Fullname.GetAsCharArray(),"House - ") || !fixture_guards(output))return 7;
        item.DefGlobalIndex=3;item.ScriptName=CCharString("Beta");
        if(!item.DrawGetEditDrawInfo(5.5f,&output.info) || !fixture_same(output.info.Fullname.GetAsCharArray()," - Beta") || !fixture_guards(output))return 8;
        if(fixture_live_strings!=2)return 9;
    }
    if(fixture_live_strings!=0)return 10;
    puts("00830150_TEST PASS");return 0;
}