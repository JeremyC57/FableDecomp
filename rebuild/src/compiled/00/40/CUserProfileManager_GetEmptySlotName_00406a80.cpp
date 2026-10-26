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
struct SaveMgr {
    char pad[0x14];
    NGameText::CDataBank* f14;              // +0x14
};
extern SaveMgr* g_saveMgr;                  // [0x13b86a0]

class CUserProfileManager { public:
    CWideString GetEmptySlotName();
};

CWideString CUserProfileManager::GetEmptySlotName()
{
    SaveMgr* mgr = g_saveMgr;
    if (mgr) {
        CCharString temp((const char*)0x122df80, -1);
        return mgr->f14->GetTextBySymbol(temp);
    }
    return CWideString((const unsigned short*)0x122df5c);
}
