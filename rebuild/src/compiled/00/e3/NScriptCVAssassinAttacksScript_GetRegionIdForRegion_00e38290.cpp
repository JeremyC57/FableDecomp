// CV_AssassinAttacksScript::GetRegionIdForRegion(CCharString) -- index (0..4) of the region-info entry whose name equals the given region name, 5 when none does
// Retail: Fable.exe @ 0x00E38290, 48 bytes, `ret 4`. Called from 0x00E3815D and 0x00E3944A.
// Callees: CCharString::operator!=(const CCharString&) @ 0x0099E900 (landed), CCharString::~CCharString @ 0x0099EAE0
// (the by-value argument, destroyed by the callee).
// Layout: Ego_r.pdb CV_AssassinAttacksScript - 0x14: RegionInfo (RegionData[5], 16 bytes each, name first) at +0x58.
// Shape: `for (i = 0; i < 5; ++i) if (!(RegionInfo[i].name != region)) break;` -- bottom-tested (the first test is
// folded), the element pointer strength-reduced into edi; the parameter's address goes to operator!= each time and
// its destructor runs before the return.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharStringData;

struct CCharString {
    CCharStringData* m_pData;
    ~CCharString();                                      // 0x0099EAE0
    bool operator!=(const CCharString& other) const;     // 0x0099E900
};

struct RegionData {
    CCharString name;                                    // +0x00
    long count;                                          // +0x04
    long max;                                            // +0x08
    bool assassinActive;                                 // +0x0c
};

namespace NScript {
class CV_AssassinAttacksScript {
public:
    unsigned char _base[0x58];
    RegionData RegionInfo[5];                            // +0x58
    long GetRegionIdForRegion(CCharString region);
};
} // namespace NScript
using NScript::CV_AssassinAttacksScript;

long NScript::CV_AssassinAttacksScript::GetRegionIdForRegion(CCharString region)
{
    long i;
    for (i = 0; i < 5; ++i) {
        if (!(RegionInfo[i].name != region))
            break;
    }
    return i;
}