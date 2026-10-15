// CThingCompare_BestForTargeting2::CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other) -- copy the targeting comparator (hero, vector comparator, enemy preference)
// Leaf: ??0CThingCompare_BestForTargeting2
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9730, 85 bytes.
// Identity: PDB CThingCompare_BestForTargeting2 (0x34 bytes) = Hero (CThing*) +0, VectorCompare
//   (CVectorCompare_BestForTargeting: AnchorPos, PreferredLine (C3DVector), AngleCompare, SqrDistCompare,
//   OverrideShortDistanceFactor, OverrideShortDistanceFalloff, CompareInXYPlane (bool)) +4, EnemyPreferenceFactor +0x30.
//   Called (by value copies of the comparator) from 14 sites in the targeting code (0x007FA276..0x0080C080).
// Shape: an out-of-line copy constructor (`mov eax,ecx` first, `ret 4`) copying every word through edx in member
//   order: a memberwise user copy constructor (EnemyPreferenceFactor assigned in the body after the record: in the
//   initialiser list it is stored first). The floats move as dwords (a user copy constructor copying a float
//   member goes through the x87), so the float members are modelled as dwords; the trailing bool as a byte.
struct CThing;

struct CVectorCompare_BestForTargeting {
    long AnchorPos[3];                  // +0x00 C3DVector
    long PreferredLine[3];              // +0x0c C3DVector
    long AngleCompare;                  // +0x18 float
    long SqrDistCompare;                // +0x1c float
    long OverrideShortDistanceFactor;   // +0x20 float
    long OverrideShortDistanceFalloff;  // +0x24 float
    bool CompareInXYPlane;              // +0x28
};

struct CThingCompare_BestForTargeting2 {
    CThing* Hero;                                       // +0x00
    CVectorCompare_BestForTargeting VectorCompare;      // +0x04
    long EnemyPreferenceFactor;                         // +0x30 float
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other);
};

CThingCompare_BestForTargeting2::CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other)
    : Hero(other.Hero)
{
    VectorCompare.AnchorPos[0] = other.VectorCompare.AnchorPos[0];
    VectorCompare.AnchorPos[1] = other.VectorCompare.AnchorPos[1];
    VectorCompare.AnchorPos[2] = other.VectorCompare.AnchorPos[2];
    VectorCompare.PreferredLine[0] = other.VectorCompare.PreferredLine[0];
    VectorCompare.PreferredLine[1] = other.VectorCompare.PreferredLine[1];
    VectorCompare.PreferredLine[2] = other.VectorCompare.PreferredLine[2];
    VectorCompare.AngleCompare = other.VectorCompare.AngleCompare;
    VectorCompare.SqrDistCompare = other.VectorCompare.SqrDistCompare;
    VectorCompare.OverrideShortDistanceFactor = other.VectorCompare.OverrideShortDistanceFactor;
    VectorCompare.OverrideShortDistanceFalloff = other.VectorCompare.OverrideShortDistanceFalloff;
    VectorCompare.CompareInXYPlane = other.VectorCompare.CompareInXYPlane;
    EnemyPreferenceFactor = other.EnemyPreferenceFactor;
}
