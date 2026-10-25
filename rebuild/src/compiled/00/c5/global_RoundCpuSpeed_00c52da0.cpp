// RoundCpuSpeed(long mhz) -- snap a measured CPU clock (MHz) to the nearest marketing speed: a multiple of 50, then of 33.3, then of 25, when it lies within 1% (+1 MHz) of it; otherwise the measurement itself
// Leaf: ?RoundCpuSpeed@@YAJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52DA0, length 148 bytes (+ int3 padding); __cdecl.
// Callees: none (the divisions are constant multiplies: 0x51EB851F with `sar 5/4/3` for /100 /50 /25, 0x55555556 for /3).
// Caller: the CPU identification 0x00C531B2, on the time-stamp-counter rate (CalibrateTSCFrequency 0x00C52CE0) divided
// by 1,000,000 (__aulldiv). No PDB/RTTI for libpk: the name is descriptive (the catalog label
// "CPU_FindNearestRoundedValue" describes it too).
// Shape: each distance is a named local made absolute with `if (d < 0) d = -d;` (`sub ; jns ; neg`); the <math.h>
// `abs` intrinsic gives `cdq ; xor ; sub` and a ?: macro reorders the compares.
long RoundCpuSpeed(long mhz)
{
    long tolerance = mhz / 100 + 1;
    long rounded = (mhz + 25) / 50 * 50;
    long distance = mhz - rounded;
    if (distance < 0)
        distance = -distance;
    if (distance <= tolerance)
        return rounded;
    rounded = (mhz * 3 + 48) / 100 * 100 / 3;
    distance = mhz - rounded;
    if (distance < 0)
        distance = -distance;
    if (distance <= tolerance)
        return rounded;
    rounded = (mhz + 12) / 25 * 25;
    distance = mhz - rounded;
    if (distance < 0)
        distance = -distance;
    if (distance <= tolerance)
        return rounded;
    return mhz;
}