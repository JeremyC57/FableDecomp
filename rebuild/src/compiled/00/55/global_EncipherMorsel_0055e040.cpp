// _global::EncipherMorsel(unsigned long*, unsigned long*, unsigned long*) -- XTEA-encrypts one 64-bit block (32 rounds, 128-bit key)
// Leaf: ?EncipherMorsel@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E040, 320 bytes (0x138 + int3 padding), __fastcall: ecx = the input block
//   (two dwords), edx = the output block, [esp+4] = the four-dword key; `ret 4`. No callees.
// Identity: Ego_r.pdb / FableWin.pdb ?EncipherMorsel@?A0xf010c0d0@@YIXPAK00@Z -- an anonymous-namespace free function
//   `void __fastcall EncipherMorsel(unsigned long*, unsigned long*, unsigned long*)` of the LHNetworkLib crypto TU.
// Shape: the reference XTEA encipher loop. VC7.1 unrolls the constant 32-round loop by four (ebx = 8, `dec ebx ; jne`),
//   the `push ecx` slot homes the output pointer (edx) across the loop, sum lives in eax and the delta 0x9E3779B9 is
//   emitted as `sub eax, 0x61C88647`.
namespace {
void __fastcall EncipherMorsel(unsigned long* in, unsigned long* out, unsigned long* key)
{
    unsigned long v0 = in[0];
    unsigned long v1 = in[1];
    unsigned long sum = 0;
    for (int i = 0; i < 32; ++i) {
        v0 += (((v1 >> 5) ^ (v1 << 4)) + v1) ^ (sum + key[sum & 3]);
        sum += 0x9E3779B9;
        v1 += (((v0 >> 5) ^ (v0 << 4)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    out[0] = v0;
    out[1] = v1;
}
}

void (__fastcall *g_keepEncipherMorsel)(unsigned long*, unsigned long*, unsigned long*) =
    &EncipherMorsel;   // keeps the internal-linkage body emitted; not part of the retail function