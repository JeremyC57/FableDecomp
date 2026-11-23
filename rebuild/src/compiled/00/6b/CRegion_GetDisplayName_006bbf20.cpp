extern const char kEmptyDisplayNameTag[];

class CCharString { public:
    char* data;
    bool operator!=(const char* s) const;   // 0x0099E960; NF303: PDB name, was Compare
};

class CRegion { public:
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

CCharString* __fastcall CRegion_GetDisplayName(CRegion* region)
{
    CCharString* pB = &region->nameB;
    if (*pB != kEmptyDisplayNameTag)
        return pB;
    return &region->nameA;
}