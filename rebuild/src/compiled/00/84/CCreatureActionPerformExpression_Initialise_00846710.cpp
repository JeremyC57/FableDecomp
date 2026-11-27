// 00846710 CCreatureAction_PerformExpression::Initialise
// Leaf: ?Initialise@CCreatureAction_PerformExpression@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_EXPRESSION_INIT_PASS29
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD
#define FABLE_RETAIL_CTCEMOTEICON_STATTYPE
#define FABLE_RETAIL_CTCHERO
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CDEFSTRING_TOWIDESTRING_INLINE
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_PerformExpression.h"
#include "engine/CTCInventory.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CExpressionDef.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCExpression.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CQuestManager.h"
#include "engine/CTCHero.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCTargetingBase.h"
#include "engine/EOpinionPostDeedType.h"
#include "engine/CThingAICreature.h"
#include "engine/NGlobalConsole.h"
void CCreatureAction_PerformExpression::Initialise()
{
    CCreatureActionBase::Initialise();
    if (!PCreature->HasTC(TCI_EXPRESSION)) {
        PCreature->AddTC("CTCExpression",false,0);
        CTCExpression* ptc_expression;
        PCreature->GetTC(&ptc_expression);
        ptc_expression->SetPDef(PExpressionDef);
    }
    if (!PCreature->HasTC(TCI_EMOTEICON))
        PCreature->AddTC("CTCEmoteIcon",false,0);
    CTCEmoteIcon* ptc_emote_icon;
    PCreature->GetTC(&ptc_emote_icon);
    if (NGlobalConsole::DebugExpressionsUseSpriteOverrideText && PExpressionDef->SpriteOverrideText!="") {
        ptc_emote_icon->SetEmoteText(CTCEmoteIcon::EMOTE_PRIORITY_DEBUG_LOW,PExpressionDef->SpriteOverrideText.ToWideString(),-1.0f,0.3f);
        EmoteIconAdded=true;
    } else if (PeekDefinitionManager().PeekGlobalDef().ExpressionIndicatorIcon>0) {
        float total_time=TotalTime * GetFrameUpdateTime();
        if (Extended) total_time=-1.0f;
        ptc_emote_icon->SetEmoteIcon(CTCEmoteIcon::EMOTE_PRIORITY_EXPRESSION,PeekDefinitionManager().PeekGlobalDef().ExpressionIndicatorIcon,total_time,0.5f,255);
        EmoteIconAdded=true;
    }
    CTCSoundPlayer* ptc_sound;
    if (PCreature->GetTC(&ptc_sound)) {
        if (PExpressionDef->TextTag) {
            if (PExpressionDef->OpinionDeedType!=OPINION_DEED_TYPE_EXPRESSION_GIGGLE || !PCreature->IsChild())
                ptc_sound->PlayAttachedDialogue(PExpressionDef->TextTag,false,false,false,false);
        } else if (PExpressionDef->OpinionDeedType==OPINION_DEED_TYPE_EXPRESSION_SNEER)
            ptc_sound->PlayAttachedCriteria("EXPRESSION_SNEER",false,false,false,-1);
        else ptc_sound->PlayAttachedCriteria("EXPRESSION_USE",false,false,false,-1);
    }
    const CTCHero* ptc_hero;
    if (PCreature->PeekTC(&ptc_hero)) {
        CQuestManager::Get().SetExpressionPerformed(PExpressionDef->GetInstantiationName());
        CTCHeroOpinionDeedLog* ptc_deed_log;
        if (PCreature->GetTC(&ptc_deed_log)) {
            if (PExpressionDef->OpinionDeedType!=OPINION_DEED_TYPE_NONE && ptc_deed_log->PeekPDeedEffectsDef(PExpressionDef->OpinionDeedType)->ReactionRestrictToTarget) {
                CTCTargetingBase* ptc_targeting;
                PCreature->GetTC(&ptc_targeting);
                CThing* pthing=ptc_targeting->GetPTargetedThing();
                if (pthing && pthing->GetType()==1)
                    ptc_deed_log->PostDeed(PExpressionDef->OpinionDeedType,PCreature->PeekPos(),pthing,GFDebugPDynamicCast(pthing,(CThingAICreature*)0),OPINION_POST_DEED_TYPE_RECIPIENT,false,false,0,false,0);
            } else ptc_deed_log->PostDeed(PExpressionDef->OpinionDeedType,PCreature->PeekPos(),0,0,OPINION_POST_DEED_TYPE_WITNESSES,false,false,0,false,0);
        }
    }
}