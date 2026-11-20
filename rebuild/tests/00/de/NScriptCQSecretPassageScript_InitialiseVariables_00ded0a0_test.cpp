// Test for 0x00DED0A0 NScript::CQ_SecretPassageScript::InitialiseVariables (links the row object; the class from the shared header).
// The script is never constructed here: the test fills raw storage of the script's size with 0xAA and checks that exactly the seven
// quest flags (+0x48..+0x4D, +0x4F) are cleared and OkayToSpawn (+0x4E) and the neighbouring bytes are left alone.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/NScript_CQ_SecretPassageScript.h"

int main()
{
    static double storage[sizeof(NScript::CQ_SecretPassageScript) / sizeof(double) + 1];
    unsigned char* bytes = reinterpret_cast<unsigned char*>(storage);
    memset(bytes, 0xAA, sizeof(storage));
    NScript::CQ_SecretPassageScript* script = reinterpret_cast<NScript::CQ_SecretPassageScript*>(bytes);
    script->InitialiseVariables();
    int ok = 1;
    for (int i = 0x40; i < 0x58; ++i) {
        int cleared = (i >= 0x48 && i <= 0x4D) || i == 0x4F;
        if (cleared ? bytes[i] != 0 : bytes[i] != 0xAA) ok = 0;
    }
    if (script->MissionSucceeded || script->QuestStartScreenShown) ok = 0;
    if (ok)
        printf("00ded0a0_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}
