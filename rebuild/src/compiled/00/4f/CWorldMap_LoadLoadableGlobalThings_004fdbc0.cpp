// CWorldMap::LoadLoadableGlobalThings() -- walk every map after the first, show the "Loading global things" progress bar, and load the global things of each used map that loads on player proximity
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x004FDBC0, 233 bytes. PDB ?LoadLoadableGlobalThings@CWorldMap@@IAEXXZ (protected)
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 ("Loading global things" 0x012445D4),
// __fastcall NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40, ~CCharString
// 0x0099EAE0 (manifest "_Cons_val<...>"), CWorldMap::LoadLoadableGlobalThingsForLevel(long) 0x004FBF60.
// Float pool 0x0122DCB4 = 2^32 (unsigned-to-float fix-up).
// Retail CWorldMap offsets: Maps (CArray<CMapInfo>, 0x48-byte elements) +0x20; CMapInfo Used +0x24,
// LoadedOnPlayerProximity +0x28.
// Shape: the loop index is a signed long compared against the unsigned size() (`jb`), its float conversion goes
// through the frame; the progress text is an implicit CCharString conversion built before the other arguments.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMapInfo { public:                                            // 0x48 bytes
    C2DBoxI       Area;                                      // +0x00
    unsigned char _pad10[0x14];
    bool          Used;                                      // +0x24
    bool          Placed;                                    // +0x25
    bool          Activated;                                 // +0x26
    bool          Loading;                                   // +0x27
    bool          LoadedOnPlayerProximity;                   // +0x28
    unsigned char _rest[0x1f];                               // +0x29 .. +0x48
};
#include "engine/CArray.h"

class CWorldMap { public:
    unsigned char    _pad00[0x20];
    CArray<CMapInfo> Maps;                                   // +0x20
    void LoadLoadableGlobalThingsForLevel(long level);       // 0x004FBF60
    void LoadLoadableGlobalThings();
};

void CWorldMap::LoadLoadableGlobalThings()
{
    for (long i = 1; i < Maps.size(); ++i)
    {
        NProgressDisplay::DisplayProgress("Loading global things", (float)i / (Maps.size() + 1), false, false);
        const CMapInfo& map = Maps[i];
        if (map.Used && map.LoadedOnPlayerProximity)
            LoadLoadableGlobalThingsForLevel(i);
    }
}