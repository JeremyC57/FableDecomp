// NScript::CV_BodyGuardScript::MakeHenchmanPerformIdleComment() -- an idle henchman remark: give up unless every henchman is in state group 2; after 15 s standing still with the bored timer run out, set it to 30 and make a BORED comment; when the idle timer has run out, set it to 60 and (two or more henchmen, three times in four) ask a QUESTION, else pick henchmen round from a random start and make the first good one (hero morality above neutral) or evil one (below neutral) say HERO_IDLE
// Grade: DIFFER(1594v1580) -- md 116: ~CScriptThing per-site split: with the row-local inline dtor every site expands; retail expands only the BORED temporary and the GOOD exit and calls 0x004AA840 at the QUESTION temporary, the EVIL exit and the loop end (BORED/QUESTION tails then cross-jump); the diagnostic inline-threshold meter (T60, see NOTES.md) reproduces retail's split exactly (inline-threshold residue: park, no flag kept)
// Next idea: none in plain C++ found (else-if and nested-if forms grade byte-identical); retry when a per-site ~CScriptThing spelling is known
// Leaf: ?MakeHenchmanPerformIdleComment@CV_BodyGuardScript@NScript@@QAE_NXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_NSCRIPT
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_BodyGuardScript.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CTimer& CTimer::operator=(long seconds) { pDSTGame->SetTimer(TimerIndex,seconds); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline long CCounter::GetFrameDelta() const { long delta=pDSTGame->GetWorldFrame()-RelevantWorldFrame; if (delta<0) delta=0; return delta; }
inline bool CCounter::operator>(float seconds) const { float elapsed=(float)GetFrameDelta(); return elapsed>(float)pDSTGame->GetConstantFPS()*seconds; }
inline CScriptThing::CScriptThing(const CScriptThing& __that):CBaseClass(__that),PImp(__that.PImp) {}
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CCharString CScriptThing::GetDataString() const { if(PImp.IsNull())return NullString;return PImp->GetDataString(); }

bool NScript::CV_BodyGuardScript::MakeHenchmanPerformIdleComment()
{
    for (unsigned long i=0;i<HenchmanList.size();i++) {
        if (HenchmanList[i].GetCurrentStateGroupType()!=(EAIStateGroupType)2) return false;
    }
    if (HeroStoodStillCounter>15.0f && HenchmanLastBoredChatTimer==0) {
        HenchmanLastBoredChatTimer=30;
        MakeHenchmanComment("BORED",1,HM_COMMENT_MULTIPLE_RANDOM);
        return true;
    }
    if (HenchmanLastIdleChatTimer==0) {
        HenchmanLastIdleChatTimer=60;
        if (HenchmanList.size()>1 && rand()%4) {
            MakeHenchmanComment("QUESTION",1,HM_COMMENT_MULTIPLE);
            return true;
        }
        if (HenchmanList.size()) {
            unsigned long start=rand()%HenchmanList.size();
            for (unsigned long j=0;j<HenchmanList.size();j++) {
                CScriptThing henchman=HenchmanList[(start+j)%HenchmanList.size()];
                if (Game->GetHeroMoralityCategory()>3 && (henchman.GetDataString()=="_GOOD1" || henchman.GetDataString()=="_GOOD2")) {
                    MakeHenchmanComment("HERO_IDLE",1,HM_COMMENT_SINGLE,henchman);
                    return true;
                }
                if (Game->GetHeroMoralityCategory()<3 && (henchman.GetDataString()=="_EVIL1" || henchman.GetDataString()=="_EVIL2")) {
                    MakeHenchmanComment("HERO_IDLE",1,HM_COMMENT_SINGLE,henchman);
                    return true;
                }
            }
        }
    }
    return true;
}
