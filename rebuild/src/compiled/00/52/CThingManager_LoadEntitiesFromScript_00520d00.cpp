// CThingManager::LoadEntitiesFromScript(const CCharString& script, long version, long level, const set<CCharString>& sections, CASuspendableProcess* psuspend, long inclusions, vector<CThing*>& loaded_things) -- parse a level / persistence entity script and load the things of the selected sections
// Leaf: ?LoadEntitiesFromScript@CThingManager@@QAEXABVCCharString@@JJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@PAVCASuspendableProcess@@JAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@4@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520D00, length 1338 bytes, `ret 1Ch` (Ego_r.pdb / FableWin.pdb, public).
// Shape: the version / level are stored for LoadSingleThing; a version >= 2 script is a run of "XXXSectionStart <name>;" blocks
//   (a section loads when its name or CLevelLoader's load-all name is in the set; an empty block reads up to the ";" after
//   "XXXSectionEnd;"), each thing a "NewThing" persist context handed to LoadSingleThing and pushed onto the result vector; every
//   g_LoadTickInterval things the suspendable process is suspended (slot 2); old scripts are one "NULL" section of NewThing blocks.
//   The process is suspended once more and ActivateAfterLoading runs for the level.
// Callees: CCharString(const char*,long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*)
//   0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0; CStringParser(const CCharString&,...) 0x00404720, SkipPastString 0x009B9BA0,
//   SkipWhitespace 0x009B9C60, ReadAsStringUntilString 0x009BA330, SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilPastString
//   0x009BA3E0, the parser's ~list<CStringParserCommentBoundaries> 0x004046B0 and ~CBaseClass 0x0099A2E0 (implicit ~CStringParser);
//   set<CCharString> _M_find 0x004B9370; NProgressDisplay::DisplayProgress 0x009E9F40; CPersistContext(CStringParser&,...) 0x009BADD0;
//   LoadSingleThing 0x0051FD80; vector<CThing*>::_M_insert_overflow 0x00436AC0 (push_back); ActivateAfterLoading 0x0051E5A0.
//   Data: g_LoadTickInterval 0x01375714 (established spelling; Codex proposed NGlobalConsole::NoThingsToLoadPerSuspend, not
//   verifiable here), CLevelLoader::LoadAllSectionName 0x013BAB08 (PDB dynamic-initializer name; was g_CCharString_013bab08).
// Source: Codex pass 12 (RELOCATION_MATCH on its own header models, held 2026-11-03) rebased by chunk 846 onto the shared headers with
//   additive opt-ins (hdr/README.md): those of LoadSingleThing plus FABLE_RETAIL_CLEVELLOADER_LOADALL, FABLE_RETAIL_STLP_SET +
//   STLP_TREE_CONSTFIND (set::find const), FABLE_RETAIL_CCHARSTRING_LESS_INLINE. The Codex body is kept. CCharString::operator< inline
//   over the same-TU noinline CBasicString<char>::operator< body (pure reads) lets VC7.1 keep set::end() in esi across the first
//   _M_find as retail does (declared out of line: one extra `mov esi,[ebx]` reload and a 12-byte longer loop alignment).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_ALLOCATECLASS
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_DEAD
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ONACTIVATEENTITY
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CLEVELLOADER_LOADALL
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CQuestManager.h"
#include "engine/CLevelLoader.h"
#include "engine/CASuspendableProcess.h"
// CBasicString<char>::operator< 0x00429950 (landed), the callee of the set<CCharString> _M_find 0x004B9370: its body in the TU,
// kept out of line, as retail's TU sees it.
#pragma auto_inline(off)
template<> __declspec(noinline) bool CBasicString<char>::operator<(const CBasicString<char>& other) const
{
    const char* b=other.PData;
    const char* a=PData;
    for(;;) {
        if(*a==0 && *b==0) return false;
        if(*a<*b) return true;
        if(*a>*b) return false;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
namespace NProgressDisplay {void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}

extern unsigned long g_LoadTickInterval;   // 0x01375714 (established spelling)
void CThingManager::LoadEntitiesFromScript(const CCharString& script,long version,long level,const std::set<CCharString>& sections,CASuspendableProcess* psuspend,long inclusions,std::vector<CThing*>& loaded_things)
{
    unsigned long how_many_is_some=g_LoadTickInterval;
    SerialiseVersionNumber=version;
    LevelBeingLoaded=level;
    bool loading_with_suspends=psuspend!=0;
    CStringParser parser(script,0,CCharString(""),0);
    long count=0;
    CurrentSectionName="";
    if(version>=2) {
        while(parser.SkipPastString("XXXSectionStart")) {
            parser.SkipWhitespace();
            CCharString name=parser.ReadAsStringUntilString(";");
            parser.SkipPastString(";");
            if(sections.find(name)!=sections.end() || sections.find(CLevelLoader::GetLoadAllSectionName())!=sections.end()) {
                CurrentSectionName=name;
                if(parser.SkipPastNextItemIfIs("XXXSectionEnd;")) {
                    parser.ReadAsStringUntilPastString(";");
                    CurrentSectionName="";
                } else {
                    while(parser.SkipPastNextItemIfIs("NewThing")) {
                        NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
                        CPersistContext thing_context(parser,0,true,0,0,0);
                        CThing* pthing=LoadSingleThing(thing_context,inclusions);
                        if(pthing) loaded_things.push_back(pthing);
                        if(++count>=how_many_is_some) {
                            count=0;
                            if(loading_with_suspends)psuspend->SuspendSuspendableProcess();
                        }
                    }
                    parser.SkipPastString("XXXSectionEnd;");
                }
            } else parser.SkipPastString("XXXSectionEnd;");
        }
    } else {
        CurrentSectionName="NULL";
        while(parser.SkipPastString("NewThing")) {
            NProgressDisplay::DisplayProgress("Loading entities from script",-1.0f,false,false);
            CPersistContext thing_context(parser,0,true,0,0,0);
            CThing* pthing=LoadSingleThing(thing_context,inclusions);
            if(pthing) loaded_things.push_back(pthing);
            ++count;
            if(loading_with_suspends && count>=how_many_is_some) {
                count=0;
                psuspend->SuspendSuspendableProcess();
            }
        }
    }
    if(psuspend)psuspend->SuspendSuspendableProcess();
    ActivateAfterLoading(level);
}