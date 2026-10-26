// Test for 0x0044C6AF CGameDefinitionManager::ErrorCallback: callable through the callback pointer the constructor
// installs, and it has no effect.
#include <stdio.h>
class CCharString { public: const char* Text; };
class CGameDefinitionManager {
    friend int main();
protected:
    static void __fastcall ErrorCallback(const CCharString& error);
};
typedef void (__fastcall* TErrorCallback)(const CCharString&);
int main()
{
    TErrorCallback cb = &CGameDefinitionManager::ErrorCallback;
    CCharString s; s.Text = "Def error";
    cb(s);
    CGameDefinitionManager::ErrorCallback(s);
    if (s.Text[0] != 'D') { printf("0044c6af_TEST FAIL\n"); return 1; }
    printf("0044c6af_TEST PASS\n");
    return 0;
}
