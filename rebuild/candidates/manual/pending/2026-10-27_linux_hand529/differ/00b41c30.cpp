// CEngineMap::PeekInterpolatedWaterDepth(long x, long y, long radius) const -- average water depth (over 0.001) of the square of cells within radius of (x, y); when (x, y) itself is dry, the square is re-centred on the first wet cell found inside it (scanning x outer, y inner), and 0 is returned when there is none
// Grade: DIFFER(377v377) -- 4 grades; control flow, frame and stack homes match; the residue is register scheduling: the prologue builds x + radius in ecx (retail: ebp, reading y first) and the re-centring block uses eax / ecx / edx where retail uses ecx / edx (and eax for the call argument). Grade 1 (if / return pair at the end) merged the two `return 0.0f` blocks (362v377); the ternary return fixed that; grade 3 (re-centring by assigning x, y) was worse (327v377); grade 4 (max-before-min order) kept the same residue. Next idea: bounds as a local struct / C2DCoordI pair, or the found flag kept as the loop condition variable.
// Leaf: ?PeekInterpolatedWaterDepth@CEngineMap@@QBEMJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41C30, 377 bytes, thiscall `ret 0Ch`, float result in st(0).
// Identity (proven): Ego_r.pdb ?PeekInterpolatedWaterDepth@CEngineMap@@QBEMJJJ@Z (const, three longs, float -- the row's
//   three stack arguments and x87 result); its callees are the Ego_r CEngineMap members PeekHasWaterFast(long, long) const
//   0x00B40260 and PeekWaterDepth(long, long) const 0x00B40BD0 (their siblings PeekInterpolatedHasWaterFast /
//   PeekInterpolatedWaterType take the same (x, y, radius)). No callers in retail (an emitted out-of-line copy).
// Shape: one ternary return at the end, so the not-found `return 0.0f` keeps its own epilogue as in retail.
// Globals: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.001f 0x012A1ED8.
class CEngineMap {
public:
    bool PeekHasWaterFast(long x, long y) const;           // 0x00B40260
    float PeekWaterDepth(long x, long y) const;            // 0x00B40BD0
    float PeekInterpolatedWaterDepth(long x, long y, long radius) const;
};

float CEngineMap::PeekInterpolatedWaterDepth(long x, long y, long radius) const
{
    long minX = x - radius;
    long maxX = x + radius;
    long minY = y - radius;
    long maxY = y + radius;
    if (!PeekHasWaterFast(x, y)) {
        bool found = false;
        for (long i = minX; i <= maxX && !found; ++i) {
            for (long j = minY; j <= maxY && !found; ++j) {
                if (PeekHasWaterFast(i, j)) {
                    minX = i - radius;
                    maxX = i + radius;
                    minY = j - radius;
                    maxY = j + radius;
                    found = true;
                }
            }
        }
        if (!found)
            return 0.0f;
    }
    float total = 0.0f;
    float count = 0.0f;
    for (long i = minX; i <= maxX; ++i) {
        for (long j = minY; j <= maxY; ++j) {
            float depth = PeekWaterDepth(i, j);
            if (depth > 0.001f) {
                total += depth;
                count += 1.0f;
            }
        }
    }
    return count > 0.0f ? total / count : 0.0f;
}
