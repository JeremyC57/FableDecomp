struct CResult {
    bool Method2(int n);
};
class CPlayer;
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
struct CSub {

};
struct CGlobal {
    char pad[0x1c];
    CSub* sub;   // at +0x1c
};

extern CGlobal* g_global;

int __fastcall CGameScriptInterface_IsHeroInTavernGame(void* self)
{
    (void)self;
    CResult* r = ((CResult*)&((CPlayerManager*)g_global->sub)->GetMainPlayer());
    if (r && !r->Method2(0x1a))
        return 0;
    return 1;
}