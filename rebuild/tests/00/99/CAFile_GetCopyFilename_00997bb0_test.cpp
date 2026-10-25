// Behaviour test for 0x00997BB0 CAFile::GetCopyFilename: the extension (from the last '.') is kept, a trailing number
// is bumped by one ("save19.sav" -> "save20.sav", "007.x" -> "008.x") and a name without one (or ending in 0) gets "2".
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>

static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static bool IsNumber(const std::string& s) { return !s.empty() && s[0] >= '0' && s[0] <= '9'; }

static std::string GetCopyFilename(const std::string& filename)
{
    std::string name(filename);
    std::string extension("");
    long length = (long)filename.size();
    for (long i = 1; i <= length; ++i) {
        if (name[length - i] == '.') {
            name = filename.substr(0, length - i);
            extension = filename.substr(length - i);
            break;
        }
    }
    long nameLength = (long)name.size();
    long number = 0;
    for (long digits = 1; digits <= nameLength; ++digits) {
        std::string character = name.substr(nameLength - digits, 1);
        if (!IsNumber(character))
            break;
        number += (long)(std::atoi(character.c_str()) * std::pow(10.0f, (float)(digits - 1)));
    }
    if (number == 0)
        return name + IntToString(2) + extension;
    std::string numberString = IntToString(number);
    std::string copyName = name.substr(0, nameLength - numberString.size());
    copyName += IntToString(number + 1);
    return copyName + extension;
}

int main()
{
    int ok = 1;
    ok &= GetCopyFilename("save.sav") == "save2.sav";
    ok &= GetCopyFilename("save3.sav") == "save4.sav";
    ok &= GetCopyFilename("save9.sav") == "save10.sav";
    ok &= GetCopyFilename("save19.sav") == "save20.sav";
    ok &= GetCopyFilename("007.x") == "008.x";
    ok &= GetCopyFilename("save0.sav") == "save02.sav";
    ok &= GetCopyFilename("readme") == "readme2";
    ok &= GetCopyFilename("a.b.c") == "a.b2.c";
    std::printf("00997bb0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}