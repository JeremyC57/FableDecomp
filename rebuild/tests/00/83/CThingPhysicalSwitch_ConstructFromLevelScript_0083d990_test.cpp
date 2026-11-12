// Behaviour test for CThingPhysicalSwitch::ConstructFromLevelScript 0x0083D990: a copy of the function's logic over a small
// stand-alone model (string, tokenising parser, base-class result) checked against hand-computed expectations.
#include <cstdio>
#include <cstring>
#include <string>

struct Parser {
    const char* p;
    explicit Parser(const char* s) : p(s) {}
    void SkipSpace() { while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') ++p; }
    bool NextItemExists() { SkipSpace(); return *p != 0; }
    std::string ReadAsString() {
        SkipSpace();
        std::string r;
        if (*p == '"') { ++p; while (*p && *p != '"') r += *p++; if (*p) ++p; return r; }
        while (*p && *p != ' ' && *p != ';' && *p != '\n') r += *p++;
        return r;
    }
    bool SkipPastString(const std::string& s) {
        const char* f = std::strstr(p, s.c_str());
        if (!f) { p += std::strlen(p); return false; }
        p = f + s.size();
        return true;
    }
};

struct Switch {
    bool baseResult;
    int baseCalls;
    bool Triggered;
    std::string TriggerableObjectScriptName;
    bool BaseConstructFromLevelScript(const char*) { ++baseCalls; return baseResult; }
    bool ConstructFromLevelScript(const char* script) {
        if (BaseConstructFromLevelScript(script)) {
            Triggered = false;
            Parser parser(script);
            while (parser.NextItemExists()) {
                std::string key = parser.ReadAsString();
                if (key == "TriggeredByThing") {
                    std::string value = parser.ReadAsString();
                    TriggerableObjectScriptName = (value == "NULL") ? std::string("") : value;
                } else if (key == "EndThing") {
                    break;
                }
                parser.SkipPastString(";");
            }
            return true;
        }
        return false;
    }
};

static Switch Make(bool base) { Switch s; s.baseResult = base; s.baseCalls = 0; s.Triggered = true; s.TriggerableObjectScriptName = "old"; return s; }

int main()
{
    bool ok = true;
    // base fails: nothing touched, false
    { Switch s = Make(false); if (s.ConstructFromLevelScript("TriggeredByThing \"Door\";\nEndThing;") || !s.Triggered || s.TriggerableObjectScriptName != "old" || s.baseCalls != 1) ok = false; }
    // named triggerable object
    { Switch s = Make(true); if (!s.ConstructFromLevelScript("Player 4;\nTriggeredByThing \"Door\";\nEndThing;") || s.Triggered || s.TriggerableObjectScriptName != "Door") ok = false; }
    // "NULL" clears the name
    { Switch s = Make(true); if (!s.ConstructFromLevelScript("TriggeredByThing NULL;\nEndThing;") || s.Triggered || s.TriggerableObjectScriptName != "") ok = false; }
    // EndThing stops the parse: a later key is ignored
    { Switch s = Make(true); if (!s.ConstructFromLevelScript("EndThing;\nTriggeredByThing \"Late\";") || s.TriggerableObjectScriptName != "old" || s.Triggered) ok = false; }
    // unknown keys are skipped to the semicolon, last value wins, no EndThing needed
    { Switch s = Make(true); if (!s.ConstructFromLevelScript("Foo TriggeredByThing;\nTriggeredByThing \"A\";\nTriggeredByThing \"B\";") || s.TriggerableObjectScriptName != "B") ok = false; }
    // empty script: only Triggered is cleared
    { Switch s = Make(true); if (!s.ConstructFromLevelScript("") || s.Triggered || s.TriggerableObjectScriptName != "old") ok = false; }
    if (!ok) { std::printf("0083d990_TEST FAIL\n"); return 1; }
    std::printf("0083d990_TEST PASS\n");
    return 0;
}