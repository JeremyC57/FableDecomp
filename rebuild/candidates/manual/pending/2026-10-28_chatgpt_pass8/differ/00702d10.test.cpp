#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
extern const char g_EmptyString[];
class CCharStringData { public: char* Data; };
class CCharString { public: CCharStringData* PStringData; CCharString(const char* text="",long=-1); ~CCharString(); CCharString& operator=(const CCharString&); operator const char*() const { if(!PStringData) return g_EmptyString; return PStringData->Data; } };
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
const char g_EmptyString[]=""; CDefStringTable g_DefStringTable; static CGameDefinitionManager manager; static const CThing* expectedThis; static CEditEngineDrawInfo* expectedInfo; static bool baseResult; static int bad,calls; static CCharStringData blocks[64]; static char text[64][128]; static int used;
CCharString::CCharString(const char* value,long length) { if(length<0) length=(long)strlen(value); PStringData=&blocks[used]; PStringData->Data=text[used++]; memcpy(PStringData->Data,value,length); PStringData->Data[length]=0; }
CCharString::~CCharString() {}
CCharString& CCharString::operator=(const CCharString& other) { PStringData=other.PStringData; return *this; }
CCharString __fastcall operator+(const CCharString& a,const CCharString& b) { char buffer[128]; strcpy(buffer,(const char*)a); strcat(buffer,(const char*)b); return CCharString(buffer); }
CCharString __fastcall operator+(const CCharString& a,const char* b) { char buffer[128]; strcpy(buffer,(const char*)a); strcat(buffer,b); return CCharString(buffer); }
bool CThingPhysical::DrawGetEditDrawInfo(float time,CEditEngineDrawInfo* info) const { if(this!=expectedThis || info!=expectedInfo || time!=3.5f || calls++) bad=1; return baseResult; }
const CGameDefinitionManager& CThing::GetDefinitionManager() const { if(this!=expectedThis || calls++!=1) bad=1; return manager; }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) const { if(this!=&manager || index!=319 || calls++!=2) bad=1; CDefString value; value.TablePos=71; return value; }
CCharString CDefStringTable::GetString(long pos) const { if(this!=&g_DefStringTable || pos!=71 || calls++!=3) bad=1; return CCharString("ObjectDef"); }
int main() { CThingObject object; CEditEngineDrawInfo info; expectedThis=&object; expectedInfo=&info; object.DefGlobalIndex=319; object.ScriptName=CCharString("ScriptOne"); baseResult=true; if(!object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=4 || strcmp((const char*)info.Fullname,"ObjectDef - ScriptOne") || info.Colour.B || info.Colour.G!=255 || info.Colour.R!=255 || info.Colour.A!=255) goto fail;
 calls=0; object.ScriptName.PStringData=0; if(!object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=4 || strcmp((const char*)info.Fullname,"ObjectDef - ")) goto fail;
 calls=0; baseResult=false; if(object.DrawGetEditDrawInfo(3.5f,&info) || bad || calls!=1) goto fail;
 puts("00702d10_TEST PASS"); return 0; fail: puts("00702d10_TEST FAIL"); return 1; }
