// Leaf: ?GetEmptySlotName@CUserProfileManager@@QBE?AVCWideString@@XZ
// CUserProfileManager::GetEmptySlotName @ 0x00406a80  (__fastcall, returns CWideString by value)
// The game text bank (0x013B86A0 -> +0x14) turns the symbol (CCharString(const char*, long) 0x0099EBF0 on the literal
// at 0x0122DF80; ~CCharString 0x0099EAE0) into its text: NGameText::CDataBank::GetTextBySymbol 0x009C95E0 (FableWin
// `?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z`); without a bank, the wide literal at
// 0x0122DF5C (CWideString(const unsigned short*) 0x0099B6B0).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
};
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its text bank PTextBank +0x14 through GetTextBank(); was the stand-in SaveMgr* g_saveMgr.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "engine/CMainGameComponent.h"

class CUserProfileManager { public:
    CWideString GetEmptySlotName() const;
};

CWideString CUserProfileManager::GetEmptySlotName() const
{
    CMainGameComponent* mgr = CMainGameComponent::Get();
    if (mgr) {
        CCharString temp("TEXT_GUI_SAVE_EMPTY_SLOT", -1);
        return mgr->GetTextBank().GetTextBySymbol(temp);
    }
    return CWideString((const unsigned short*)L"<Empty Slot Name>");
}
