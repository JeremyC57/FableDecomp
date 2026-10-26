extern const char kEmptyDisplayNameTag[];

class CCharString { public:
    char* data;
    bool Compare(const char* s) const;
};

class CRegion { public:
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

CCharString* __fastcall CRegion_GetDisplayName(CRegion* region)
{
    CCharString* pB = &region->nameB;
    if (pB->Compare(kEmptyDisplayNameTag))
        return pB;
    return &region->nameA;
}