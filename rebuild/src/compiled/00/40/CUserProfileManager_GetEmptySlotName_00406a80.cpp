// CUserProfileManager::GetEmptySlotName @ 0x00406a80  (__fastcall, returns CWideString by value)
namespace N_0099eae0 { struct CWideString {
    void* p;
    CWideString(const char* s, int len);   // 0x99ebf0
    CWideString(const char* s);             // 0x99b6b0
    ~CWideString();                         // 0x99eae0
}; } using N_0099eae0::CWideString;
struct SaveSub {
    CWideString Format(CWideString* in);    // 0x9c95e0 (thiscall, struct-return)
};
struct SaveMgr {
    char pad[0x14];
    SaveSub* f14;                           // +0x14
};
extern SaveMgr* g_saveMgr;                  // [0x13b86a0]

struct CUserProfileManager {
    CWideString GetEmptySlotName();
};

CWideString CUserProfileManager::GetEmptySlotName()
{
    SaveMgr* mgr = g_saveMgr;
    if (mgr) {
        CWideString temp((const char*)0x122df80, -1);
        return mgr->f14->Format(&temp);
    }
    return CWideString((const char*)0x122df5c);
}