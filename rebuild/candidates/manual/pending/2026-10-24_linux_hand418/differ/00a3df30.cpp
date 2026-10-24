// md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes) -- L. Peter Deutsch md5.c: add nbytes of data to the running MD5 state (bit count, partial-block buffer, full 64-byte blocks through md5_process)
// Grade: DIFFER(209v209) -- scheduling only: at the first md5_process call retail loads `lea ecx,[ebx+18h]` BEFORE `push ebx` (ours pushes first); md5_process has an LTCG custom convention (data in ecx, pms on the stack) modelled as a __thiscall member; 2 grades
// Leaf: @md5_append@
// Retail: Fable.exe @ 0x00A3DF30, 208 bytes (+ int3 padding), `ret 4`. Ego_r.pdb `@md5_append@12`: md5.c built
//   extern "C" __fastcall (pms in ecx, data in edx, nbytes on the stack).
// Callers: MD5(const CCharString&) 0x0055E635 (landed 0x0055E5F0) and md5_finish 0x00A3E079 / 0x00A3E086.
// Callee: md5_process 0x00A3D5B0 (LTCG custom convention, see the model).
// Shape: md5.c (2002-04-13) verbatim: the bit count update with the carry test (`cmp edi,ecx ; jae`), the initial
//   partial block through the intrinsic memcpy (`rep movsd ; rep movsb`, the copy length homed in the nbytes slot, data
//   re-read from the pushed-ecx frame word), the full-block loop strength-reduced to a 64-byte stride with a precomputed
//   trip count, and the final partial-block memcpy.

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
// convention (the 64-byte data block in ecx, pms on the stack, callee-clean `ret 4`). That is the __thiscall shape,
// so the callee is declared as a member of the block it transforms (Ego_r.pdb lists it undecorated, "md5_process").
struct md5_block_t {
    md5_byte_t bytes[64];
    void md5_process(md5_state_t* pms) const;               // 0x00A3D5B0
};

extern "C" void __fastcall md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes)
{
    const md5_byte_t* p = data;
    int left = nbytes;
    int offset = (pms->count[0] >> 3) & 63;
    md5_word_t nbits = (md5_word_t)(nbytes << 3);

    if (nbytes <= 0)
        return;

    /* Update the message length. */
    pms->count[1] += nbytes >> 29;
    pms->count[0] += nbits;
    if (pms->count[0] < nbits)
        pms->count[1]++;

    /* Process an initial partial block. */
    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

        memcpy(pms->buf + offset, p, copy);
        if (offset + copy < 64)
            return;
        p += copy;
        left -= copy;
        ((const md5_block_t*)pms->buf)->md5_process(pms);
    }

    /* Process full blocks. */
    for (; left >= 64; p += 64, left -= 64)
        ((const md5_block_t*)p)->md5_process(pms);

    /* Process a final partial block. */
    if (left)
        memcpy(pms->buf, p, left);
}
