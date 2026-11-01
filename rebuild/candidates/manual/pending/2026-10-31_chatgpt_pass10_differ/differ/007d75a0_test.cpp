// Behavior-only fixture. Native target body is pasted unchanged. GNU64 changes
// only the RNG seed pointer to a 32-bit word; native unsigned long is 32-bit.
// Unused def metadata, interfaces and container storage are reduced. This checks
// the target's selection algorithm and ownership boundary, not native layouts.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int cases,draws,copies,cardCtors,cardDtors,defCalls;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
#ifdef _MSC_VER
typedef unsigned long SeedWord;
#else
typedef unsigned int SeedWord;
#endif
static SeedWord worldSeed;
class CDefPointeeBase{public:unsigned long RefCount;CDefPointeeBase():RefCount(1){}virtual~CDefPointeeBase(){}void AddRef(){++RefCount;}long Release(){CHECK(RefCount>0);return(long)--RefCount;}virtual void Delete()const{CHECK(false);}};
enum ENullDef{NULL_DEF};template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer(ENullDef=NULL_DEF):Object(0){}~CDefPointer(){if(Object){CDefPointeeBase*object=Object;if(object->Release()==0)object->Delete();}}T*operator->()const{return static_cast<T*>(Object);}};
namespace std{template<class T>class vector{public:T*First;unsigned int Count;vector():First(0),Count(0){}T*begin(){return First;}T*end(){return First+Count;}const T&operator[](unsigned int n)const{CHECK(n<Count);return First[n];}T&operator[](unsigned int n){CHECK(n<Count);return First[n];}};}
class CCardDef{public:long CardName,CardVal;CCardDef():CardName(0),CardVal(0){++cardCtors;}~CCardDef(){++cardDtors;}CCardDef&operator=(const CCardDef&o){CardName=o.CardName;CardVal=o.CardVal;++copies;return*this;}};
class CTavernGameCardBaseDef:public CDefPointeeBase{public:std::vector<CCardDef>CardPack;};
class CThingObject;class CCard{public:CThingObject*Thing;long Name,Value;};
static CTavernGameCardBaseDef*expectedDef;
class CTCBase{protected:SeedWord*GetPWorldSeed()const{CHECK(++draws<10000);return &worldSeed;}};class CTCTavernGame:public CTCBase{};
class CTCTavernGameCardBase:public CTCTavernGame{public:std::vector<CCard*>Cards;void GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*out){CHECK(!out->Object);out->Object=expectedDef;out->Object->AddRef();++defCalls;}virtual void GetRandomCard(long&,long&,long,long){};};
class CTCTavernGameCardPairs:public CTCTavernGameCardBase{public:virtual void GetRandomCard(long&,long&,long,long);};
long __fastcall GFROR13(unsigned long value){SeedWord v=(SeedWord)value;return(long)(int)((v>>13)|(v<<19));}
#ifdef _MSC_VER
void CTCTavernGameCardPairs::GetRandomCard(long&name,long&value,long dealt,long total){
 CDefPointer<const CTavernGameCardBaseDef>def;GetCardDef(&def);CCardDef card;long half=total/2;
 bool accepted;do{accepted=true;unsigned long*seed=GetPWorldSeed();*seed=*seed*0x24a1+0x24df;*seed=GFROR13(*seed);card=def->CardPack[*seed%52];
 if(dealt>half){long original_card=dealt-half-1;if(Cards[original_card]->Value!=card.CardVal||Cards[original_card]->Name==card.CardName)accepted=false;}
 else{for(CCard**i=Cards.begin();i!=Cards.end();++i){if((*i)->Name==card.CardName||(*i)->Value==card.CardVal){accepted=false;break;}}}
 }while(!accepted);
 name=card.CardName;value=card.CardVal;
}
#else
void CTCTavernGameCardPairs::GetRandomCard(long&name,long&value,long dealt,long total){
 CDefPointer<const CTavernGameCardBaseDef>def;GetCardDef(&def);CCardDef card;long half=total/2;
 bool accepted;do{accepted=true;unsigned int*seed=GetPWorldSeed();*seed=*seed*0x24a1+0x24df;*seed=GFROR13(*seed);card=def->CardPack[*seed%52];
 if(dealt>half){long original_card=dealt-half-1;if(Cards[original_card]->Value!=card.CardVal||Cards[original_card]->Name==card.CardName)accepted=false;}
 else{for(CCard**i=Cards.begin();i!=Cards.end();++i){if((*i)->Name==card.CardName||(*i)->Value==card.CardVal){accepted=false;break;}}}
 }while(!accepted);
 name=card.CardName;value=card.CardVal;
}
#endif

int main(){const int totals[]={4,6,8,10};const unsigned int seeds[]={0,1,2,17,0xffffffffU,0x80000000U,0x12345678U,0xabcdef01U};
 for(int t=0;t<4;++t)for(int dealt=0;dealt<totals[t];++dealt)for(int seed=0;seed<8;++seed)for(int alias=0;alias<2;++alias)for(int names=0;names<2;++names){
 CTavernGameCardBaseDef definition;CCardDef deck[52];for(int k=0;k<52;++k){deck[k].CardName=k;deck[k].CardVal=k%13;}definition.CardPack.First=deck;definition.CardPack.Count=52;expectedDef=&definition;
 CCard old[10];CCard*ptrs[10];for(int i=0;i<dealt;++i){old[i].Thing=0;old[i].Name=i+13*names;old[i].Value=i%13;ptrs[i]=&old[i];}CTCTavernGameCardPairs game;game.Cards.First=ptrs;game.Cards.Count=dealt;
 SeedWord expectedSeed=seeds[seed];int tries=0;long expectedName=0,expectedValue=0;
 for(;;){expectedSeed=(SeedWord)(expectedSeed*0x24a1U+0x24dfU);expectedSeed=(SeedWord)((expectedSeed>>13)|(expectedSeed<<19));unsigned int choice=expectedSeed%52;expectedName=(long)choice;expectedValue=(long)(choice%13);bool accept=true;if(dealt>totals[t]/2){const CCard&o=old[dealt-totals[t]/2-1];accept=o.Value==expectedValue&&o.Name!=expectedName;}else for(int q=0;q<dealt;++q)if(old[q].Name==expectedName||old[q].Value==expectedValue){accept=false;break;}CHECK(++tries<10000);if(accept)break;}
 worldSeed=seeds[seed];draws=copies=cardCtors=cardDtors=defCalls=0;long name=-99,value=-88;
 game.GetRandomCard(name,alias?name:value,dealt,totals[t]);CHECK(name==(alias?expectedValue:expectedName));if(!alias)CHECK(value==expectedValue);CHECK(worldSeed==expectedSeed&&draws==tries&&copies==tries&&cardCtors==1&&cardDtors==1&&defCalls==1&&definition.RefCount==1);
 for(int z=0;z<dealt;++z)CHECK(old[z].Name==z+13*names&&old[z].Value==z%13);for(int d=0;d<52;++d)CHECK(deck[d].CardName==d&&deck[d].CardVal==d%13);++cases;
 }
 CHECK(cases==896);puts("007d75a0_TEST PASS");return 0;}
