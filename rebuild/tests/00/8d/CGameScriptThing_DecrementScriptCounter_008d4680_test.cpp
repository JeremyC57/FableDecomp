// Test for 0x008D4680 CGameScriptThing::DecrementScriptCounter: the virtual drops ScriptCounter by one, also through a base pointer.
#include <stdio.h>
class CGameScriptThing {
public:
    virtual void DecrementScriptCounter() { --ScriptCounter; }
    unsigned char _pad_0x04[0x10];
    int ScriptCounter;
};
int main()
{
    CGameScriptThing t; t.ScriptCounter = 3;
    CGameScriptThing* p = &t;
    p->DecrementScriptCounter(); p->DecrementScriptCounter();
    bool ok = t.ScriptCounter == 1;
    t.ScriptCounter = 0; p->DecrementScriptCounter();
    if (t.ScriptCounter != -1) ok = false;
    puts(ok ? "008d4680_TEST PASS" : "008d4680_TEST FAIL");
    return ok ? 0 : 1;
}