// _global::Copy_CActionInputControl(const CActionInputControl* first, const CActionInputControl* last, CActionInputControl* result, const random_access_iterator_tag&, int*) -- STLport __copy<CActionInputControl*>: counted forward assignment of 28-byte elements, returns the advanced result
// Leaf: Copy_CActionInputControl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E350, 72 bytes (+8 int3 pad). No callees.
//
// Retail bytes:  push ebx ; mov ebx,ecx ; sub edx,ebx ; mov ecx,edx ; mov eax,0x92492493 ; imul ecx ;
//   add edx,ecx ; sar edx,4 ; mov eax,edx ; shr eax,0x1f ; add eax,edx ; test eax,eax ; jle R ;
//   push esi ; mov edx,eax ; mov eax,[result] ; push edi ; L: mov esi,ebx ; mov edi,eax ;
//   mov ecx,7 ; add ebx,0x1c ; add eax,0x1c ; dec edx ; rep movsd ; jne L ; pop edi ; pop esi ;
//   pop ebx ; ret 0xc ; R: mov eax,[result] ; pop ebx ; ret 0xc
// STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the
// signed count `last - first` (0x92492493 multiply + `sar 4` = divide by 28) computed once, the
// element's implicit operator= a 7-dword `rep movsd`, both pointers stepped by 0x1C, the advanced
// result returned. /Gr algorithm TU: __fastcall (first ecx, last edx; result, the tag reference and
// the null distance pointer on the stack -> `ret 0xc`). The manifest label
// `_Copy_impl<CActionInputControl*,CActionInputControl*>` names the element correctly (FableWin.pdb
// is a later Dinkumware build where this helper is `std::_Copy_impl`). Ego_r.pdb CActionInputControl
// (0x1C): GameAction +0, ControllerType +4, KeyboardKey +8, XboxButton +0xC, MouseButton +0x10,
// ControlDirection C2DVector +0x14.
class C2DVector { public:
    float X;                                      // +0
    float Y;                                      // +4
};

class CActionInputControl { public:
    long GameAction;                              // +0x00 EGameAction
    long ControllerType;                          // +0x04 EControllerType
    long KeyboardKey;                             // +0x08 EInputKey
    long XboxButton;                              // +0x0C EXboxControllerButton
    long MouseButton;                             // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                   // +0x14
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CActionInputControl* __fastcall Copy_CActionInputControl(const CActionInputControl* first, const CActionInputControl* last, CActionInputControl* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}