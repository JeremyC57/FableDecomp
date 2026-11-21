// CCharString::UnassignString() -- drop this string's reference to its shared data block: when it was the last owner, free the
// block (its text array through the CBasicString destructor, then the block), and clear the data pointer
// Leaf: ?UnassignString@CCharString@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099E9B0, 87 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb 0x00AFA2E0 + FableWin.pdb 0x02F624F0 ?UnassignString@CCharString@@IAEXXZ (protected, thiscall); fable_string.h
//   declares it (protected, `// 0x0099E9B0`). Callers: the CCharString TU (~CCharString 0x0099EAE0, the copy ctor 0x0099EC30,
//   MakeOwnCopyOfData 0x0099EAF0, operator= 0x0099EFB0 ...).
// Name fix (NF218, 2026-11-21, q12377): was a July local CCharString / CCharStringData model with a public `__fastcall` member
//   (?UnassignString@CCharString@@QAIXXZ); ported onto fable_string.h (FABLE_RETAIL_CCHARSTRING + CCHARSTRINGDATA_FRIEND +
//   SHOVE_STRING_CLEANUP_PASS23, the header path whose CBasicString / CCharStringData destructors are this body's expanded
//   `delete data`), the body the landed TU-mate copies in 0x00859040 / 0x0099ED10 / 0x0099EE20 carry (87 bytes, equal to retail).
// Callees: operator delete[] (MSVCR71 thunk 0x00BFEB1C, the text array), operator delete 0x00BFE9BC (the block).
#include "fable_compiler.h"

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#include "fable_string.h"

void __cdecl operator delete(void* block);                                       // 0x00BFE9BC

void CCharString::UnassignString()
{
    CCharStringData* p = PStringData;
    if (p != 0) {
        if (--p->NoRefs <= 0) {
            CCharStringData* data = PStringData;
            if (data != 0)
                delete data;
        }
        PStringData = 0;
    }
}
