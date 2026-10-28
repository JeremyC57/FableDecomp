#include <stdio.h>
#include <string.h>
#include <new>
class CThing; class CThingCreatureBase;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer():CBaseIntelligentPointer() {} };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0) {} };
class CTCDivineWrath { public: struct PerCreatureInfo { CIntelligentPointer<CThingCreatureBase> ActualCreature; long FramesToGetHit; CIntelligentPointer<CThing> Effect; CEnginePrimitiveThemeChannelDesc DefaultChannel; PerCreatureInfo(); }; };
CTCDivineWrath::PerCreatureInfo::PerCreatureInfo() {}
static void* first; static void* second; static int calls,bad;
CBaseIntelligentPointer::CBaseIntelligentPointer() { if(calls==0) first=this; else if(calls==1) second=this; else bad=1; ++calls; PData=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
int main() { union Storage { void* align[16]; unsigned char data[128]; } storage; memset(storage.data,0x5A,sizeof(storage.data)); CTCDivineWrath::PerCreatureInfo* object=new(storage.data) CTCDivineWrath::PerCreatureInfo;
 if(bad || calls!=2 || first!=&object->ActualCreature || second!=&object->Effect || object->ActualCreature.PData || object->Effect.PData || object->DefaultChannel.ChannelIndex0 || object->DefaultChannel.ChannelIndex1 || object->DefaultChannel.ChannelBlendFactor || object->DefaultChannel.Pad!=0x5A) goto fail;
 { unsigned char* p=(unsigned char*)&object->FramesToGetHit; for(unsigned i=0;i<sizeof(object->FramesToGetHit);++i) if(p[i]!=0x5A) goto fail; } object->~PerCreatureInfo(); puts("0078e300_TEST PASS"); return 0; fail: puts("0078e300_TEST FAIL"); return 1; }