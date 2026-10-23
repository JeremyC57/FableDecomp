// 008af850 CombatDialogueManager::PlayBattleCry
// Flags: /O2 /Oy /W3
// Leaf: PlayBattleCry
// Layout: CDefStringTable buffer+12 and BufferedData start+8/finish+12 from9d49b0/995e70; CASoundBank mapper+4 froma01920; CCharString release99e9b0.
// Layout: manager last-frame+0x20,CThingCreatureBase flags+0x20/interfaces+0x44/def+0x70,def battlecry+0xb0; shared def header and retail offsets.
#include "engine/CDefClassBase.h"
#include "engine/CThingCreatureBase.h"
namespace ExternalRetry_008af850 {

struct CCreatureDef:CDefClassBase {unsigned char Pad[0xb0-0x25];unsigned int BattleCry;bool GetSubDef(CCreatureDef**);};
struct DestroyDef {virtual void v00();virtual void DeleteThis();};
__forceinline CCreatureDef* Acquire(CCreatureDef* source){CCreatureDef* result=0;source->GetSubDef(&result);return result;}
struct DefGuard {CDefClassBase* Data;__forceinline DefGuard(CCreatureDef* p):Data(p){} __forceinline ~DefGuard(){CDefClassBase* p=Data;unsigned int& count=*(unsigned int*)p->_base_0x04;if(--count==0)reinterpret_cast<DestroyDef*>(p)->DeleteThis();}};
#pragma pack(push,1)
struct CCharStringData {char* Text;unsigned int Length;unsigned int Capacity;unsigned char Flag;unsigned int Refs;};
#pragma pack(pop)
extern unsigned int LiveStrings;
struct CCharString {CCharStringData* Data;CCharString();CCharString(const CCharString&);CCharString(const char*,int);CCharStringData*Allocate(const char*,int);void Release();__declspec(noinline) ~CCharString();};
__declspec(noinline) CCharString::CCharString():Data(0){++LiveStrings;}
__declspec(noinline) CCharString::CCharString(const char*p,int n):Data(0){++LiveStrings;if(Data)Release();if(p&&*p)Data=Allocate(p,n);}
struct BufferedData {unsigned int Header[2];char* Start;char* Finish;char* End;const char* GetData();};
struct CDefStringTable {unsigned int Tree[3];BufferedData Buffer;__declspec(noinline) CCharString GetString(unsigned int);};extern CDefStringTable Strings;
unsigned __fastcall Calc(unsigned,void*,unsigned);
struct SymbolEntry {unsigned Hash,Index;};
struct SymbolMap {unsigned Header;SymbolEntry*First,*Last,*End;
 __declspec(noinline) bool Lookup(const CCharString&name,unsigned*out){unsigned crc=Calc(0,name.Data?(void*)name.Data->Text:(void*)"",name.Data?name.Data->Length:0);SymbolEntry*first=First;int n=Last-First;while(n>0){int half=n>>1;SymbolEntry*mid=first+half;if(mid->Hash<crc){first=mid+1;n-=half+1;}else n=half;}if(first==Last||crc<first->Hash)return false;*out=first->Index;return true;}
};
struct CASoundBank {unsigned int Header;SymbolMap* Mapper;__declspec(noinline) long GetSoundIndexFromSymbol(const CCharString&)const;};

__declspec(noinline) CCharString CDefStringTable::GetString(unsigned int id){if(id==0xffffffff)return CCharString();return CCharString(Buffer.GetData()+id+4,-1);}
__declspec(noinline) long CASoundBank::GetSoundIndexFromSymbol(const CCharString& key)const{unsigned int index=0;if(Mapper)Mapper->Lookup(key,&index);return index;}
//00485815 GetSubDef<CCreatureDef>: actual virtual lookup and counted manager result; OUT address never escapes.
struct CSubDefInfo {unsigned GlobalIndex;};
struct SubDefMethods {virtual void Slot0();virtual void Slot1();virtual void Slot2();virtual void Slot3();virtual void Slot4();virtual void Slot5();virtual void Slot6();virtual void Slot7();virtual void Slot8();virtual void Slot9();virtual void Slot10();virtual void Slot11();virtual void Slot12();virtual void Slot13();virtual CSubDefInfo* GetSubDefInfo(const CCharString&);};
__forceinline unsigned& DefReferences(CDefClassBase*p){return *(unsigned*)p->_base_0x04;}
__forceinline void ReleaseDefinition(CDefClassBase*p){if(p&&--DefReferences(p)==0)reinterpret_cast<DestroyDef*>(p)->DeleteThis();}
struct DefLookup {CCreatureDef*Data;DefLookup(CCreatureDef*p):Data(p){if(p)++DefReferences(p);}DefLookup(const DefLookup&d):Data(d.Data){if(Data)++DefReferences(Data);}~DefLookup(){ReleaseDefinition(Data);}};
struct DefinitionManagerMethods {DefLookup GetPDefFromGlobalIndex(unsigned);};
__declspec(noinline) bool CCreatureDef::GetSubDef(CCreatureDef**out){CSubDefInfo*info;{CCharString name("CCreatureDef",-1);info=reinterpret_cast<SubDefMethods*>(this)->GetSubDefInfo(name);}if(!info)return false;CCreatureDef*value;{DefLookup found=reinterpret_cast<DefinitionManagerMethods*>(PDefManager)->GetPDefFromGlobalIndex(info->GlobalIndex);value=found.Data;if(value)++DefReferences(value);}CCreatureDef*previous=*out;if(previous!=value){if(previous){ReleaseDefinition(previous);*out=0;}*out=value;if(value)++DefReferences(value);}ReleaseDefinition(value);return true;}
struct CMainGameComponent {unsigned int Pad[4];CASoundBank* PSampleBank;__forceinline CASoundBank&GetSampleBank()const{return *PSampleBank;}};extern CMainGameComponent* Sounds;
struct CSoundPair {long FirstSound,LastSound;};
struct CSoundInit {float Volume,Pitch,Pan;long NoPlays,PlaybackType;unsigned long GainFaderBits,PitchFaderBits;long OverrideID;__forceinline CSoundInit(){Volume=1.0f;Pitch=1.0f;Pan=0.0f;NoPlays=1;GainFaderBits=0;PitchFaderBits=0;OverrideID=-1;PlaybackType=1;}};
struct CTCSoundPlayer {unsigned long PlayAttachedSample(const CSoundPair&,const CSoundInit&,bool,bool,bool);};
struct Entry {int Type;CTCSoundPlayer* Value;};struct CVectorMap {Entry* Start;Entry* Finish;Entry* End;__declspec(noinline) Entry* LowerBound(const int* key){int length=(int)(Finish-Start);Entry* first=Start;while(length>0){int half=length>>1;Entry* middle=first+half;if(middle->Type<*key){first=middle+1;length-=half+1;}else length=half;}return first;}};
struct CThingCreatureBase: ::CThingCreatureBase {__forceinline unsigned&Flags(){return *(unsigned*)_pad_0x20;}__forceinline CVectorMap&Interfaces(){return *(CVectorMap*)_pad_0x44;}__forceinline CCreatureDef*&Definition(){return *(CCreatureDef**)_pad_0x70;}};
__forceinline CTCSoundPlayer* FindSoundInterface(CThingCreatureBase* thing){int type=16;CVectorMap& interfaces=thing->Interfaces();Entry* p=interfaces.LowerBound(&type);return (p==interfaces.Finish||type<p->Type?interfaces.Finish:p)->Value;}
unsigned int GetWorldFrame();
struct CombatDialogueManager {unsigned char Pad[0x20];unsigned int LastBattleCryFrame;bool IsSpeechAllowed(const CThingCreatureBase&)const;bool PlayBattleCry(CThingCreatureBase&);};
bool CombatDialogueManager::PlayBattleCry(CThingCreatureBase& creature){
 CThingCreatureBase* thing=&creature;
 if(IsSpeechAllowed(creature)){
  DefGuard def(Acquire(thing->Definition()));
  unsigned int index;{CCharString symbol=Strings.GetString(static_cast<CCreatureDef*>(def.Data)->BattleCry);index=Sounds->GetSampleBank().GetSoundIndexFromSymbol(symbol);}
  if(index && (thing->Flags()&0x10000)){
   CThingCreatureBase* receiver=thing;CTCSoundPlayer* sound=FindSoundInterface(receiver);
   CSoundInit params;CSoundPair pair;pair.FirstSound=index;pair.LastSound=index;
   if(sound->PlayAttachedSample(pair,params,1,0,0)){LastBattleCryFrame=GetWorldFrame();return true;}
  }
 }
 return false;
}

}

#include <cstdio>
#include <cassert>
#include <new>
#include <cstring>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace ExternalRetry_008af850 {

unsigned int LiveStrings;static int blocks,allowed=1,plays,playResult=1,deletes,defGets,lookups,frameCalls;static CCreatureDef speech;static unsigned int expectedIndex=23;
CCharString::CCharString(const CCharString& x):Data(x.Data){++LiveStrings;if(Data)++Data->Refs;}
CCharStringData*CCharString::Allocate(const char*p,int length){if(length<0)length=(int)std::strlen(p);CCharStringData*v=new CCharStringData;v->Text=new char[length+1];std::memcpy(v->Text,p,length);v->Text[length]=0;v->Length=length;v->Capacity=length+1;v->Flag=0;v->Refs=1;++blocks;return v;}
void CCharString::Release(){if(Data&&--Data->Refs==0){delete[] Data->Text;delete Data;--blocks;}Data=0;}
const char* BufferedData::GetData(){return Finish==Start?(const char*)&Start:Start;}
CDefStringTable Strings;CMainGameComponent* Sounds;
unsigned __fastcall Calc(unsigned seed,void*data,unsigned length){assert(!seed);++lookups;return length==3&&!std::memcmp(data,"cry",3)?1:2;}
DefLookup DefinitionManagerMethods::GetPDefFromGlobalIndex(unsigned id){assert(id==123);++defGets;return DefLookup(&speech);}
struct SpeechDispatch {virtual void Slot0(){}virtual void DeleteThis(){++deletes;}virtual void Slot2(){}virtual void Slot3(){}virtual void Slot4(){}virtual void Slot5(){}virtual void Slot6(){}virtual void Slot7(){}virtual void Slot8(){}virtual void Slot9(){}virtual void Slot10(){}virtual void Slot11(){}virtual void Slot12(){}virtual void Slot13(){}virtual CSubDefInfo* GetSubDefInfo(const CCharString&n){assert(n.Data&&!std::strcmp(n.Data->Text,"CCreatureDef"));static CSubDefInfo i={123};return &i;}};
void DestroyDef::v00(){}void DestroyDef::DeleteThis(){++deletes;}
bool CombatDialogueManager::IsSpeechAllowed(const CThingCreatureBase& p)const{return allowed&&const_cast<CThingCreatureBase&>(p).Definition()!=0;}
unsigned long CTCSoundPlayer::PlayAttachedSample(const CSoundPair& pair,const CSoundInit& p,bool a,bool b,bool c){if(pair.FirstSound!=expectedIndex||pair.LastSound!=expectedIndex||p.Volume!=1.0f||p.Pitch!=1.0f||p.Pan!=0.0f||p.NoPlays!=1||p.PlaybackType!=1||p.GainFaderBits||p.PitchFaderBits||p.OverrideID!=-1||a!=1||b||c)return 0;++plays;return playResult;}
unsigned int GetWorldFrame(){++frameCalls;return 77;}
}
int main(){using namespace ExternalRetry_008af850;if(sizeof(CCharStringData)!=17)return 1;{CCharString nullString((const char*)0,-1);CCharString original("shared",-1);CCharString* copies[300];for(int i=0;i<300;++i)copies[i]=new CCharString(original);if(original.Data->Refs!=301)return 2;for(int j=0;j<46;++j)delete copies[j];if(original.Data->Refs!=255)return 3;for(int k=46;k<300;++k)delete copies[k];if(original.Data->Refs!=1||nullString.Data)return 4;}if(blocks||LiveStrings)return 5;char buffer[16]={0,0,0,0,'c','r','y',0,'m','i','s','s',0,0,0,0};std::memset(&Strings,0,sizeof Strings);Strings.Buffer.Start=buffer;Strings.Buffer.Finish=buffer+13;Strings.Buffer.End=buffer+16;{CCharString empty=Strings.GetString(0xffffffff);CCharString cry=Strings.GetString(0);CCharString miss=Strings.GetString(4);if(empty.Data||!cry.Data||std::strcmp(cry.Data->Text,"cry")||!miss.Data||std::strcmp(miss.Data->Text,"miss"))return 6;CASoundBank nullBank;std::memset(&nullBank,0,sizeof nullBank);if(nullBank.GetSoundIndexFromSymbol(cry))return 7;}if(blocks||LiveStrings)return 8;SpeechDispatch dispatch;std::memset(&speech,0,sizeof speech);speech.__vftable=*(void**)&dispatch;speech.PDefManager=reinterpret_cast<CDefinitionManager*>(&speech);speech.BattleCry=0;SymbolMap symbols;SymbolEntry symbolEntry={1,expectedIndex};symbols.Header=0;symbols.First=&symbolEntry;symbols.Last=symbols.End=&symbolEntry+1;CASoundBank bank;std::memset(&bank,0,sizeof bank);bank.Mapper=&symbols;CMainGameComponent manager;std::memset(&manager,0,sizeof manager);manager.PSampleBank=&bank;Sounds=&manager;ExternalRetry_008af850::CThingCreatureBase thing;std::memset(&thing,0,sizeof thing);new(thing._pad_0x20)unsigned(0);new(thing._pad_0x44)CVectorMap;typedef CCreatureDef*DefPtr;new(thing._pad_0x70)DefPtr(0);thing.Flags()=0x10000;thing.Definition()=&speech;CTCSoundPlayer component;Entry entries[3]={{3,&component},{16,&component},{0,0}};thing.Interfaces().Start=entries;thing.Interfaces().Finish=entries+2;thing.Interfaces().End=entries+3;CombatDialogueManager dialogue;std::memset(&dialogue,0,sizeof dialogue);if(!dialogue.PlayBattleCry(thing)||plays!=1||dialogue.LastBattleCryFrame!=77||deletes!=1||frameCalls!=1)return 9;allowed=0;if(dialogue.PlayBattleCry(thing)||defGets!=1)return 10;allowed=1;thing.Flags()=0;if(dialogue.PlayBattleCry(thing)||plays!=1||deletes!=2)return 11;thing.Flags()=0x10000;playResult=0;if(dialogue.PlayBattleCry(thing)||plays!=2||deletes!=3||frameCalls!=1)return 12;playResult=1;speech.BattleCry=4;if(dialogue.PlayBattleCry(thing)||plays!=2||deletes!=4)return 13;speech.BattleCry=0xffffffff;if(dialogue.PlayBattleCry(thing)||plays!=2||deletes!=5)return 14;speech.BattleCry=0;bank.Mapper=0;if(dialogue.PlayBattleCry(thing)||plays!=2||deletes!=6)return 15;if(blocks||LiveStrings||*(unsigned int*)((unsigned char*)&speech+4)||defGets!=6||lookups!=5)return 16;std::printf("008af850_TEST PASS\n");return 0;}

namespace ExternalRetry_008af850 { __declspec(noinline) CCharString::~CCharString(){Release();--LiveStrings;} }