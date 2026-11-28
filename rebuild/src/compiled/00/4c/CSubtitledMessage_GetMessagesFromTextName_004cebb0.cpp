// Leaf: ?GetMessagesFromTextName@CSubtitledMessage@@SIXAAV?$CArray@VCSubtitledMessage@@@@PBD1W4ETextGroupSelectionMethod@@@Z
// Flags: /O2 /Oy /W3 /Gr
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
#define FABLE_RETAIL_CSUBTITLEDMESSAGE_PASS29
#define FABLE_RETAIL_SUBTITLE_GETMESSAGES_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CVectorMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/GFRandom.h"
#include "engine/CSubtitledMessage.h"
#include "fable_assert.h"

void FABLE_FASTCALL CSubtitledMessage::GetMessagesFromTextName(
    CArray<CSubtitledMessage>& messages, const char* ptext_name,
    const char* pnarrator_name, ETextGroupSelectionMethod selection_method)
{
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long text_index = text_bank.FindIndexBySymbolCRC(CCharString(ptext_name).GetCRC());
    if (text_index > 0) {
        if (text_bank.GetEntryDataType(text_index) == NGameText::CDataBank::TYPE_TEXT) {
            FABLE_RETAIL_ASSERT(pnarrator_name == 0);
            messages.push_back(GetMessageFromTextIndex(text_index, pnarrator_name));
        } else {
            FABLE_RETAIL_ASSERT(text_bank.GetEntryDataType(text_index) == NGameText::CDataBank::TYPE_GROUP);
            const CArray<long>& group_entries = text_bank.GetPGroupEntry(text_index, true)->GetTextEntries();
            if (selection_method == GROUP_SELECT_NONE) {
                for (long c1 = 0; c1 < group_entries.size(); ++c1)
                    messages.push_back(GetMessageFromTextIndex(group_entries[c1], pnarrator_name));
            } else if (selection_method == GROUP_SELECT_FIRST_ONLY) {
                if (!group_entries.empty())
                    messages.push_back(GetMessageFromTextIndex(group_entries.front(), pnarrator_name));
            } else {
                const CWorld& world = CMainGameComponent::Get()->PeekWorld();
                if (CCharString(ptext_name) != MostRecentGroupQueried) {
                    MostRecentGroupQueried = CCharString(ptext_name);
                    LastReturnedIndex = -1;
                }
                long random_index = 0;
                switch (selection_method) {
                case GROUP_SELECT_RANDOM:
                    random_index = GFRandom(group_entries.size(), world.GetWorldSeed());
                    break;
                case GROUP_SELECT_RANDOM_NO_REPEAT:
                    if (group_entries.size() > 1 && LastReturnedIndex != -1) {
                        random_index = GFRandom(group_entries.size() - 1, world.GetWorldSeed());
                        if (random_index >= LastReturnedIndex)
                            ++random_index;
                    } else {
                        random_index = GFRandom(group_entries.size(), world.GetWorldSeed());
                    }
                    break;
                case GROUP_SELECT_SEQUENTIAL:
                    random_index = LastReturnedIndex + 1;
                    if (random_index >= group_entries.size())
                        random_index = 0;
                    break;
                default:
                    FABLE_RETAIL_ASSERT(false);
                    break;
                }
                LastReturnedIndex = random_index;
                messages.push_back(GetMessageFromTextIndex(group_entries[random_index], pnarrator_name));
            }
        }
    } else {
        CWideString text_string = L"(HC)" + CCharString(ptext_name).ToWideString();
        messages.push_back(CSubtitledMessage(text_string));
    }
}