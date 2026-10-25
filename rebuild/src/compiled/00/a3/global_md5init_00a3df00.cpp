// md5_init(md5_state_t* pms) -- L. Peter Deutsch's md5.c: clear the bit count and load the four MD5 chaining constants
// Leaf: @md5_init@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3DF00, 42 bytes, __fastcall (pms in ecx), plain `ret`.
// Identity: FableWin.pdb / Ego_r.pdb `@md5_init@4` (with @md5_append@12, @md5_finish@8 and md5_process); the one caller
//   is the anonymous-namespace MD5(const CCharString&) 0x0055E5F0 (Ego_r.pdb ?MD5@?A0xf010c0d0@@YI?AVCCharString@@ABV2@@Z),
//   which runs md5_init 0x00A3DF00 -> md5_append 0x00A3DF30 (ecx state, edx data, length pushed) -> md5_finish 0x00A3E010
//   on a stack state. The bytes are the RFC 1321 initial values. The earlier "COpinionTransientOffset" constructor label
//   and the generated C00a3df00::M stub were propagated / descriptive.
// Shape: `count[0] = count[1] = 0` stores count[1] first; abcd[1] and abcd[2] are written as T_MASK ^ constant in md5.c
//   and fold to 0xEFCDAB89 / 0x98BADCFE.
typedef unsigned char md5_byte_t;
typedef unsigned int md5_word_t;
typedef struct md5_state_s {
    md5_word_t count[2];                        // +0x00 message length in bits, lsw first
    md5_word_t abcd[4];                         // +0x08 digest buffer
    md5_byte_t buf[64];                         // +0x18 accumulate block
} md5_state_t;
#define T_MASK ((md5_word_t)~0)

extern "C" void __fastcall md5_init(md5_state_t* pms)
{
    pms->count[0] = pms->count[1] = 0;
    pms->abcd[0] = 0x67452301;
    pms->abcd[1] = /*0xefcdab89*/ T_MASK ^ 0x10325476;
    pms->abcd[2] = /*0x98badcfe*/ T_MASK ^ 0x67452301;
    pms->abcd[3] = 0x10325476;
}
