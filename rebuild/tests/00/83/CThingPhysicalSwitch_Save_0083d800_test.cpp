// Behaviour test for CThingPhysicalSwitch::Save (0x0083D800): after the base text, append
// "" + "TriggeredByThing " + (name empty ? "NULL" : name) + ";\r\n".
#include <cstdio>
#include <cstring>

struct Switch {
    bool Triggered;
    const char* TriggerableObjectScriptName;   // 0 = the empty CCharString (null block)
};

static void BaseSave(char* out) { std::strcat(out, "CThingPhysical;\r\n"); }   // stands in for 0x006A4BA0

static void Save(const Switch& s, char* out)
{
    BaseSave(out);
    char line[256];
    std::strcpy(line, "");
    std::strcat(line, "TriggeredByThing ");
    const char* name = s.TriggerableObjectScriptName;
    bool empty = (name == 0) ? (std::strcmp("", "") == 0) : (std::strcmp(name, "") == 0);
    std::strcat(line, empty ? "NULL" : name);
    std::strcat(line, ";\r\n");
    std::strcat(out, line);
}

int main()
{
    int bad = 0;
    char out[512];
    Switch a = { false, 0 };
    out[0] = 0; Save(a, out);
    if (std::strcmp(out, "CThingPhysical;\r\nTriggeredByThing NULL;\r\n") != 0) bad |= 1;
    Switch b = { true, "" };
    out[0] = 0; Save(b, out);
    if (std::strcmp(out, "CThingPhysical;\r\nTriggeredByThing NULL;\r\n") != 0) bad |= 2;
    Switch c = { false, "DoorLever01" };
    out[0] = 0; Save(c, out);
    if (std::strcmp(out, "CThingPhysical;\r\nTriggeredByThing DoorLever01;\r\n") != 0) bad |= 4;
    if (bad) { std::printf("0083d800_TEST FAIL\n"); return 1; }
    std::printf("0083d800_TEST PASS\n");
    return 0;
}