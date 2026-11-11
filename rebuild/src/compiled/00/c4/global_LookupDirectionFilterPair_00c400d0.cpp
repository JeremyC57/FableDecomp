// LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first, const SDirectionFilter** second) -- libpk audio: pick the direction-filter table for the output mode (1: the second table; 4: the first; 8 and 64: the first, with the azimuth folded into -90..90 front/back), look up the (azimuth, elevation) entry and return it and the record after it, swapped for a negative azimuth; any other mode returns nothing
// Leaf: ?LookupDirectionFilterPair@@YAXJMMPAPBUSDirectionFilter@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C400D0, 175 bytes of code + a 4-entry jump table (0x00C40180) + a 64-entry byte index
//   table (0x00C40190): 256 bytes up to the next function 0x00C401D0 (SPLITS.tsv; the catalog's 528 runs over eight small
//   vector helpers 0x00C401D0..0x00C402C0). __cdecl, void.
// Identity (descriptive; libpk has no PDB/RTTI): the landed callers are the multi-tap delay effects' Update slot 4
//   (0x00C6D150 calls it LookupDirectionFilterPair; 0x00C71270 / 0x00C708A0 call it Animation_ComputeFrameOffset, a propagated
//   wrong label: NAME_FIXES.tsv). The table selector is the audio master's TapTable (0x0143BC2C +0x1B8) or 4.
// Callees: LookupAzimuthElevation(float, float) 0x00C3FF90 (landed, __cdecl), the entry index of the 0x34-byte records.
// Data: the two table pointers of the libpk static record 0x0143BC18 (landed InitTimerGlobals 0x00C33770 sets both to the
//   _PDATA table 0x01443000): +4 0x0143BC1C, +8 0x0143BC20. Float constants 90 / 180 / -90 / -180 / 0.
// Shape: a switch over the mode values (VC7.1 builds the byte index table over 1..64); `azimuth > 90` / `else if (azimuth < -90)`
//   fold the back hemisphere into the front; `azimuth >= 0.0f` lays the unswapped stores first.
struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };   // 0x34-byte record (delay word +0x32)

struct CStaticRecord_0143bc18 {                                 // libpk static record (descriptive; landed InitTimerGlobals model)
    long                    m_count;                            // +0 (0x0143BC18)
    const SDirectionFilter* m_pFirst;                           // +4 (0x0143BC1C)
    const SDirectionFilter* m_pLast;                            // +8 (0x0143BC20)
};
extern CStaticRecord_0143bc18 g_record_0143bc18;                // 0x0143BC18

long __cdecl LookupAzimuthElevation(float azimuth, float elevation);   // 0x00C3FF90

void __cdecl LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first,
                                       const SDirectionFilter** second)
{
    const SDirectionFilter* filters;
    switch (table) {
    case 1:
        filters = g_record_0143bc18.m_pLast;
        break;
    case 8:
    case 64:
        if (azimuth > 90.0f)
            azimuth = 180.0f - azimuth;
        else if (azimuth < -90.0f)
            azimuth = -180.0f - azimuth;
        // fall through
    case 4:
        filters = g_record_0143bc18.m_pFirst;
        break;
    default:
        return;
    }
    long index = LookupAzimuthElevation(azimuth, elevation);
    if (azimuth >= 0.0f) {
        *first = &filters[index];
        *second = &filters[index + 1];
    } else {
        *second = &filters[index];
        *first = &filters[index + 1];
    }
}