// CPhysicsMesh::CalculateNumberOfNavigationLineLevels() -- the highest i in 1..9 for which a helper point named "NAV_LAYER_0<i>" exists (0 when none)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A73DB0, length 153 bytes.
// Leaf: CalculateNumberOfNavigationLineLevels
// Callees: GFIntToCharString(long) @ 0x0099F830 and operator+(const char*, const CCharString&) @ 0x0099F690 (both
// __fastcall: hidden result in ecx), ~CCharString @ 0x0099EAE0 (x2), CVectorMap<CCharString,C3DHelperPoint>::LowerBound
// @ 0x00A6F290, CCharString::FastCompare @ 0x009E6530 (the CCompareString "less"). Literal "NAV_LAYER_0" @ 0x0129DC80.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 16-byte vector maps: HelperPoints +0x10 (vector first/last +0x10/+0x14).
// Shape: a zero-based 9-pass loop using `i + 1` (VC keeps i+1 in ebx and turns i into a count-down frame counter; a
// 1..9 loop compares ebx with 10 instead); `this` spilled to the frame, the map kept in ebp
// ; the number string is a temporary destroyed before the lookup, the name a loop-scope local destroyed at
// the end of each pass; the inline map Find as the landed CSymbolMap::FindSymbol 0x009E51C0, compared with the member
// end() re-read through `this`; `ret`.
class CCharStringData;
class CCharString { public:
    CCharStringData* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class C3DHelperPoint { public: float m[12]; };
struct SHelperPointPair {                          // pair<CCharString, C3DHelperPoint>
    CCharString    first;
    C3DHelperPoint second;
};
struct CHelperPointMap {                           // CVectorMap<CCharString, C3DHelperPoint, CCompareString<...>>
    SHelperPointPair* m_pFirst;                    // +0x00
    SHelperPointPair* m_pLast;                     // +0x04
    SHelperPointPair* m_pEnd;                      // +0x08
    long              m_compare;                   // +0x0c
    SHelperPointPair* LowerBound(const CCharString& key) const;   // 0x00A6F290
    SHelperPointPair* end() const { return m_pLast; }
    SHelperPointPair* Find(const CCharString& key) const
    {
        SHelperPointPair* it = LowerBound(key);
        SHelperPointPair* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

class CPhysicsMesh { public:
    unsigned char   _pad00[0x10];
    CHelperPointMap HelperPoints;                  // +0x10
    long CalculateNumberOfNavigationLineLevels() const;
};

long CPhysicsMesh::CalculateNumberOfNavigationLineLevels() const
{
    long levels = 0;
    for (long i = 0; i < 9; ++i) {
        CCharString name = "NAV_LAYER_0" + GFIntToCharString(i + 1);
        if (HelperPoints.Find(name) != HelperPoints.end())
            levels = i + 1;
    }
    return levels;
}