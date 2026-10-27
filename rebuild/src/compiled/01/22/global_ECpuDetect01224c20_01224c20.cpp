// _global::$E_CpuDetect_01224c20() -- libpk dynamic initializer: run the CPU capability detection once at start-up unless it has already run
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224C20, 15 bytes, in `.text$yc`, reached from the .CRT$XCU table; a tail
//   jump to the detection.
// Identity: a `.text$yc` dynamic initializer (between the libpk-range initializers 0x01224C00 and 0x01224C30) whose only work
//   is `if (!detected) DetectCpu();` over the libpk CPU flag 0x0143BC78, the same test the libpk mixers (landed
//   ConvertInt128BlockToFloat 0x00C6B3A0) run before their SIMD paths. libpk has no PDB or RTTI: the static object's type and
//   name are descriptive (CCpuDetectInit_01224c20 / s_CpuDetectInit_01224c20).
// Label: the manifest's "sub_01224c20" is a gap-scan placeholder.
// Callee: the CPU detection 0x00C52E40 (landed binding ?DetectCpu@@YAXXZ); global g_cpuDetected 0x0143BC78 (landed spelling).
// Shape: an empty static object with an inline constructor and no destructor (no atexit registration).
extern bool g_cpuDetected;                                // 0x0143BC78
void DetectCpu();                                         // 0x00C52E40

struct CCpuDetectInit_01224c20 {
    CCpuDetectInit_01224c20() { if (!g_cpuDetected) DetectCpu(); }
};

static CCpuDetectInit_01224c20 s_CpuDetectInit_01224c20;