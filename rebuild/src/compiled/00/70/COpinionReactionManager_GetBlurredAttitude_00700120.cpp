// COpinionReactionManager::GetBlurredAttitude(EOpinionAttitudeType attitude) const -- pick one of the attitude's "blurred" alternatives at random with the world generator; the attitude itself when it has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700120, length 95 bytes (`ret 4`). Callees: CWorld::GetPRandomSeed
// @ 0x0049D810 (`[world+8] + 1613Ch`; the manifest's "CVertexBufferWin32::DoSizeof" is propagated), GFROR13 @ 0x00497890
// (__fastcall, one step of the world random generator as in the landed 0x006E8730).
// Layout: PDef (the def object pointer) at +0, World at +4 (PDB). COpinionReactionManagerDef::AttitudeBlur
// (vector<CBlurredAttitudesDef>) sits at retail +0x3C (PDB +0x40: the def base is 0x3C bytes in retail), and each
// CBlurredAttitudesDef is 0x48 bytes (PDB 0x50) with its Attitudes vector at +0x3C (`lea ebx,[edx+esi*8+3Ch]`, esi =
// attitude*9). Shape: the entry test is `size() > 0` (`sar 2 ; test ; jbe`) and the range a second size() read into
// a named unsigned local inside the block (one register for both, but VC7.1 no longer folds the helper's zero guard,
// so retail's `test esi,esi ; je` stays; a single count local tested and passed drops the guard, 79 bytes), the seed
// pointer a named local fetched after the count test, and the inlined helper written `range ? seed % range : 0`
// (the nonzero arm falls through; an `if (range == 0) return 0;` helper lays the zero arm first), with the element
// read duplicated into both arms of the guard.
// Leaf: GetBlurredAttitude
extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return range ? seed % range : 0;
}

enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };

struct CAttitudeVector {                           // retail std::vector<EOpinionAttitudeType>, 12 bytes
    EOpinionAttitudeType* m_pFirst;
    EOpinionAttitudeType* m_pLast;
    EOpinionAttitudeType* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    EOpinionAttitudeType operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CBlurredAttitudesDef { public:                      // 0x48 bytes in retail
    unsigned char   _base[0x3C];                   // def base
    CAttitudeVector Attitudes;                     // +0x3C
};

struct CBlurredAttitudesDefVector {                // retail std::vector<CBlurredAttitudesDef>
    CBlurredAttitudesDef* m_pFirst;
    CBlurredAttitudesDef* m_pLast;
    CBlurredAttitudesDef* m_pEnd;
    const CBlurredAttitudesDef& operator[](long index) const { return m_pFirst[index]; }
};

class COpinionReactionManagerDef { public:
    unsigned char              _base[0x3C];        // def base
    CBlurredAttitudesDefVector AttitudeBlur;       // +0x3C
};

class CWorld { public:
    unsigned long* GetPRandomSeed();               // 0x0049D810
};

class COpinionReactionManager {
public:
    const COpinionReactionManagerDef* PDef;        // +0x00 (CDefPointer: the object word)
    CWorld*                           World;       // +0x04

    EOpinionAttitudeType GetBlurredAttitude(EOpinionAttitudeType attitude) const;
};

EOpinionAttitudeType COpinionReactionManager::GetBlurredAttitude(EOpinionAttitudeType attitude) const
{
    const CAttitudeVector& blurred = PDef->AttitudeBlur[attitude].Attitudes;
    if (blurred.size() > 0) {
        unsigned long count = blurred.size();
        unsigned long* seed = World->GetPRandomSeed();
        return blurred[WorldRandomRange(*seed, count)];
    }
    return attitude;
}