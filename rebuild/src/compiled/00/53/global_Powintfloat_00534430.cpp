// _Pow_int<float>(float x, int y) -- the VC7.1 <math.h> integer power helper behind pow(float, int): square-and-multiply on |y|, reciprocal for a negative exponent
// Flags: /Gr
// Leaf: ??$_Pow_int@M@@YIMMH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00534430, 55 bytes, __fastcall (/Gr: y in ecx, the float on the stack), `ret 4`,
//   result on st(0).
// Identity (proven): Ego_r.pdb has ??$_Pow_int@M@@YIMMH@Z; the only caller 0x005349B0 is the out-of-line
//   `inline float __cdecl pow(float, int)` of <math.h> (both arguments from the stack, `push x ; mov ecx,y`), and the
//   body is the library template verbatim (1.0f from 0x0122DED8, `fdivr` for a negative exponent). Library code, not a
//   Fable function: the TU that holds it (compiled /Gr) instantiated it out of line.
// Body: VC7.1 <math.h> _Pow_int as shipped.
template<class _Ty> inline
_Ty _Pow_int(_Ty _X, int _Y)
{
    unsigned int _N;
    if (_Y >= 0)
        _N = (unsigned int)_Y;
    else
        _N = (unsigned int)(-_Y);
    for (_Ty _Z = _Ty(1); ; _X *= _X)
    {
        if ((_N & 1) != 0)
            _Z *= _X;
        if ((_N >>= 1) == 0)
            return (_Y < 0 ? _Ty(1) / _Z : _Z);
    }
}

template float _Pow_int<float>(float, int);