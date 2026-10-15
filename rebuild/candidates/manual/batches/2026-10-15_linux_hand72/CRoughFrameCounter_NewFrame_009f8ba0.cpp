// CRoughFrameCounter::NewFrame() -- FPS = 1000 / (milliseconds since the last frame), 0 when no time passed; FrameStart = now
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F8BA0, length 62 bytes.
// Retail bytes: push ecx ; push esi ; mov esi,ecx ; call [timeGetTime] ; mov edx,[esi] ; mov ecx,eax ; sub ecx,edx ; je Z ; test ecx,ecx ;
//               mov [esp+4],ecx ; fild [esp+4] ; jge P ; fadd [2^32] ; P: fdivr qword [1000.0] ; mov [esi],eax ; fstp [esi+4] ; ret ;
//               Z: mov dword ptr [esi+4],0 ; mov [esi],eax ; ret
// Callee: WINMM timeGetTime (import 0x014404B4). Constants: 2^32 (float @ 0x0122DCB4, the unsigned fix-up of `fild`) and
// 1000.0 (double @ 0x0122EF08). Layout (PDB): unsigned long FrameStart +0, float FPS +4.
// Shape: the elapsed time is an unsigned long converted to FLOAT (`fild` + the float 2^32 fix-up) and divided into the
// double literal 1000.0; the non-zero case is the `if` body (falls through), the zero case stores the float 0 as an immediate.
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

struct CRoughFrameCounter {
    unsigned long FrameStart;                  // +0x00
    float         FPS;                         // +0x04
    void NewFrame();
};

void CRoughFrameCounter::NewFrame()
{
    unsigned long now = timeGetTime();
    unsigned long elapsed = now - FrameStart;
    if (elapsed != 0)
        FPS = (float)(1000.0 / (float)elapsed);
    else
        FPS = 0.0f;
    FrameStart = now;
}
