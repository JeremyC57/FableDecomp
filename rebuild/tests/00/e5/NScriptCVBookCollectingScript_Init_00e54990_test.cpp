// 00e54990 NScript::CV_BookCollectingScript::Init
// Flags: /O2 /Oy /W3
// Leaf: Init
namespace X_00e54990{
struct CCharStringData;struct CCharString{CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString&operator=(const CCharString&);};
namespace _STL{template<class T>struct vector{T*Start,*Finish,*Storage;vector&operator=(const vector&);void resize(unsigned,const T&);__forceinline void resize(unsigned n){resize(n,T());}};}
// Layout: retail CConversation is92: ten prefix words, Lines40 and four12-byte string vectors44/56/68/80. PDB gives original member names, donor vector size16.
struct CConversation{unsigned Prefix[10];long Lines;_STL::vector<CCharString>Speaker,Dialogue,Animation,AnimLoop;};
// Layout: retail ScriptDef BookReactions4c8, BookDefNames4d4, BooksNumberTotal4ec, from this function. No active header exists.
struct CScriptDef{unsigned char Prefix[0x4c8];_STL::vector<CConversation>BookReactions;_STL::vector<CCharString>BookDefNames;long BooksNumberWanted,BooksNumberAccepted,BooksNumberComment,BooksNumberTotal;};extern CScriptDef*ScriptDef;
namespace NScript{
// Layout: original names from PDB CV_BookCollectingScript; retail offsets ReadingBook48, BookReactions4c, DoneIntro78 through BookOwnedac. Base prefix and resource wrappers untouched here.
struct CV_BookCollectingScript{unsigned char Prefix[0x48];bool ReadingBook;_STL::vector<CConversation>BookReactions;unsigned char ResourceWrappers[0x20];bool DoneIntro,HatRewarded,KeyRewarded;long GossipState;bool HasInformation;long BooksDonated,GoodBooksDonated,LastBookRequested,BooksInGame;_STL::vector<CCharString>BookDef;_STL::vector<unsigned char>BookDonated,BookOwned;void Init();};
void CV_BookCollectingScript::Init(){ReadingBook=false;BookReactions=ScriptDef->BookReactions;DoneIntro=false;HatRewarded=false;KeyRewarded=false;GossipState=0;HasInformation=false;BooksDonated=0;GoodBooksDonated=0;LastBookRequested=-1;BooksInGame=ScriptDef->BooksNumberTotal;BookDef.resize(BooksInGame);BookDonated.resize(BooksInGame);BookOwned.resize(BooksInGame);for(long i=0;i<BooksInGame;++i){BookDef.Start[i]=ScriptDef->BookDefNames.Start[i];BookOwned.Start[i]=false;BookDonated.Start[i]=false;}}
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <stddef.h>
namespace X_00e54990{
CScriptDef*ScriptDef;struct CCharStringData{unsigned refs;char text[24];};CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&r):PStringData(r.PStringData){if(PStringData)++PStringData->refs;}CCharString::~CCharString(){if(PStringData&&!--PStringData->refs)delete PStringData;}CCharString&CCharString::operator=(const CCharString&r){if(this!=&r){if(r.PStringData)++r.PStringData->refs;if(PStringData&&!--PStringData->refs)delete PStringData;PStringData=r.PStringData;}return *this;}
namespace _STL{
template<class T>void clear(vector<T>&v){for(T*p=v.Start;p!=v.Finish;++p)p->~T();free(v.Start);v.Start=v.Finish=v.Storage=0;}
template<class T>void vector<T>::resize(unsigned n,const T&value){unsigned old=Finish-Start;T*p=n?(T*)malloc(n*sizeof(T)):0;for(unsigned i=0;i<n;++i)new(p+i)T(i<old?Start[i]:value);clear(*this);Start=p;Finish=Storage=p+n;}
template<>void clear(vector<CConversation>&v){for(CConversation*p=v.Start;p!=v.Finish;++p){clear(p->Speaker);clear(p->Dialogue);clear(p->Animation);clear(p->AnimLoop);}free(v.Start);v.Start=v.Finish=v.Storage=0;}
template<class T>void CopyValue(T&out,const T&in){out=in;}
void CopyValue(CConversation&out,const CConversation&in){memset(&out,0,sizeof(out));memcpy(out.Prefix,in.Prefix,sizeof(out.Prefix));out.Lines=in.Lines;out.Speaker=in.Speaker;out.Dialogue=in.Dialogue;out.Animation=in.Animation;out.AnimLoop=in.AnimLoop;}
template<class T>vector<T>&vector<T>::operator=(const vector&r){if(this!=&r){clear(*this);unsigned n=r.Finish-r.Start;Start=n?(T*)malloc(n*sizeof(T)):0;for(unsigned i=0;i<n;++i){new(Start+i)T;CopyValue(Start[i],r.Start[i]);}Finish=Storage=Start+n;}return *this;}
}

static void Set(CCharString&s,const char*t){s.PStringData=new CCharStringData;s.PStringData->refs=1;strcpy(s.PStringData->text,t);}
}
int main(){using namespace X_00e54990;using namespace X_00e54990::NScript;assert(offsetof(CV_BookCollectingScript,BookDef)==0x94&&offsetof(CV_BookCollectingScript,BookOwned)==0xac&&offsetof(CScriptDef,BooksNumberTotal)==0x4ec);CScriptDef def;memset(&def,0,sizeof(def));ScriptDef=&def;CCharString empty;def.BookDefNames.resize(3,empty);Set(def.BookDefNames.Start[0],"BOOK_A");Set(def.BookDefNames.Start[1],"BOOK_B");Set(def.BookDefNames.Start[2],"BOOK_C");CConversation reaction;memset(&reaction,0,sizeof(reaction));def.BookReactions.resize(1,reaction);CConversation&entry=def.BookReactions.Start[0];entry.Lines=2;entry.Speaker.resize(1,empty);entry.Dialogue.resize(1,empty);entry.Animation.resize(1,empty);entry.AnimLoop.resize(1,empty);Set(entry.Speaker.Start[0],"teacher");Set(entry.Dialogue.Start[0],"thank_you");Set(entry.Animation.Start[0],"read");Set(entry.AnimLoop.Start[0],"loop");def.BooksNumberTotal=3;CV_BookCollectingScript s;memset(&s,0,sizeof(s));s.Init();assert(s.BookReactions.Finish-s.BookReactions.Start==1&&s.BookReactions.Start[0].Lines==2&&s.BookReactions.Start[0].Speaker.Start!=entry.Speaker.Start&&s.BookReactions.Start[0].Speaker.Start[0].PStringData==entry.Speaker.Start[0].PStringData&&entry.Speaker.Start[0].PStringData->refs==2);assert(!s.ReadingBook&&!s.DoneIntro&&!s.HatRewarded&&!s.KeyRewarded&&!s.HasInformation&&s.GossipState==0&&s.BooksDonated==0&&s.GoodBooksDonated==0&&s.LastBookRequested==-1&&s.BooksInGame==3);for(unsigned i=0;i<3;++i){assert(s.BookDef.Start[i].PStringData==def.BookDefNames.Start[i].PStringData&&s.BookDef.Start[i].PStringData->refs==2&&!s.BookDonated.Start[i]&&!s.BookOwned.Start[i]);s.BookDonated.Start[i]=1;s.BookOwned.Start[i]=1;}s.Init();for(unsigned j=0;j<3;++j)assert(!s.BookDonated.Start[j]&&!s.BookOwned.Start[j]&&s.BookDef.Start[j].PStringData->refs==2);def.BooksNumberTotal=0;s.Init();assert(s.BookDef.Start==s.BookDef.Finish&&s.BookDonated.Start==s.BookDonated.Finish&&s.BookOwned.Start==s.BookOwned.Finish&&def.BookDefNames.Start[0].PStringData->refs==1);_STL::clear(s.BookDef);_STL::clear(s.BookDonated);_STL::clear(s.BookOwned);_STL::clear(def.BookDefNames);_STL::clear(s.BookReactions);assert(entry.Speaker.Start[0].PStringData->refs==1);_STL::clear(def.BookReactions);puts("00e54990_TEST PASS");return 0;}