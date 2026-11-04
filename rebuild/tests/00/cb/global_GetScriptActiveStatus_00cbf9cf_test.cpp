// Behaviour test for GetScriptActiveStatus 0x00CBF9CF: returns the status recorded for the name in ScriptActiveStatuses (as
// SetScriptActiveStatus 0x00CBFAB8 stored it).
#include <cstdio>
#include <map>
#include <string>
enum EScriptActiveStatus { STATUS_A = 0, STATUS_B = 1, STATUS_C = 2 };
static std::map<std::string, EScriptActiveStatus> ScriptActiveStatuses;
static void SetScriptActiveStatus(const std::string& name, EScriptActiveStatus s) { ScriptActiveStatuses[name] = s; }
static EScriptActiveStatus GetScriptActiveStatus(const std::string& name) { return ScriptActiveStatuses.find(name)->second; }
int main()
{
    SetScriptActiveStatus("Quest1", STATUS_B); SetScriptActiveStatus("Quest2", STATUS_C);
    if (GetScriptActiveStatus("Quest1") != STATUS_B || GetScriptActiveStatus("Quest2") != STATUS_C) { std::printf("00cbf9cf_TEST FAIL\n"); return 1; }
    SetScriptActiveStatus("Quest1", STATUS_A);
    if (GetScriptActiveStatus("Quest1") != STATUS_A) { std::printf("00cbf9cf_TEST FAIL\n"); return 1; }
    std::printf("00cbf9cf_TEST PASS\n");
    return 0;
}