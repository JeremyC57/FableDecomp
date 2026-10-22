// Behaviour test for CDefinitionManager::ParseStringForSymbols (0x009ADE90): with instantiation on, every
// "#definition_template" then every "#definition" pair is instantiated (failures reported); "enum" blocks are always
// parsed.
#include <cstdio>
#include <cstring>

struct Result { int templates, defs, enums, errors; };

static int Eq(const char* a, const char* b) { return std::strcmp(a, b) == 0; }

static Result Parse(const char* const* tokens, size_t count, bool instantiate)
{
    Result r = { 0, 0, 0, 0 };
    if (instantiate) {
        for (size_t i = 0; i + 2 < count + 1; ++i)
            if (Eq(tokens[i], "#definition_template") && i + 2 < count) {
                if (Eq(tokens[i + 1], "UNKNOWN")) ++r.errors; else ++r.templates;
            }
        for (size_t i = 0; i < count; ++i)
            if (Eq(tokens[i], "#definition") && i + 2 < count) {
                if (Eq(tokens[i + 1], "UNKNOWN")) ++r.errors; else ++r.defs;
            }
    }
    for (size_t i = 0; i < count; ++i)
        if (Eq(tokens[i], "enum"))
            ++r.enums;
    return r;
}

int main()
{
    const char* words[] = { "#definition_template", "CThingDef", "BASE", "#definition", "CThingDef", "SWORD",
                            "#definition", "UNKNOWN", "X", "enum", "E", "enum", "F" };
    size_t count = sizeof(words) / sizeof(words[0]);
    Result on = Parse(words, count, true);
    Result off = Parse(words, count, false);
    int ok = on.templates == 1 && on.defs == 1 && on.errors == 1 && on.enums == 2
          && off.templates == 0 && off.defs == 0 && off.errors == 0 && off.enums == 2;
    std::printf(ok ? "009ade90_TEST PASS\n" : "009ade90_TEST FAIL\n");
    return ok ? 0 : 1;
}