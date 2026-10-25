// ConvertLongToFloat(float* output, const long* input) -- convert one 128-sample mix block of 32-bit integers to floats with SSE2 (cvtdq2ps), SSE (cvtpi2ps) or x87 code, chosen by the CPU capability flags (detected on first use)
// Grade: DIFFER(265v237) -- retail_asm candidate: SSE2 cvtdq2ps/movaps and SSE cvtpi2ps/movlps/emms inline-asm blocks (ebp inline-asm frame); ours models them with scalar loops | 1 grade | next: land as retail_asm (the two SIMD blocks as inline asm, the x87 fallback in C++)
// Leaf: ?ConvertLongToFloat@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B3A0, 237 bytes (+ int3 padding), `ret` (__cdecl, ebp frame).
// Module: libpk's software mixer (no PDB/RTTI; descriptive name; the manifest's "LongToFloatConvert_Vectorized" is propagated).
// Callees: DetectCPUCapabilities 0x00C52E40 (manifest "InitCPUCapabilities") while the detected flag 0x0143BC78 is clear.
//   Flags: 0x0143BC7C = SSE, 0x0143BC7D = SSE2.
// Retail-asm: 0x00C6B3BA..0x00C6B3F8 (8 passes of `cvtdq2ps xmm0-3,[eax+0/10h/20h/30h] ; movaps [edx+..],xmm0-3`) and
//   0x00C6B402..0x00C6B43A (16 passes of `cvtpi2ps` / `movlps`, then `emms`) are inline-assembly blocks (the ebp frame and
//   the `mov ecx,N ; mov eax,[ebp+0Ch] ; mov edx,[ebp+8]` loads are inline-asm signatures); only the x87 fallback
//   (16 passes of eight fild/fstp) is compiler code. The C++ below models the two SIMD paths with plain loops.
extern bool g_cpuCapabilitiesDetected;   // 0x0143BC78
extern bool g_cpuHasSSE;                 // 0x0143BC7C
extern bool g_cpuHasSSE2;                // 0x0143BC7D
void __cdecl DetectCPUCapabilities();    // 0x00C52E40
void __cdecl ConvertLongToFloat(float* output, const long* input)
{
    if (!g_cpuCapabilitiesDetected)
        DetectCPUCapabilities();
    if (g_cpuHasSSE2) {
        // Retail-asm: SSE2 block (cvtdq2ps / movaps, 16 values per pass)
        for (int pass = 0; pass < 8; ++pass) {
            for (int i = 0; i < 16; ++i)
                output[i] = (float)input[i];
            input += 16;
            output += 16;
        }
        return;
    }
    if (g_cpuHasSSE) {
        // Retail-asm: SSE block (cvtpi2ps / movlps, 8 values per pass, emms)
        for (int pass = 0; pass < 16; ++pass) {
            for (int i = 0; i < 8; ++i)
                output[i] = (float)input[i];
            input += 8;
            output += 8;
        }
        return;
    }
    for (int pass = 16; pass != 0; --pass) {
        output[0] = (float)input[0];
        output[1] = (float)input[1];
        output[2] = (float)input[2];
        output[3] = (float)input[3];
        output[4] = (float)input[4];
        output[5] = (float)input[5];
        output[6] = (float)input[6];
        output[7] = (float)input[7];
        input += 8;
        output += 8;
    }
}
