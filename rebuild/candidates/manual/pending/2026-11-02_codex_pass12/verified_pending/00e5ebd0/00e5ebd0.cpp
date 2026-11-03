// NScript::CV_BodyGuardScript::MakeHenchmanComment(const CCharString&,long,EHENCHMAN_COMMENT,const CScriptThing&) -- choose a henchman and start a one- or two-speaker comment.
// Leaf: ?MakeHenchmanComment@CV_BodyGuardScript@NScript@@QAE_NABVCCharString@@JW4EHENCHMAN_COMMENT@12@ABVCScriptThing@@@Z
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BODYGUARD
#include <stdlib.h>
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_BodyGuardScript.h"
extern CGameScriptInterfaceBase* pDSTGame;
CCharString __fastcall operator+(const char*,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
inline CTimer& CTimer::operator=(long seconds) { pDSTGame->SetTimer(TimerIndex,seconds); return *this; }
inline CScriptThing::CScriptThing(const CScriptThing& __that):CBaseClass(__that),PImp(__that.PImp) {}
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& rhs) const { return PImp->IsEqualTo(*rhs.PImp); }
inline CCharString CScriptThing::GetDataString() const { if(PImp.IsNull())return NullString;return PImp->GetDataString(); }

bool NScript::CV_BodyGuardScript::MakeHenchmanComment(const CCharString& comment_to_make,long comment_state,EHENCHMAN_COMMENT comment_type,const CScriptThing& specific_bodyguard)
{
    if(HenchmanList.size()==0) return false;
    if(Game.IsConversationActive(ConversationIndex)) return false;
    if(MasterScriptData.HenchmanCommentaryState!=0) return false;
    CommentaryTimer=5;
    MasterScriptData.HenchmanCommentaryState=comment_state;
    long HenchmanID=rand()%HenchmanList.size();
    CScriptThing Henchman=HenchmanList[HenchmanID];
    CScriptThing HenchmanBuddy=HenchmanList[(HenchmanID+1)%HenchmanList.size()];
    bool ResponseAllowed;
    if(comment_type==HM_COMMENT_MULTIPLE_RANDOM) ResponseAllowed=(rand()%3)==0;
    else { ResponseAllowed=true; if(comment_type==HM_COMMENT_SINGLE) ResponseAllowed=false; }
    
    if(specific_bodyguard.IsAlive() && !(specific_bodyguard==Henchman)) {
        HenchmanBuddy=Henchman;
        Henchman=specific_bodyguard;
    }
    CCharString HenchmanMorality;
    if(Henchman.IsAlive()) {
        HenchmanMorality=Henchman.GetDataString();
        HenchmanMorality.Truncate(HenchmanMorality.GetLength()-1);
    }
    if(HenchmanList.size()>1 && ResponseAllowed) {
        long HLStyleConversationIndex=Game.AddNewConversation(Henchman,false,false);
        ConversationIndex=HLStyleConversationIndex;
        Game.AddPersonToConversation(HLStyleConversationIndex,HenchmanBuddy);
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+Henchman.GetDataString()+"_"+comment_to_make,false,Henchman,HenchmanBuddy);
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+HenchmanBuddy.GetDataString()+"_"+comment_to_make+"_RESPONSE"+HenchmanMorality,false,HenchmanBuddy,Henchman);
    } else {
        long HLStyleConversationIndex=Game.AddNewConversation(Henchman,false,false);
        ConversationIndex=HLStyleConversationIndex;
        Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_016_BODYGUARD"+Henchman.GetDataString()+"_"+comment_to_make,false,Henchman,Game.GetHero());
    }
    return true;
}
