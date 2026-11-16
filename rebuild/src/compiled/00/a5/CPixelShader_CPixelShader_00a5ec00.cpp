// CPixelShader::CPixelShader() -- the default constructor: install the vtable (0x0129A054) and clear the shader data pointer
// Leaf: ??0CPixelShader@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5EC00, 16 bytes, thiscall, returns this.
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0x129a054 ; mov dword ptr [eax + 4], 0 ; ret
// Identity: engine/CPixelShader.h (??0CPixelShader@@QAE@XZ 0x00A5EC00, Data +0x04); 24 landed owners (0x00B99D90, 0x00B9ECE0,
//   0x00BC34D0, 0x00B69000, 0x00B73760, ...) call it as ??0CPixelShader@@QAE@XZ; the vtable 0x0129A054 is CPixelShader's.
#include "engine/CPixelShader.h"

CPixelShader::CPixelShader()
    : Data(0)
{
}
