// _global::DecodeFixed16(unsigned short value) -- the parameter record's 16-bit fixed-point decoder: `value * 10000 * (1/65535)`, the inverse of EncodeFixed16 (0x00C1BA20)
// Leaf: DecodeFixed16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BA40, 28 bytes, __stdcall `ret 4`.
// Callers: the record's range getters 0x00C1BA60 / 0x00C1BA70 (`xor eax,eax ; mov ax,[ecx+10h/12h] ; push eax ; call ;
//   ret` -- ecx is not set up for it, so it is not a member). Constants: 10000.0f @ 0x0125A678, 1/65535 (0x37800080)
//   @ 0x012B3230.
// Shape: `movzx eax,word [esp+4] ; mov [esp+4],eax ; fild [esp+4] ; fmul 10000 ; fmul 1/65535` -- the widened value is
//   homed in its own argument slot; the scaled value is a named local so the two literals stay two `fmul`s (the
//   landed encoder's lesson).
// Label correction: the catalog's `WeaponClassToFloatScale` is descriptive (propagated).
float __stdcall DecodeFixed16(unsigned short value)
{
    float scaled = (float)value * 10000.0f;
    return scaled * (1.0f / 65535.0f);
}