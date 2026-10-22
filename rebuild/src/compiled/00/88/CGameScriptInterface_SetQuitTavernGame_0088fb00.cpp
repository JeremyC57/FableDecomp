extern unsigned char g_quitTavernGame;

void __stdcall CGameScriptInterface_SetQuitTavernGame(bool quit)
{
    g_quitTavernGame = (unsigned char)quit;
}