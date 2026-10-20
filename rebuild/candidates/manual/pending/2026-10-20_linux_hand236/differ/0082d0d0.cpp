// CThingBuilding::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const -- the physical-thing editor draw info, relabelled "<def name> - <script name>" and coloured blue (R 0, G 0, B 255, A 255)
// Grade: DIFFER(ours v retail) -- see twin 0x00830150 (same body, blue): CDefString temporary slot sharing (frame 0xc v 0x10) and the colour store order
// Leaf: ?DrawGetEditDrawInfo@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D0D0, 239 bytes (+17 int3), virtual (catalog "vfunc_25"), `ret 8`.
// Callees: CThingPhysical::DrawGetEditDrawInfo @ 0x006A4E90, CThing::GetDefinitionManager() @ 0x004C7990,
//   CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (CDefString by value), CDefStringTable::GetString(long)
//   @ 0x009D49B0 on the global string table 0x013CA828, CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
//   operator+(const CCharString&, const CCharString&) @ 0x0099F570, operator+(const CCharString&, const char*) @ 0x0099F600
//   (both __fastcall, hidden result in ecx; " - " @ 0x01264DC0), CCharString::operator= @ 0x0099EFB0, ~CCharString
//   @ 0x0099EAE0 (five temporaries).
// Layout: retail CThing ScriptName +0x74 (data pointer, Text first), DefGlobalIndex +0x8C; CEditEngineDrawInfo Fullname
//   +0x10, Colour (B,G,R,A) +0x14 (the landed CThingMarker::DrawGetEditDrawInfo 0x0083D070).
// Shape: one full expression `Fullname = CCharString("") + <def name> + " - " + ScriptName.c_str();` -- the def name is
//   the string table's text for the def's name position; c_str() is the inline null-first form with its own empty
//   literal (0x0129AAF4, a different pooled "" than the ctor's); the colour is the inline (r, g, b, a) setter.
struct CCharStringData { char* Text; };
extern const char g_szEmptyCString[];             // 0x0129AAF4 (CCharString::c_str's "")
struct CCharString {
    CCharStringData* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const char* c_str() const { return PData == 0 ? g_szEmptyCString : PData->Text; }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CDefString { long TablePos; };
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long globalIndex);   // 0x009ACCC0
};
struct CDefStringTable {
    CCharString GetString(long tablePos);        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;         // 0x013CA828
struct CRGBColour {
    unsigned char B;                            // +0
    unsigned char G;                            // +1
    unsigned char R;                            // +2
    unsigned char A;                            // +3
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};
struct C3DVector { float X, Y, Z; };
struct CEditEngineDrawInfo {
    C3DVector   Pos;                            // +0x00
    float       Radius;                         // +0x0c
    CCharString Fullname;                       // +0x10
    CRGBColour  Colour;                         // +0x14
    long        PlayerOwner;                    // +0x18
};
struct CThingPhysical {
    void*          __vftable;
    unsigned char  _pad04[0x74 - 0x04];
    CCharString    ScriptName;                  // +0x74 (retail)
    unsigned char  _pad78[0x8c - 0x78];
    unsigned short DefGlobalIndex;              // +0x8c
    CDefinitionManager* GetDefinitionManager() const;                        // 0x004C7990
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;   // 0x006A4E90
};
struct CThingBuilding : CThingPhysical {
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;
};

bool CThingBuilding::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const
{
    if (CThingPhysical::DrawGetEditDrawInfo(time, info)) {
        const CCharString& defName =
            g_DefStringTable.GetString(GetDefinitionManager()->GetDefNameFromGlobalIndex(DefGlobalIndex).TablePos);
        info->Fullname = CCharString("") + defName + " - " + ScriptName.c_str();
        info->Colour.Set(0, 0, 0xff, 0xff);
        return true;
    }
    return false;
}
