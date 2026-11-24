// Leaf: ?CalculateNextTextTag@CProgressDisplay@@AAEXXZ
// Flags:
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUIDEF_PROGRESS
#define FABLE_RETAIL_SINGLETON_ISSETUP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVENAME
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
#include "fable_user_profile.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CProgressDisplay.h"

inline const NGameText::CDataBank* CProgressDisplay::GetPTextBank() const
{
    if (CMainGameComponent::Get() && CMainGameComponent::Get()->IsTextBankInitialised())
        return &CMainGameComponent::Get()->GetTextBank();
    if (CNewFrontendGameComponent::Get() && CNewFrontendGameComponent::Get()->IsTextBankInitialised())
        return &CNewFrontendGameComponent::Get()->GetTextBank();
    return 0;
}

void CProgressDisplay::CalculateNextTextTag()
{
    if (DisplayText)
    {
        const NGameText::CDataBank* ptext_bank = GetPTextBank();
        if (ptext_bank)
        {
            bool moved_to_next_in_group = false;
            if (TextTagIndex != -1)
            {
                if (TextTag.EndsWith("_10"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_20";
                else if (TextTag.EndsWith("_20"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_30";
                else if (TextTag.EndsWith("_30"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_40";
                else if (TextTag.EndsWith("_40"))
                    TextTag = TextTag.GetSubString(0, TextTag.GetLength() - 3) + "_50";
                else
                    TextTag = "";
                if (TextTag != "")
                    if (ptext_bank->FindIndexBySymbolCRC(TextTag.GetCRC()) != 0)
                        moved_to_next_in_group = true;
            }
            if (!moved_to_next_in_group)
            {
                const std::vector<CCharString>& text_tags = TextTagsPC;
                if (!text_tags.empty())
                {
                    ++TextTagIndex;
                    if (static_cast<unsigned long>(TextTagIndex) >= text_tags.size())
                        TextTagIndex = 0;
                    TextTag = text_tags[TextTagIndex];
                    unsigned long index = ptext_bank->FindIndexBySymbolCRC(TextTag.GetCRC());
                    if (index > 0 && ptext_bank->GetEntryDataType(index) == 1)
                        TextTag += "_10";
                }
            }
        }
    }
}