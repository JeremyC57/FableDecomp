// Behaviour test for CWorld::GetPathNameForFileName (0x0049EF50): scan each top-level directory of the
// drive, split every entry's name, and return the first entry whose extension is empty and whose base
// name equals the requested name; otherwise the empty string.
#include <cstdio>

struct Entry { const char* name; bool isFile; const char* children[4]; };

static int StrEq(const char* a, const char* b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}

// SplitPath model: baseName = name up to the last '.', extension = text after it (empty if none).
static void SplitPath(const char* path, char* baseName, char* extension)
{
    const char* dot = 0;
    const char* p;
    for (p = path; *p; ++p)
        if (*p == '.') dot = p;
    int n = 0;
    for (p = path; *p && p != dot; ++p) baseName[n++] = *p;
    baseName[n] = 0;
    n = 0;
    if (dot)
        for (p = dot + 1; *p; ++p) extension[n++] = *p;
    extension[n] = 0;
}

static const char* GetPathNameForFileName(const Entry* root, int count, const char* fileName)
{
    for (int i = 0; i < count; ++i) {
        if (root[i].isFile)
            continue;
        for (int j = 0; j < 4 && root[i].children[j]; ++j) {
            char baseName[64];
            char extension[64];
            SplitPath(root[i].children[j], baseName, extension);
            if (StrEq(extension, "") && StrEq(baseName, fileName))
                return root[i].children[j];
        }
    }
    return "";
}

int main()
{
    Entry root[3] = {
        { "Data",     false, { "a.txt", "target", 0, 0 } },
        { "boot.ini", true,  { "hidden", 0, 0, 0 } },
        { "Levels",   false, { "target.lev", "other", "deep", 0 } },
    };
    int ok = 1;
    ok &= StrEq(GetPathNameForFileName(root, 3, "target"), "target");
    ok &= StrEq(GetPathNameForFileName(root, 3, "other"), "other");
    ok &= StrEq(GetPathNameForFileName(root, 3, "a"), "");        // has an extension
    ok &= StrEq(GetPathNameForFileName(root, 3, "hidden"), "");   // inside a file entry, never scanned
    ok &= StrEq(GetPathNameForFileName(root, 3, "missing"), "");
    if (!ok) { std::printf("0049ef50_TEST FAIL\n"); return 1; }
    std::printf("0049ef50_TEST PASS\n");
    return 0;
}