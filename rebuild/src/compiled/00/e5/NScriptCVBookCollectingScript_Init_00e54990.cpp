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