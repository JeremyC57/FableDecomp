// CTavernGameDef::CTavernGameDef() -- base CSubDefClassBase(), own vtable, the four CDefString members = -1, the PrizeScores vector empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1049, length 61 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (vtable 0x01231D54; the manifest's
// "CShoveHaPennyRegionDef" label is propagated). Vtable 0x012424BC = CTavernGameDef; the
// "CCombatAbilityStrafeDef" label on this body is propagated too.
// PDB CTavernGameDef (0x1a8 bytes) agrees with retail up to the vector: 26 phrase ids +0x28..+0x90,
// CameraName (CDefString) +0x90, ScoreFont/TargetFont/StatsFont +0x11c/+0x120/+0x124, PrizeScores
// (vector<CPrizeScoreDef>, 12 bytes in retail) +0x174. The four def strings share one `or eax,-1`
// and the vector's inline constructor zeroes its three pointers through `lea eax,[esi+174h]` -- the
// size optimiser hoists the far base address into a register (`#pragma optimize("s", on)`).
#pragma optimize("s", on)
struct CSubDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                              // +0x04 .. +0x28
    CSubDefClassBase();                                     // 0x0044C0C0
};

struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CPrizeScoreDef;

template<class T> struct CRetailVector {                    // retail std::vector: {first, last, end}
    T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
};

struct CTavernGameDef : CSubDefClassBase {
    unsigned long Banter;                                   // +0x28 (26 phrase/graphic ids follow)
    unsigned char _ids_0x2c[0x64];                          // +0x2c .. +0x90
    CDefString    CameraName;                               // +0x90
    unsigned char _ids_0x94[0x88];                          // +0x94 .. +0x11c (box graphics, phrases, bets)
    CDefString    ScoreFont;                                // +0x11c
    CDefString    TargetFont;                               // +0x120
    CDefString    StatsFont;                                // +0x124
    unsigned char _layout_0x128[0x4c];                      // +0x128 .. +0x174 (positions, BestScoreHigh)
    CRetailVector<CPrizeScoreDef> PrizeScores;              // +0x174
    long          Prize;                                    // +0x180
    unsigned char _tail_0x184[0x24];                        // +0x184 .. +0x1a8
    virtual void v00();                                     // own vtable 0x012424BC
    CTavernGameDef();
};

CTavernGameDef::CTavernGameDef()
    : CSubDefClassBase()
    , CameraName()
    , ScoreFont()
    , TargetFont()
    , StatsFont()
    , PrizeScores()
{
}