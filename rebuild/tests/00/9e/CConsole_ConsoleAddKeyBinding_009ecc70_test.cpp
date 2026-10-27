// Test for 0x009ECC70 CConsole::ConsoleAddKeyBinding ("BindKey"): binds the first character of argument 0 to the
// command text of argument 1 (an empty key string binds '\0'); rebinding replaces the old command.
#include <stdio.h>
#include <string.h>
class CCharString {
public:
    const char* Data;
    const char* GetAsCharArray() const { return Data == 0 ? "" : Data; }
};
class CGenericVar { public: CCharString Str; const CCharString& GetString() const { return Str; } };
class CConsole {
public:
    static void ConsoleAddKeyBinding(CConsole& console, const CGenericVar* args)
    { console.AddKeyBinding(args[0].GetString().GetAsCharArray()[0], args[1].GetString()); }
    const char* KeyBindingMap[256];
private:
    void AddKeyBinding(char key, const CCharString& binding) { KeyBindingMap[(unsigned char)key] = binding.GetAsCharArray(); }
};
int main()
{
    CConsole c; memset(c.KeyBindingMap, 0, sizeof(c.KeyBindingMap));
    CGenericVar args[2]; args[0].Str.Data = "q"; args[1].Str.Data = "quit";
    CConsole::ConsoleAddKeyBinding(c, args);
    bool ok = c.KeyBindingMap['q'] && strcmp(c.KeyBindingMap['q'], "quit") == 0;
    args[1].Str.Data = "quicksave"; CConsole::ConsoleAddKeyBinding(c, args);
    if (strcmp(c.KeyBindingMap['q'], "quicksave") != 0) ok = false;
    args[0].Str.Data = 0; args[1].Str.Data = "nothing"; CConsole::ConsoleAddKeyBinding(c, args);
    if (!c.KeyBindingMap[0] || strcmp(c.KeyBindingMap[0], "nothing") != 0) ok = false;
    puts(ok ? "009ecc70_TEST PASS" : "009ecc70_TEST FAIL");
    return ok ? 0 : 1;
}