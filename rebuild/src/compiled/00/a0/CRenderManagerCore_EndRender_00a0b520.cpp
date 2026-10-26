// CRenderManagerCore::EndRender() -- close a frame: if rendering, derive the triangles-per-second statistic from the frame time and clear the rendering flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B520, length 63 bytes (+1 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; cmp byte ptr [esi+3A54h],1 ; jne R ; call GFGetTime ; fsub dword ptr [esi+3A40h] ;
//                fild dword ptr [esi+3CD0h] ; mov dword ptr [esi+3CC4h],0 ; fdiv st,st(1) ; call __ftol2 ; fstp st(0) ;
//                mov [esi+3A44h],eax ; mov byte ptr [esi+3A54h],0 ; R: pop esi ; ret
// engine/CRenderManagerCore.h: +0x3A54 bool Rendering, +0x3A40 float LastRenderEndTime, +0x3CD0 long NoTrisDrawn,
// +0x3CC4 NoRenderStateChanges, +0x3A44 long NoTrisPerSecond. Callees: GFGetTime @ 0x009E1BC0 (returns double),
// __ftol2 @ 0x00BFEA70 (the float->long conversion helper the compiler emits).
// Shape: the elapsed time stays on the x87 stack under the division (fdiv st,st(1)) and is popped after the
// conversion, i.e. it is a named local the quotient reads.
#include "engine/CRenderManagerCore.h"

double __fastcall GFGetTime();     // 0x009E1BC0

struct CRenderManagerCore_Methods : CRenderManagerCore {
    void EndRender();
};

void CRenderManagerCore_Methods::EndRender()
{
    if (Rendering == true)
    {
        double elapsed = GFGetTime() - LastRenderEndTime;
        NoRenderStateChanges = 0;
        NoTrisPerSecond = (long)(NoTrisDrawn / elapsed);
        Rendering = false;
    }
}