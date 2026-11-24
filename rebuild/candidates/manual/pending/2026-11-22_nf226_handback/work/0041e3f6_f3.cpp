// Leaf: ??0CManager@NUISystem@@IAE@XZ
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#include "engine/NUISystem_CObservable.h"
class CGraphicDataBank;
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#define FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY
#include "engine/CGraphicDataBankInit.h"
class CDisplayManager { public:
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bits, CPixelFormat& out);    // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bits, CPixelFormat& out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bits, CPixelFormat& out);              // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bits, CPixelFormat& out, bool flag);  // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bits, CPixelFormat& out);             // 0x009BE590
};
class CGraphicBankManager { public:
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0 (FableWin.pdb / Ego_r.pdb)
};
class CSystemManager { public:
    unsigned char        _pad00[0x6c];
    CGraphicBankManager* GraphicBankManager;    // +0x6c
};
class CNewFrontendGameComponent { public:
    unsigned char _pad00[9];
    bool          Active;                       // +0x09 (descriptive)
};
extern CDisplayManager* GPDisplayManager;      // 0x013B8390
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
void __fastcall SetIsUsingRelativeCoords(bool relative);   // 0x004299A8

#pragma optimize("y", off)
namespace NUISystem {
CManager::CManager()
    : m_pDefinitionManager(0), m_bHasErrorMessage(false), m_pErrorMessage(0), Restoring(false), m_fLastRenderTime(0.0),
      PMouseCursor(0), PDropped(0), DescriptionScrollingOffset(-1.0f), DescriptionScrollingToMax(false), MouseDisabled(false),
      AssignmentItemsExpressionsSwapping(false), AssignmentSpellsSwapping(false)
{
    PTCHeroExperience = 0;
    PAbilitiesInventory = 0;
    PWeaponsInventory = 0;
    PMapInventory = 0;
    PQuestInventory = 0;
    PExperienceInventory = 0;
    PInventory = 0;
    PInventoryClothing = 0;
    PInventoryTrade = 0;
    PPlayer = 0;
    SetIsUsingRelativeCoords(true);
    SetInput(UI_INPUT_KEYBOARD);
    PWeaponsInventory = 0;
    CGraphicDataBankInit params;
    GPDisplayManager->GetBestCompressedOpaqueTextureFormat(0x10, params.NonAlphaFormat);
    GPDisplayManager->GetBestCompressedAlphaTextureFormat(0x10, params.AlphaFormat);
    GPDisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, params.BooleanAlphaFormat);
    GPDisplayManager->GetBestAlphaTextureFormat(0x20, params.UncompressedAlphaFormat);
    GPDisplayManager->GetBestOpaqueTextureFormat(0x20, params.UncompressedNonAlphaFormat, false);
    GPDisplayManager->GetBestSignedTextureFormat(0x10, params.SignedFormat);
    params.MaxGraphicWidth = (unsigned long)-1;
    params.MaxGraphicHeight = (unsigned long)-1;
    params.GenerateMipmaps = false;
    params.AllowDither = false;
    if (g_pNewFrontendGameComponent && g_pNewFrontendGameComponent->Active) {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->OpenGraphicBank(CCharString("GBANK_FRONT_END"), params, true, true));
    } else {
        CGraphicBankManager* manager = GFGetSystemManager()->GraphicBankManager;
        SetGraphicsBank(manager->OpenGraphicBank(CCharString("GBANK_MAIN"), params, true, true));
    }
    SetMetaLayer(0);
}

}
#pragma optimize("y", on)
