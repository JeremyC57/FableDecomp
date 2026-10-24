// ceil(float x) -- the <math.h> float overload: ceil of x widened to double, returned as float
// Leaf: ?ceil@@YAMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A2D0, 19 bytes, __cdecl `ret`. FableWin.pdb `?ceil@@YAMM@Z`; the VC7.1
//   <math.h> C++ overload `inline float ceil(float _X) { return ((float)ceil((double)_X)); }`, emitted out of line
//   (its only caller is 0x0057DDD3 in the CTCHeroStats TU). The catalog "ceil" names it.
// Callees: the CRT ceil(double) @ 0x00BFEB9C.
// Shape: the float argument widened through an 8-byte stack slot (`fld [esp+4] ; sub esp,8 ; fstp qword`), the
//   double result left in st(0) unrounded (no /Op). Speed TU (`sub esp,8 ; add esp,8`: a size pragma gives
//   `push ecx ; push ecx`).

extern "C" double __cdecl ceil(double x);                                        // 0x00BFEB9C

float ceil(float _X)
{
    return ((float)ceil((double)_X));
}