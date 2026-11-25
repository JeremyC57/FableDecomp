class CCharString { public:
    char* data;
    bool operator!=(const char* s) const;   // 0x0099E960; NF303: PDB name, was Compare
};

class CRegion { public:
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

// Literal (NF413, 2026-11-25): HEAD compared against an extern data symbol that reloc_bindings.tsv bound to 0x0122D70E, retail's one
//   pooled "" (00 at 0x0122D70E, NUL-preceded); it is now that literal.
CCharString* __fastcall CRegion_GetDisplayName(CRegion* region)
{
    CCharString* pB = &region->nameB;
    if (*pB != "")
        return pB;
    return &region->nameA;
}