// GetSizeVal(const char*, const char*) -- (anonymous namespace) parse "<digits> [spaces] [k|m]" into a byte count (k = 1024, m = 1048576)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F1C80, 147 bytes (160 with padding). PDB: ?GetSizeVal@?A0xcf4a15dd@@YIIPBD0@Z
//   (anonymous namespace, `unsigned int __fastcall GetSizeVal(const char*, const char*)`; the catalog name is right).
//   The leaf is the `?GetSizeVal@` prefix because the anonymous-namespace tag is a per-file hash.
// Callees: isdigit 0x00BFEDA6, isspace 0x00BFEDBE, tolower 0x00BFED10 (MSVCR71 imports, the char sign-extended).
// Shape: two `while (p < end && pred(*p))` loops (each loop's own p < end entry test survives) then the suffix: 'k'
//   returns value << 10 from its own epilogue, 'm' shifts by 20; the digit step is `lea ecx,[v+v*4] ; lea v,[c+ecx*2-30h]`.
// Leaf: ?GetSizeVal@
extern "C" int __cdecl isdigit(int c);              // 0x00BFEDA6 (MSVCR71)
extern "C" int __cdecl isspace(int c);              // 0x00BFEDBE (MSVCR71)
extern "C" int __cdecl tolower(int c);              // 0x00BFED10 (MSVCR71)

namespace {

unsigned int __fastcall GetSizeVal(const char* p, const char* end)
{
    unsigned int value = 0;
    while (p < end && isdigit(*p)) {
        value = value * 10 + (*p - '0');
        ++p;
    }
    while (p < end && isspace(*p))
        ++p;
    if (p < end) {
        if (tolower(*p) == 'k')
            value <<= 10;
        else if (tolower(*p) == 'm')
            value <<= 20;
    }
    return value;
}

}
