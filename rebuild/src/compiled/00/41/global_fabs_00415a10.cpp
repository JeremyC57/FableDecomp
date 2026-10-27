// fabs(float x) -- VC7.1 <math.h> C++ float overload: |x| through the double intrinsic (`fld ; fabs ; ret`)
// Leaf: ?fabs@@YAMM@Z
// Retail: Fable.exe @ 0x00415A10, 7 bytes, __cdecl: `fld dword [esp+4] ; fabs ; ret`.
// Identity (probable): the out-of-line copy of the VC7.1 math.h overload `inline float fabs(float _X)
// {return ((float)fabs((double)_X)); }` (Ego_r.pdb / FableWin.pdb name sets hold ?fabs@@YAMM@Z; the sibling overload
// ?abs@@YAMM@Z compiles to the same seven bytes, so the choice between the two is by name frequency, not by bytes).
// No direct caller in retail (gap-scan COMDAT); libpk carries a byte-identical copy at 0x00C70010.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

float __cdecl fabs(float x)
{
    return (float)fabs((double)x);
}