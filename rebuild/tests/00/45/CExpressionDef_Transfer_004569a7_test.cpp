#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CExpressionDef::Transfer(CPersistContext&) -- transfers expression definition fields.
// Retail004569a7 length306. Quarantined donor CExpressionDef first member+0x40;
// native+0x3c, all subsequent members -4. Native layout below uses donor names.
#pragma optimize("s",on)
#include <stddef.h>
class CDefString;
enum EOpinionDeedType { };
class CPersistContext { public:
 template <class T> void Transfer(T& value); //0045228f
 void TransferDefIndex(int&); //004522f4
 void TransferGameEvent(int&); //00456ad9
 void TransferExpressionType(int&); //00456c21
};
template <> void CPersistContext::Transfer<CDefString>(CDefString& value);
template <> void CPersistContext::Transfer<float>(float& value);
template <> void CPersistContext::Transfer<long>(long& value);
template <> void CPersistContext::Transfer<bool>(bool& value);
template <> void CPersistContext::Transfer<EOpinionDeedType>(EOpinionDeedType& value);
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value);
class CExpressionDef { public:
 unsigned char DefinitionBase[0x3c];
 int Animation_TablePos, AnimationInto_TablePos, AnimationLoop_TablePos, AnimationOutOf_TablePos;
 int AnimationToOther_TablePos, AnimationToOtherInto_TablePos, AnimationToOtherLoop_TablePos, AnimationToOtherOutOf_TablePos;
 float SpriteDuration; int SpriteGraphic, SpriteOverrideText_TablePos, SoundCriteria_TablePos, LoopSoundCriteria_TablePos;
 int DummyObject_Val, GameEventType, ScriptName_TablePos;
 bool ScriptLoadResources, DeactivateScriptAtEnd, PerformAfterScript;
 int OpinionDeedType;
 bool Extendable, SheatheWeapons, ReplaceHeldItemWhenDone;
 unsigned int TextTag;
 int ExpressionType;
 void Transfer(CPersistContext& c);
};
typedef char NativeExpressionSize[(sizeof(CExpressionDef)==0x90)?1:-1];
typedef char NativeExpressionOffset[(offsetof(CExpressionDef,ExpressionType)==0x8c)?1:-1];
void CExpressionDef::Transfer(CPersistContext& c) {
 c.Transfer(*(CDefString*)&(Animation_TablePos));
 c.Transfer(*(CDefString*)&(AnimationInto_TablePos));
 c.Transfer(*(CDefString*)&(AnimationLoop_TablePos));
 c.Transfer(*(CDefString*)&(AnimationOutOf_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOther_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherInto_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherLoop_TablePos));
 c.Transfer(*(CDefString*)&(AnimationToOtherOutOf_TablePos));
 c.Transfer(SpriteDuration);
 c.Transfer(*(long*)&(SpriteGraphic));
 c.Transfer(*(CDefString*)&(SpriteOverrideText_TablePos));
 c.Transfer(*(CDefString*)&(SoundCriteria_TablePos));
 c.Transfer(*(CDefString*)&(LoopSoundCriteria_TablePos));
 c.TransferDefIndex(DummyObject_Val);
 c.TransferGameEvent(GameEventType);
 c.Transfer(*(CDefString*)&(ScriptName_TablePos));
 c.Transfer(ScriptLoadResources);
 c.Transfer(DeactivateScriptAtEnd);
 c.Transfer(PerformAfterScript);
 c.Transfer(*(EOpinionDeedType*)&(OpinionDeedType));
 c.Transfer(Extendable);
 c.Transfer(SheatheWeapons);
 c.Transfer(ReplaceHeldItemWhenDone);
 c.Transfer(*(unsigned long*)&(TextTag));
 c.TransferExpressionType(ExpressionType);
}



#include <stdio.h>
static void* expected[25];static int types[25],n,bad,writing;
static void Check(void*p,int type){if(n>=25||expected[n]!=p||types[n]!=type)bad=1;++n;}
template <> void CPersistContext::Transfer<CDefString>(CDefString& value_)
{
    int& v = *(int*)&value_; (void)&v;Check(&v,0);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
template <> void CPersistContext::Transfer<float>(float& value_)
{
    float& v = *(float*)&value_; (void)&v;Check(&v,1);if(writing)v=0.25f+n;else if(v!=(0.25f+n))bad=1;}
template <> void CPersistContext::Transfer<long>(long& value_)
{
    int& v = *(int*)&value_; (void)&v;Check(&v,2);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
void CPersistContext::TransferDefIndex(int&v){Check(&v,3);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
void CPersistContext::TransferGameEvent(int&v){Check(&v,4);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
template <> void CPersistContext::Transfer<bool>(bool& value_)
{
    bool& v = *(bool*)&value_; (void)&v;Check(&v,5);if(writing)v=((n%2)!=0);else if(v!=(((n%2)!=0)))bad=1;}
template <> void CPersistContext::Transfer<EOpinionDeedType>(EOpinionDeedType& value_)
{
    int& v = *(int*)&value_; (void)&v;Check(&v,6);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value_)
{
    unsigned int& v = *(unsigned int*)&value_; (void)&v;Check(&v,7);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
void CPersistContext::TransferExpressionType(int&v){Check(&v,8);if(writing)v=100+n;else if(v!=(100+n))bad=1;}
int main(){CExpressionDef x;CPersistContext c;
expected[0]=&x.Animation_TablePos;types[0]=0;
expected[1]=&x.AnimationInto_TablePos;types[1]=0;
expected[2]=&x.AnimationLoop_TablePos;types[2]=0;
expected[3]=&x.AnimationOutOf_TablePos;types[3]=0;
expected[4]=&x.AnimationToOther_TablePos;types[4]=0;
expected[5]=&x.AnimationToOtherInto_TablePos;types[5]=0;
expected[6]=&x.AnimationToOtherLoop_TablePos;types[6]=0;
expected[7]=&x.AnimationToOtherOutOf_TablePos;types[7]=0;
expected[8]=&x.SpriteDuration;types[8]=1;
expected[9]=&x.SpriteGraphic;types[9]=2;
expected[10]=&x.SpriteOverrideText_TablePos;types[10]=0;
expected[11]=&x.SoundCriteria_TablePos;types[11]=0;
expected[12]=&x.LoopSoundCriteria_TablePos;types[12]=0;
expected[13]=&x.DummyObject_Val;types[13]=3;
expected[14]=&x.GameEventType;types[14]=4;
expected[15]=&x.ScriptName_TablePos;types[15]=0;
expected[16]=&x.ScriptLoadResources;types[16]=5;
expected[17]=&x.DeactivateScriptAtEnd;types[17]=5;
expected[18]=&x.PerformAfterScript;types[18]=5;
expected[19]=&x.OpinionDeedType;types[19]=6;
expected[20]=&x.Extendable;types[20]=5;
expected[21]=&x.SheatheWeapons;types[21]=5;
expected[22]=&x.ReplaceHeldItemWhenDone;types[22]=5;
expected[23]=&x.TextTag;types[23]=7;
expected[24]=&x.ExpressionType;types[24]=8;
writing=1;x.Transfer(c);if(n!=25)bad=1;n=0;writing=0;x.Transfer(c);if(n!=25)bad=1;printf("004569a7_TEST %s\n",bad?"FAIL":"PASS");return bad;}