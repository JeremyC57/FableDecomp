// CThingObject::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- build the editor name and own editor colour after physical base info succeeds.
// Leaf: ?DrawGetEditDrawInfo@CThingObject@@UBE_NMPAVCEditEngineDrawInfo@@@Z
// Identity: proven; RTTI own vtable0x01264DCC slot25 and both PDB names virtual const DrawGetEditDrawInfo(float,CEditEngineDrawInfo*); own data ScriptName+0x74,DefGlobalIndex+0x8C and literal separator.
// Layout: included generated Physical/GameObject headers flatten the polymorphic base chain and omit named base fields; CThing header is quarantined. Local minimal named member view uses proved retail offsets, and actual RTTI inheritance with matching class keys. Draw-info/string classes have no header.
#define CThingPhysical CThingPhysical_HeaderLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObject_HeaderLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#ifndef _MSC_VER
#define __fastcall
#endif
extern const char g_EmptyString[];
class CCharStringData { public: char* Data; };
class CCharString { public: CCharStringData* PStringData; CCharString(const char*,long=-1); ~CCharString(); CCharString& operator=(const CCharString&); operator const char*() const { if(!PStringData) return g_EmptyString; return PStringData->Data; } };
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
class CDefString { public: long TablePos; };
class CDefinitionManager { public: CDefString GetDefNameFromGlobalIndex(long) const; };
class CGameDefinitionManager:public CDefinitionManager {};
class CDefStringTable { public: CCharString GetString(long) const; }; extern CDefStringTable g_DefStringTable;
class CRGBColour { public: unsigned char B,G,R,A; void Set(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; } };
class C3DVector { public: float X,Y,Z; };
class CEditEngineDrawInfo { public: C3DVector Pos; float Radius; CCharString Fullname; CRGBColour Colour; long PlayerOwner; float EffectRadius; bool UsesEffectRadius; float AngleXY; };
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseObject:public CBaseClass {}; class CClass:public CBaseObject {};
class CThing:public CClass { public: unsigned char _unmodeled04[0x70]; CCharString ScriptName; unsigned char _unmodeled78[0x14]; unsigned short DefGlobalIndex; const CGameDefinitionManager& GetDefinitionManager() const; };
class CThingPhysical:public CThing { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
class CThingGameObject:public CThingPhysical {};
class CThingObject:public CThingGameObject { public: virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const; };
bool CThingObject::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const {
 if(CThingPhysical::DrawGetEditDrawInfo(time,info)) {
  { const CCharString definitionName=g_DefStringTable.GetString(GetDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex).TablePos); info->Fullname=CCharString("")+definitionName+" - "+(const char*)ScriptName; }
  info->Colour.Set(255,255,0,255); return true;
 } return false;
}
