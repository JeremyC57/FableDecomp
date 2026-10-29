struct TutMgr { char pad0[0xc]; char flagC; char pad1[0x17-0xd]; char flag17; char pad2[0xd7-0x18]; char flagD7; };
class CUserProfileManager : public TutMgr { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ

char __fastcall CGameScriptInterface_IsTutorialSystemEnabled(void* self)
{
    TutMgr* p = &CUserProfileManager::Get();
    if (p->flagC)
        return p->flagD7;
    return p->flag17;
}