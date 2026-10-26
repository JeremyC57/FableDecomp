// CTCWife::GetAppearanceChangedNoticed(EWifeAppearanceCategories* category) -- among the appearance categories whose change the wife noticed within the def's notice window, report the one with the highest priority; true if any
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CED50, length 334 bytes (`ret 4`).
// Callees: GFGetCurrentFrame @ 0x0049D870 (static; `this` happens to be in ecx). Globals: g_FramesPerSecond @ 0x01375550,
//   the per-category priority table @ 0x0125ED00 (16 longs in .rdata; descriptive name).
// Layout: engine/CTCWife.h carries the PDB offsets (FrameNoticedChangeInAppearance +0xAC); retail's GiftsReceived map and
//   ThingsSeenThePlayerWear set are 12-byte STLport trees (PDB 16), so the array sits at +0xA4 in retail (the landed
//   ClearAppearanceChangedNoticed 0x006CECD0 stores to +0xA4 + 4*category). PHeroMarriageDef +0x0C is unshifted; its
//   +0x40 long is the notice window in seconds (descriptive name). Local model for that reason.
// Shape: a 16-pass counted loop the RTM compiler unrolls by 4 (pointer walks over the array and the table, the counter
//   kept 2 high); `best` in ecx, the bool in al, the window recomputed per pass; `best` is assigned before the
//   out-parameter store (the other order swaps eax/ecx in the second unrolled pass).
typedef long EWifeAppearanceCategories;

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                        // 0x0049D870
extern long g_FramesPerSecond;                              // 0x01375550
extern const long g_AppearanceCategoryPriority[16];         // 0x0125ED00

class CHeroMarriageDef { public:
    unsigned char _pad00[0x40];
    long AppearanceChangeNoticeSeconds;                     // +0x40
};

class CTCWife { public:
    void*             __vftable;                            // +0x00
    void*             ParentThing;                          // +0x04
    unsigned char     _pad_0x08[0x4];
    CHeroMarriageDef* PHeroMarriageDef;                     // +0x0c CDefPointer<const CHeroMarriageDef>::Object
    unsigned char     _pad_0x10[0xa4 - 0x10];
    long              FrameNoticedChangeInAppearance[16];   // +0xa4 (PDB +0xac)
    bool GetAppearanceChangedNoticed(EWifeAppearanceCategories* category);
};

bool CTCWife::GetAppearanceChangedNoticed(EWifeAppearanceCategories* category)
{
    long now = CWorld::GetFrame();
    long best = 0;
    bool found = false;
    for (long i = 0; i < 16; ++i) {
        long noticed = FrameNoticedChangeInAppearance[i];
        if (noticed > 0 && now - noticed < PHeroMarriageDef->AppearanceChangeNoticeSeconds * g_FramesPerSecond
            && g_AppearanceCategoryPriority[i] > best) {
            best = g_AppearanceCategoryPriority[i];
            *category = i;
            found = true;
        }
    }
    return found;
}