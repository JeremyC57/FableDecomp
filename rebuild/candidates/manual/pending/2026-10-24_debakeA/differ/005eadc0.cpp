// Grade: DIFFER(22v22) -- IMPOSSIBLE as catalogued (row mis-cut): the 22 bytes 0x005EADC0..0x005EADD5 are only the /GS prologue prefix (`sub esp,34h ; mov eax,[__security_cookie 0139C8A8h] ; push ebx ; mov [esp+34h],eax ; mov al,[013BAE1Ch] ; xor ebx,ebx ; cmp al,bl`) of a 354-byte CTCTavernGame member that runs to `call __security_check_cookie ; add esp,34h ; ret 1Ch` at 0x005EAF1F (seven stack arguments, four by-value CWideStrings destroyed in the callee; checked with dx.py on Fable.exe). The prefix has no `ret` and cannot be a compiled function on its own; re-cut the row to 354 bytes (and grade with /GS) before reconstructing. This stand-in only models the first test. One grade.
// CTCTavernGame::SetStateAfterYesNo -- first 22 bytes of a 354-byte /GS function (catalog row mis-cut)
// Leaf: ?SetStateAfterYesNo@CTCTavernGame
extern bool g_tavernYesNoPending;            // 0x013BAE1C
struct CTCTavernGame
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void CancelGame();               // slot 30 (+0x78)
    void SetStateAfterYesNo();
};
void CTCTavernGame::SetStateAfterYesNo()
{
    char buffer[0x20];
    buffer[0] = 0;
    if (g_tavernYesNoPending)
    {
        g_tavernYesNoPending = false;
        CancelGame();
    }
}
