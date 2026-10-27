// GFLimitUpper(float& value, const float& limit) -- clamp a value to an upper limit in place (assign the limit when the value exceeds it)
// Leaf: ??$GFLimitUpper@MM@@YIXAAMABM@Z
// Retail: Fable.exe @ 0x0043DC10, 16 bytes, __fastcall (value in ecx, limit in edx), plain `ret`:
// `fld [ecx] ; fcomp [edx] ; fnstsw ax ; test ah,41h ; jne done ; mov eax,[edx] ; mov [ecx],eax`.
// Identity (proven by name set + body): the PDB name sets hold exactly two float (T&, const T&) __fastcall helpers,
// ??$GFLimitUpper@MM@@YIXAAMABM@Z and ??$GFLimitLower@MM@@YIXAAMABM@Z; this body assigns when value > limit (`test ah,41h ;
// jne` skips on less-or-equal), which is the upper clamp. An unreferenced inline COMDAT (gap-scan function); the limit
// is copied as a dword (no x87 store).
template <class T1, class T2>
void __fastcall GFLimitUpper(T1& value, const T2& limit)
{
    if (value > limit)
        value = limit;
}

template void __fastcall GFLimitUpper<float, float>(float& value, const float& limit);