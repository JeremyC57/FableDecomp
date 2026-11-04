// CTCTalk::OnCreate() -- a talker without a dialogue voice picks one at random from its creature def's dialogue voices
// Leaf: ?OnCreate@CTCTalk@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2C00, 165 bytes; CTCTalk vtable slot 4 (the CTCBase::OnCreate override).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const 0x00485815 (an ICF-folded
//   instantiation; its body is defined here, as retail instantiated it in this TU, so VC7.1 sees the def slot is not captured),
//   CTCBase::GetPWorldSeed() const 0x00686DA0, GFRandom(unsigned long, unsigned long*) 0x004CDAE0 (__fastcall),
//   CDefStringTable::GetString 0x009D49B0 on GDefStringTable (through the inline CDefString::GetString),
//   CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, the def's virtual slot 1 (CDefPointer release).
// Layout: Ego_r.pdb CTCTalk DialogueVoice (CCharString) +0x38; CThing PBaseDef +0x70; CCreatureDef DialogueVoices
//   (vector<CDefString>) retail +0x6C (engine/CCreatureDef.h FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_ONCREATE
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CCreatureDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CTCTalk.h"

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0 ?GFRandom@@YIKKPAK@Z

// GetSubDef<CCreatureDef> 0x00485815 (the landed GetSubDef<CTavernGameDef> 0x005ED4A0 body): the member template is only
// declared in CDefClassBase.h, so its instantiation is defined here; VC7.1 keeps it out of line.
template <>
bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCreatureDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCTalk::OnCreate()
{
    if (DialogueVoice.IsEmpty()) {
        CDefPointer<const CCreatureDef> creatureDef;
        if (GetParentThing().PeekBaseDef().GetSubDef(&creatureDef) && creatureDef->DialogueVoices.size() > 0) {
            unsigned long index = GFRandom(creatureDef->DialogueVoices.size(), GetPWorldSeed());
            DialogueVoice = creatureDef->DialogueVoices[index].GetString();
        }
    }
}