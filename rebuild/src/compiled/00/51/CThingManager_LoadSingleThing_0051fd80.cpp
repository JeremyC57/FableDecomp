// CThingManager::LoadSingleThing(CPersistContext& context, long inclusions) -- restore or construct one serialised thing
// Leaf: ?LoadSingleThing@CThingManager@@AAEPAVCThing@@AAVCPersistContext@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051FD80, length 1320 bytes, `ret 8` (private: Ego_r.pdb / FableWin.pdb
//   ?LoadSingleThing@CThingManager@@AAEPAVCThing@@AAVCPersistContext@@J@Z; callers LoadHeroStateBinary 0x00520670 and the entity loaders).
// Shape: the thing name, its CSerialiseInfo and the level-script sub script are read from a binary stream (ReadNullTerminatedString,
//   the static SerialiseThingInfo) or from the text parser ("<name>;" then up to "EndThing"); the definition decides whether the
//   inclusion filter (0 all, 1 level things, 2 globals) loads it; the UID is remapped, an existing thing is reused (revived from limbo,
//   or its UID fixed), the hero's thing is reused while the world serialises the hero state, a thing persisted as dead is skipped;
//   otherwise the class is allocated by name, constructed (binary save / level script virtuals), then activated or created and
//   tagged with the current section name, or killed. Five DisplayProgress milestones.
// Callees: CCharString() 0x0099E4B0 / (const char*,long) 0x0099EBF0 / ~ 0x0099EAE0 / operator= 0x0099EFB0 / operator!=(const char*)
//   0x0099E960 / operator const char* 0x0099E4C0; CBasicString<char>::operator==(const char*) 0x004115A0; NProgressDisplay::
//   DisplayProgress 0x009E9F40; CThing::SerialiseThingInfo 0x004C81F0 / 0x004C88E0; CStringParser::SkipWhitespace 0x009B9C60,
//   ReadAsStringUntilString 0x009BA330, SkipPastString 0x009B9BA0; CDefinitionManager::GetDef<CThingBaseDef> 0x00528760;
//   RemapUIDForSerialisation 0x0051E210; the UIDToThingMap / PersistedDeadEntities _M_find 0x00527D80 / 0x00527D20; CThing::SetInLimbo
//   0x004C8CF0, IsLevelPersistent 0x004C82A0, Kill 0x004C9B80; CQuestManager::OnActivateEntity 0x004AFA60; CPlayerManager::
//   GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; CheckAndFixUID 0x0051F340; CClassFactory::AllocateClass
//   0x00A371C0; CThing virtual slots 4 / 8 / 9 / 10 / 16; ReadNullTerminatedString virtual slot 6.
// Source: Codex pass 12 (RELOCATION_MATCH on its own header models, held 2026-11-03) rebased by chunk 846 onto the shared headers:
//   the missing members are additive opt-ins (hdr/README.md): FABLE_RETAIL_CTHING_LOADSINGLE, FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE,
//   FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_DEAD, FABLE_RETAIL_STLP_TREE_CONSTFIND, FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY,
//   FABLE_RETAIL_CCLASSFACTORY_ALLOCATECLASS, FABLE_RETAIL_CQUESTMANAGER_ONACTIVATEENTITY. The Codex body is kept; the definition
//   manager and level are read through the inline Ego_r accessors PeekDefinitionManager / GetLevelBeingLoaded (direct member reads
//   load the this-based operands in a different order), and the landed GetDef<CThingBaseDef> body sits in the TU under
//   auto_inline(off) (declared only, the frame is 20 bytes smaller and the strings move).
#define FABLE_RETAIL_CCHARSTRING
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
namespace NProgressDisplay {
    void __fastcall DisplayProgress(const CCharString&,float,bool,bool);
}
// The landed out-of-line GetDef<CThingBaseDef> (0x00528760) body, visible to the TU as in retail's thing_manager TU and kept out
// of line (GOTCHAS: same-TU callee bodies under auto_inline(off)).
#pragma auto_inline(off)
template<class T> bool CDefinitionManager::GetDef(const CCharString& name,CDefPointer<const T>* out) const
{
    CDefPointer<const CDefClassBase> def=GetPDefFromInstantiationName(name);
    if(!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
CThing* CThingManager::LoadSingleThing(CPersistContext& context,long inclusions)
{
    CCharString name;
    CThing::CSerialiseInfo thing_info;
    CCharString sub_script;
    NProgressDisplay::DisplayProgress("Load Single Thing 1",-1.0f,false,false);
    if(context.IsBinary()) {
        name=context.GetPLoadStream()->ReadNullTerminatedString();
        CThing::SerialiseThingInfo(context,thing_info);
    } else {
        CStringParser& parser=*context.GetPParser();
        parser.SkipWhitespace();
        name=parser.ReadAsStringUntilString(";");
        parser.SkipPastString(";");
        sub_script=parser.ReadAsStringUntilString("EndThing");
        CThing::SerialiseThingInfo(false,sub_script,thing_info);
        parser.SkipPastString("EndThing;");
    }
    NProgressDisplay::DisplayProgress("Load Single Thing 2",-1.0f,false,false);
    bool load_it=true;
    CDefPointer<const CThingBaseDef> pdef;
    if(thing_info.DefinitionType!="NULL") {
        if(PeekDefinitionManager().GetDef(thing_info.DefinitionType,&pdef)) {
            if(!((pdef->IsLoadableGlobal && (inclusions==0 || inclusions==2)) ||
                 (!pdef->IsLoadableGlobal && (inclusions==0 || inclusions==1)))) load_it=false;
        } else {ErrorFlag=true;load_it=false;}
    } else if(inclusions!=0 && inclusions!=2) load_it=false;
    CThing* pout_thing=0;
    if(load_it) {
        bool level_less=false;
        if(!pdef.IsNull()) level_less=pdef->LevelLess;
        unsigned __int64 uid=RemapUIDForSerialisation(false,level_less,GetLevelBeingLoaded(),thing_info.UID);
        CThing* ptemp_thing=GetPThingWithUID(uid);
        bool create_it=true;
        bool is_serialising_hero_state=false;
        if(World.IsSerialisingHeroState() && name=="PlayerCreature") {
            is_serialising_hero_state=true;
            CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            ptemp_thing=phero;
        } else {
            if(ptemp_thing) {
                if(!ptemp_thing->IsToKillOnLevelUnload()) {
                    create_it=false;
                    if(ptemp_thing->IsInLimbo()) {
                        ptemp_thing->SetInLimbo(false);
                        CQuestManager::Get().OnActivateEntity(*ptemp_thing);
                    }
                } else {
                    if(ptemp_thing->IsLevelPersistent()) create_it=false;
                    else CheckAndFixUID(uid);
                }
            }
        }
        if(!is_serialising_hero_state && EntityRuntimePersistence.IsEntityPersistedAsDead(uid)) create_it=false;
        if(create_it) {
            if(!is_serialising_hero_state) {
                NProgressDisplay::DisplayProgress("Load Single Thing: Allocate Class",-1.0f,false,false);
                CClass* pclass=AllocateClass(name);
                if(pclass) ptemp_thing=static_cast<CThing*>(pclass);
            }
            if(ptemp_thing) {
                NProgressDisplay::DisplayProgress("Load Single Thing: Construct Thing",-1.0f,false,false);
                if(context.IsBinary()) ptemp_thing->ConstructFromBinarySave(*context.GetPLoadStream(),thing_info);
                else ptemp_thing->ConstructFromLevelScript(sub_script);
                NProgressDisplay::DisplayProgress("Load Single Thing: Initial Activate",-1.0f,false,false);
                if(is_serialising_hero_state) {
                    ptemp_thing->InitialActivate();
                    ptemp_thing->OnInitialActivate();
                    pout_thing=ptemp_thing;
                } else if(ptemp_thing->OnCreate()) {
                    ptemp_thing->SetSerialisationSectionName(CurrentSectionName);
                    pout_thing=ptemp_thing;
                } else ptemp_thing->Kill(true);
            }
        } else pdef.Release();
    }
    NProgressDisplay::DisplayProgress("Load Single Thing 3",-1.0f,false,false);
    return pout_thing;
}