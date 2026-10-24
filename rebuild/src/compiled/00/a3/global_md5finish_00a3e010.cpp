// md5_finish(md5_state_t* pms, md5_byte_t digest[16]) -- L. Peter Deutsch md5.c: pad the message to 56 mod 64 bytes, append the 64-bit bit length and write the 16-byte digest
// Leaf: @md5_finish@
// Flags: /GS
// Retail: Fable.exe @ 0x00A3E010, 172 bytes, plain `ret`. Ego_r.pdb `@md5_finish@8`: extern "C" __fastcall (pms in ecx,
//   digest in edx). /GS: cookie 0x0139C8A8 above the 8-byte `data` buffer, checked by __security_check_cookie 0x00BFE9F9.
// Caller: MD5(const CCharString&) 0x0055E642 (landed 0x0055E5F0).
// Callees: md5_append 0x00A3DF30 (twice: the static 64-byte pad table 0x0129D200, then the 8 length bytes).
// Shape: md5.c verbatim: the 8-iteration length loop fully unrolled into byte stores, the pad length
//   `((55 - (count[0] >> 3)) & 63) + 1` (VC7.1 folds 55 to -9 under the mask), the 16-byte digest loop kept rolled.

#include <string.h>
#pragma intrinsic(memcpy)

typedef unsigned char md5_byte_t;                           // 8-bit byte
typedef unsigned int  md5_word_t;                           // 32-bit word

typedef struct md5_state_s {
    md5_word_t count[2];                                    // +0x00 message length in bits, lsw first
    md5_word_t abcd[4];                                     // +0x08 digest buffer
    md5_byte_t buf[64];                                     // +0x18 accumulate block
} md5_state_t;                                              // 0x58 bytes

// md5_process 0x00A3D5B0 is md5.c's `static` block transform; retail's link-time code generation gave it a custom
// convention (the 64-byte data block in ecx, pms on the stack, callee-clean `ret 4`). That is the __fastcall shape,
// so the callee is declared as a member of the block it transforms (Ego_r.pdb lists it undecorated, "md5_process").
struct md5_block_t {
    md5_byte_t bytes[64];
    void md5_process(md5_state_t* pms) const;               // 0x00A3D5B0
};

extern "C" void __fastcall md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes);   // 0x00A3DF30

extern "C" void __fastcall md5_finish(md5_state_t* pms, md5_byte_t digest[16])
{
    static const md5_byte_t pad[64] = {                     // 0x0129D200
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    md5_byte_t data[8];
    int i;

    /* Save the length before padding. */
    for (i = 0; i < 8; ++i)
        data[i] = (md5_byte_t)(pms->count[i >> 2] >> ((i & 3) << 3));
    /* Pad to 56 bytes mod 64. */
    md5_append(pms, pad, ((55 - (pms->count[0] >> 3)) & 63) + 1);
    /* Append the length. */
    md5_append(pms, data, 8);
    for (i = 0; i < 16; ++i)
        digest[i] = (md5_byte_t)(pms->abcd[i >> 2] >> ((i & 3) << 3));
}