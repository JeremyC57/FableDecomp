// `dynamic atexit destructor for 'GFontTextureCache'' -- the inline ~CTexture on the font texture cache: `mov ecx,offset GFontTextureCache ;
//   jmp ?Uninitialise@CTexture@@QAEXXZ`
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A600, 10 bytes; registered with atexit by the dynamic initializer 0x01223EE0 (`push 122A600h`).
// Identity (NF142, 2026-11-20, q4296 binder): the static at 0x013C5A80 is the font TU's anonymous-namespace CTexture GFontTextureCache (the
//   spelling the landed font-cache rows 0x009A3640 / 0x009A25D0 bind, ?GFontTextureCache@?A0x...@@3VCTexture@@A); its destructor is
//   engine/CTexture.h's inline ~CTexture() { Uninitialise(); } (Uninitialise 0x009F9F70: Ego_r.pdb 0x00B69E70 + FableWin.pdb 0x030DE280).
//   Was the stand-in ?GlobalMethodTailThunk_0122a600@@YIXXZ calling ?Invoke_009f9f70@AutoTinyThunkTarget@@QAEXXZ on
//   ?g_AutoTinyThunkObject_013c5a80@@3UAutoTinyThunkTarget@@A. No landed row references 0x0122A600 except its initializer 0x01223EE0
//   (still the CStatic_009f9f70 stand-in; it pushes the address, which its own object emits as its own `_$E` thunk).
// Shape: the static defined in the row (the NF92 0x01228CE0 / NF89 0x01229BE0 form); VC7.1 emits the dynamic initializer (_$E1) and this
//   atexit destructor (_$E2). The `_$E` names are TU-local.
#define FABLE_RETAIL_CTEXTURE
#include "fable_compiler.h"
#include "engine/CTexture.h"
namespace {
CTexture GFontTextureCache;                                             // 0x013C5A80
}
