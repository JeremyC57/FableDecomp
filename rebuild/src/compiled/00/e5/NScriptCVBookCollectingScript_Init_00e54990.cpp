// 00e54990 NScript::CV_BookCollectingScript::Init
// Flags: /O2 /Oy /W3
// Leaf: ?Init@CV_BookCollectingScript@NScript@@QAEXXZ
// NF48 (round 7): the local namespace X_00e54990 (its std::vector, CConversation, CScriptDef and ScriptDef stand-ins) is gone: the
// vectors are STLport 4.6.2 std::vector<T> from the shared stlport/fable_containers.h, CConversation / CScriptDef / pGlobalScriptDef
// (0x0143E90C, Ego_r.pdb CDefPointer<const CScriptDef>) come from engine/CConversation.h and engine/fable_script_def_global.h.
// BookReactions = ... is the out-of-line vector<CConversation>::operator= 0x00E54AA0 (FABLE_RETAIL_STLP_VECTOR_ASSIGN); the one-argument
// resize is STLport's inline resize(n, T()) over the out-of-line resize(n, const T&) (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE):
// vector<CCharString> 0x0043332C, vector<bool> 0x00467686 (Ego_r.pdb types BookDonated / BookOwned vector<bool>; the landed
// 0x00467686 row is std::vector<bool>::resize and 0x00CDD0A0 binds that spelling; retail's vector<bool> is a plain byte vector).
// Layout: retail CV_BookCollectingScript (Ego_r.pdb member names): ReadingBook +0x48, BookReactions +0x4c, DoneIntro +0x78 .. BookOwned
// +0xac; the base prefix and resource wrappers are untouched here. CScriptDef (header, retail offsets): BookReactions +0x4c8,
// BookDefNames +0x4d4, BooksNumberTotal +0x4ec.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CConversation.h"
#include "engine/fable_script_def_global.h"
namespace NScript{
struct CV_BookCollectingScript{unsigned char Prefix[0x48];bool ReadingBook;std::vector<CConversation>BookReactions;unsigned char ResourceWrappers[0x20];bool DoneIntro,HatRewarded,KeyRewarded;long GossipState;bool HasInformation;long BooksDonated,GoodBooksDonated,LastBookRequested,BooksInGame;std::vector<CCharString>BookDef;std::vector<bool>BookDonated,BookOwned;void Init();};
void CV_BookCollectingScript::Init(){ReadingBook=false;BookReactions=pGlobalScriptDef->BookReactions;DoneIntro=false;HatRewarded=false;KeyRewarded=false;GossipState=0;HasInformation=false;BooksDonated=0;GoodBooksDonated=0;LastBookRequested=-1;BooksInGame=pGlobalScriptDef->BooksNumberTotal;BookDef.resize(BooksInGame);BookDonated.resize(BooksInGame);BookOwned.resize(BooksInGame);for(long i=0;i<BooksInGame;++i){BookDef[i]=pGlobalScriptDef->BookDefNames[i];BookOwned[i]=false;BookDonated[i]=false;}}
}
