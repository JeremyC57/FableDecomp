// _global::DecipherMorsel(unsigned long*, unsigned long*, unsigned long*) -- XTEA-decrypts one 64-bit block (32 rounds, 128-bit key)
// Leaf: ?DecipherMorsel@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E180, 320 bytes (0x138 + int3 padding), __fastcall: ecx = the input block
//   (two dwords), edx = the output block, [esp+4] = the four-dword key; `ret 4`. No callees.
// Identity: Ego_r.pdb / FableWin.pdb ?DecipherMorsel@?A0xf010c0d0@@YIXPAK00@Z -- the anonymous-namespace twin of
//   EncipherMorsel (0x0055E040) in the LHNetworkLib crypto TU.
// Shape: the reference XTEA decipher loop: sum starts at 32 * 0x9E3779B9 = 0xC6EF3720 and the delta is subtracted
//   (`add eax, 0x61C88647`); VC7.1 unrolls the constant 32-round loop by four (ebx = 8) and homes the output pointer in
//   the `push ecx` slot.
namespace {
void __fastcall DecipherMorsel(unsigned long* in, unsigned long* out, unsigned long* key)
{
    unsigned long v0 = in[0];
    unsigned long v1 = in[1];
    unsigned long sum = 0xC6EF3720;
    for (int i = 0; i < 32; ++i) {
        v1 -= (((v0 >> 5) ^ (v0 << 4)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= 0x9E3779B9;
        v0 -= (((v1 >> 5) ^ (v1 << 4)) + v1) ^ (sum + key[sum & 3]);
    }
    out[0] = v0;
    out[1] = v1;
}
}

void (__fastcall *g_keepDecipherMorsel)(unsigned long*, unsigned long*, unsigned long*) =
    &DecipherMorsel;   // keeps the internal-linkage body emitted; not part of the retail function