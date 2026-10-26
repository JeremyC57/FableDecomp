// CAFile::IsForbiddenSymbolInComponent(const CCharString& component) -- true when the path component contains any of the seven characters < > : " / \ |
// Leaf: ?IsForbiddenSymbolInComponent@CAFile@@CI_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009975E0, length 55 bytes (+ int3 padding), private static fastcall.
// Data: the seven forbidden characters @ 0x0129A79C ("<>:" 0x93 "/\\|" -- 0x93 is the retail build's '"' as
// stored, a curly-quote byte from the source file's code page).
// Callee: CCharString::FindCharPos(char, long start = 0) const @ 0x0099E5A0 (one stack argument, retail pushes only the char:
// the manifest's "FindCharPos"; a -1 return means absent).
#include <stddef.h>

class CCharString
{
public:

private:
    void* Data;
public:
    long FindCharPos(char) const;   // 0x0099E5A0
};

static const char ForbiddenSymbols[] = "<>:\x93/\\|";   // 0x0129A79C

class CAFile
{
private:
    static bool __fastcall IsForbiddenSymbolInComponent(const CCharString& component);
};

bool __fastcall CAFile::IsForbiddenSymbolInComponent(const CCharString& component)
{
    bool forbidden = false;
    for (long i = 0; i < 7; ++i)
    {
        if (component.FindCharPos(ForbiddenSymbols[i]) > -1)
        {
            forbidden = true;
            break;
        }
    }
    return forbidden;
}