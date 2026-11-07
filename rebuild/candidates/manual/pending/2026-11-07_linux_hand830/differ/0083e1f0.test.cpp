// Behaviour test for 0x0083E1F0 CThingScriptUIDReplacer::CThingScriptUIDReplacer: models the two passes the row makes over a
// level-script fragment (collect each NewThing block's non-zero UID and swap it for a placeholder token, then swap every token for
// a freshly allocated UID), keeping zero UIDs and repeated UIDs consistent.
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

static unsigned long g_next = 1000;
static unsigned long AllocateNewThingUID() { return g_next++; }

static std::string Replace(const std::string& text)
{
    std::string out = text;
    std::vector<std::string> uids;
    size_t pos = 0;
    while ((pos = out.find("UID ", pos)) != std::string::npos) {   // pass 1: UID -> token
        size_t start = pos + 4, end = out.find(';', start);
        std::string value = out.substr(start, end - start);
        if (value != "0") {
            size_t i = 0;
            while (i < uids.size() && uids[i] != value) i++;
            if (i == uids.size()) uids.push_back(value);
            char token[32]; sprintf(token, "#%u#", (unsigned)i);
            out.replace(start, end - start, token);
        }
        pos = start;
    }
    for (size_t i = 0; i < uids.size(); i++) {                       // pass 2: token -> new UID
        char token[32]; sprintf(token, "#%u#", (unsigned)i);
        char value[32]; sprintf(value, "%lu", AllocateNewThingUID());
        size_t p;
        while ((p = out.find(token)) != std::string::npos) out.replace(p, strlen(token), value);
    }
    return out;
}

int main()
{
    int ok = 1;
    ok &= Replace("NewThing A; UID 55; NewThing B; UID 0; NewThing C; UID 55;") == "NewThing A; UID 1000; NewThing B; UID 0; NewThing C; UID 1000;";
    ok &= Replace("UID 7; UID 8;") == "UID 1001; UID 1002;";
    ok &= Replace("no things") == "no things";
    if (ok) printf("0083e1f0_TEST PASS\n"); else printf("0083e1f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
