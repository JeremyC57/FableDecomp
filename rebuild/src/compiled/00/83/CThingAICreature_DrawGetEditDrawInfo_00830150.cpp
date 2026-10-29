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
protected:T*PData;unsigned char unobservedBasicStringState[9];
public:const T*PeekData()const{return PData;}
};
class CCharStringData {
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