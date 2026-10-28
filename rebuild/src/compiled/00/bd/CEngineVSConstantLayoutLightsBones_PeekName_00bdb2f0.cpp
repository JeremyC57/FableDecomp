// CEngineVSConstantLayoutLightsBones::PeekName @ 00bdb2f0
// Real __fastcall member returning CCharString by value: this in ecx (unused),
// hidden UDT return pointer on the stack ([esp+8] after push esi), ret 4.
// Retail: push esi; mov esi,[esp+8]; push -1; push offset "..."; mov ecx,esi;
//         call CCharString::ctor(const char*,long); mov eax,esi; pop esi; ret 4
// The layout name string lives at 0x12a7d0c (pushed as an immediate offset).

class CCharString { public:
    int d;
    CCharString(const char* s, long n); // ctor @ 0x99ebf0
};

extern char kLayoutName[];

class CEngineVSConstantLayoutLightsBones { public:
    int x;
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutLightsBones::PeekName()
{
    return CCharString(kLayoutName, -1);
}