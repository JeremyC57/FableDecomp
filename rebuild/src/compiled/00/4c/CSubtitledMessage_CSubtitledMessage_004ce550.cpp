// 004ce550 CSubtitledMessage::CSubtitledMessage
#define FABLE_RETAIL_CSUBTITLEDMESSAGE_PASS29
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
// Leaf: ??0CSubtitledMessage@@QAE@ABVCTextEntry@NGameText@@@Z
// Flags: /O2 /Gr
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STRING_MEMTRACKING_PASS26
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#define FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CStringParser.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CASoundManager.h"
#include "engine/CVectorMap.h"
#include "engine/CSubtitledMessage.h"
#include "engine/CCRCSymbolMap.h"

extern CCRCSymbolMap g_MiscDefTypesSymbolMap;

CSubtitledMessage::CSubtitledMessage(const NGameText::CTextEntry& entry)
    : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Text(entry.GetText()), Narrator(entry.GetNarrator())
{
    PSpeechBank = PSoundManager->GetPSoundBank(entry.GetSpeechBank());
    if (PSpeechBank == 0)
        PSpeechBank = PSoundManager->GetPSoundBank(1);
    long textLength = Text.GetLength();
    const CArray<NGameText::CTextTag>& tags = entry.GetTags();
    CDefPointer<const CGlobalDef> globalDefs;
    {
        CCharString defsName("DEFAULT_GLOBAL_DEFS");
        CGameDefinitionManager::Get().CDefinitionManager::GetDef<CGlobalDef>(defsName, &globalDefs);
    }
    for (long i = 0; i < tags.size(); ++i) {
        float time = (float)tags[i].Pos / (float)textLength;
        const CCharString& tag = tags[i].Name;
        if (tag.StartsWith("ANIM:")) {
            AnimationTags.Insert(time, tag.GetSubString(5, tag.GetLength()));
        } else if (tag.StartsWith("CAM_SHOT:")) {
            AddMasterShotCameraTagFromText(time, tag.GetSubString(9, tag.GetLength()));
        } else if (tag.StartsWith("CAM:")) {
            AddCameraTagFromText(time, tag.GetSubString(4, tag.GetLength()));
        } else if (tag.StartsWith("CAM")) {
            CCharString rest = tag.GetSubString(3, tag.GetLength());
            CStringParser parser(rest, 0, CCharString(""), 0);
            if (parser.NextItemExists() && parser.PeekNextItemType() == PARSED_ITEM_INTEGER) {
                long camera_number=parser.ReadAsInteger();
                bool camera_tag_check=AddCameraTagFromIndex(camera_number, time, globalDefs);
            }
        } else {
            long attitude = 0;
            if (g_MiscDefTypesSymbolMap.FindSymbol(tags[i].Name, attitude))
                AttitudeTags.Insert(time, (EConversationAttitude)attitude);
        }
    }
    AttitudeTags.Update();
    AnimationTags.Update();
    CameraTags.Update();
}