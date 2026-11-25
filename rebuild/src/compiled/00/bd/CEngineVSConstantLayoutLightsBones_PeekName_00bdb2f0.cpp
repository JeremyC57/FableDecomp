// CEngineVSConstantLayoutLightsBones::PeekName @ 00bdb2f0
// Real __fastcall member returning CCharString by value: this in ecx (unused),
// hidden UDT return pointer on the stack ([esp+8] after push esi), ret 4.
// Retail: push esi; mov esi,[esp+8]; push -1; push offset "..."; mov ecx,esi;
//         call CCharString::ctor(const char*,long); mov eax,esi; pop esi; ret 4
// The layout name string lives at 0x12a7d0c (pushed as an immediate offset).
// Literal (NF418, 2026-11-25): was extern data `kLayoutName` (table ?kLayoutName@@3PADA -> 0x012A7D0C); now spelled at the use site:
//   "LayoutLightsBones", retail's pooled literal at 0x012A7D0C (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).

class CCharString { public:
    int d;
    CCharString(const char* s, long n); // ctor @ 0x99ebf0
};


class CEngineVSConstantLayoutLightsBones { public:
    int x;
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutLightsBones::PeekName()
{
    return CCharString("LayoutLightsBones", -1);
}