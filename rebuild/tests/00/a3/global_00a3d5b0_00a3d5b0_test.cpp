#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#endif
#include <stdio.h>
#include <string.h>
// md5_process(md5_state_t*, const md5_byte_t*) -- Deutsch MD5 block transform, retail 00a3d5b0.
// Leaf: _md5_process@8
// Flags:
// Original vendor text restored to its pre-2007 names; static private convention retained.
// The real md5_append caller is present solely to preserve the compiler context.
/*
  Copyright (C) 1999-2012 Artifex Software, Inc.
  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  L. Peter Deutsch
  ghost@aladdin.com

 */
#include <string.h>
#pragma intrinsic(memcpy)

typedef unsigned char md5_byte_t;                           // 8-bit byte
typedef unsigned int  md5_word_t;                           // 32-bit word

typedef struct md5_state_s {
    md5_word_t count[2];                                    // +0x00 message length in bits, lsw first
    md5_word_t abcd[4];                                     // +0x08 digest buffer
    md5_byte_t buf[64];                                     // +0x18 accumulate block
} md5_state_t;                                              // 0x58 bytes

#define T_MASK ((md5_word_t)~0)
#define T1 /* 0xd76aa478 */ (T_MASK ^ 0x28955b87)
#define T2 /* 0xe8c7b756 */ (T_MASK ^ 0x173848a9)
#define T3    0x242070db
#define T4 /* 0xc1bdceee */ (T_MASK ^ 0x3e423111)
#define T5 /* 0xf57c0faf */ (T_MASK ^ 0x0a83f050)
#define T6    0x4787c62a
#define T7 /* 0xa8304613 */ (T_MASK ^ 0x57cfb9ec)
#define T8 /* 0xfd469501 */ (T_MASK ^ 0x02b96afe)
#define T9    0x698098d8
#define T10 /* 0x8b44f7af */ (T_MASK ^ 0x74bb0850)
#define T11 /* 0xffff5bb1 */ (T_MASK ^ 0x0000a44e)
#define T12 /* 0x895cd7be */ (T_MASK ^ 0x76a32841)
#define T13    0x6b901122
#define T14 /* 0xfd987193 */ (T_MASK ^ 0x02678e6c)
#define T15 /* 0xa679438e */ (T_MASK ^ 0x5986bc71)
#define T16    0x49b40821
#define T17 /* 0xf61e2562 */ (T_MASK ^ 0x09e1da9d)
#define T18 /* 0xc040b340 */ (T_MASK ^ 0x3fbf4cbf)
#define T19    0x265e5a51
#define T20 /* 0xe9b6c7aa */ (T_MASK ^ 0x16493855)
#define T21 /* 0xd62f105d */ (T_MASK ^ 0x29d0efa2)
#define T22    0x02441453
#define T23 /* 0xd8a1e681 */ (T_MASK ^ 0x275e197e)
#define T24 /* 0xe7d3fbc8 */ (T_MASK ^ 0x182c0437)
#define T25    0x21e1cde6
#define T26 /* 0xc33707d6 */ (T_MASK ^ 0x3cc8f829)
#define T27 /* 0xf4d50d87 */ (T_MASK ^ 0x0b2af278)
#define T28    0x455a14ed
#define T29 /* 0xa9e3e905 */ (T_MASK ^ 0x561c16fa)
#define T30 /* 0xfcefa3f8 */ (T_MASK ^ 0x03105c07)
#define T31    0x676f02d9
#define T32 /* 0x8d2a4c8a */ (T_MASK ^ 0x72d5b375)
#define T33 /* 0xfffa3942 */ (T_MASK ^ 0x0005c6bd)
#define T34 /* 0x8771f681 */ (T_MASK ^ 0x788e097e)
#define T35    0x6d9d6122
#define T36 /* 0xfde5380c */ (T_MASK ^ 0x021ac7f3)
#define T37 /* 0xa4beea44 */ (T_MASK ^ 0x5b4115bb)
#define T38    0x4bdecfa9
#define T39 /* 0xf6bb4b60 */ (T_MASK ^ 0x0944b49f)
#define T40 /* 0xbebfbc70 */ (T_MASK ^ 0x4140438f)
#define T41    0x289b7ec6
#define T42 /* 0xeaa127fa */ (T_MASK ^ 0x155ed805)
#define T43 /* 0xd4ef3085 */ (T_MASK ^ 0x2b10cf7a)
#define T44    0x04881d05
#define T45 /* 0xd9d4d039 */ (T_MASK ^ 0x262b2fc6)
#define T46 /* 0xe6db99e5 */ (T_MASK ^ 0x1924661a)
#define T47    0x1fa27cf8
#define T48 /* 0xc4ac5665 */ (T_MASK ^ 0x3b53a99a)
#define T49 /* 0xf4292244 */ (T_MASK ^ 0x0bd6ddbb)
#define T50    0x432aff97
#define T51 /* 0xab9423a7 */ (T_MASK ^ 0x546bdc58)
#define T52 /* 0xfc93a039 */ (T_MASK ^ 0x036c5fc6)
#define T53    0x655b59c3
#define T54 /* 0x8f0ccc92 */ (T_MASK ^ 0x70f3336d)
#define T55 /* 0xffeff47d */ (T_MASK ^ 0x00100b82)
#define T56 /* 0x85845dd1 */ (T_MASK ^ 0x7a7ba22e)
#define T57    0x6fa87e4f
#define T58 /* 0xfe2ce6e0 */ (T_MASK ^ 0x01d3191f)
#define T59 /* 0xa3014314 */ (T_MASK ^ 0x5cfebceb)
#define T60    0x4e0811a1
#define T61 /* 0xf7537e82 */ (T_MASK ^ 0x08ac817d)
#define T62 /* 0xbd3af235 */ (T_MASK ^ 0x42c50dca)
#define T63    0x2ad7d2bb
#define T64 /* 0xeb86d391 */ (T_MASK ^ 0x14792c6e)

extern "C" {
static __declspec(noinline) void __stdcall md5_process(md5_state_t *pms, const md5_byte_t *data /*[64]*/)   // 0x00A3D5B0
{
    md5_word_t
        a = pms->abcd[0], b = pms->abcd[1],
        c = pms->abcd[2], d = pms->abcd[3];
    md5_word_t t;
    /* Define storage only for big-endian CPUs. */
    md5_word_t X[16];

    {
        /*
         * On big-endian machines, we must arrange the bytes in the
         * right order.
         */
        const md5_byte_t *xp = data;
        int i;

#define xbuf X                  /* (static only) */
        for (i = 0; i < 16; ++i, xp += 4)
            xbuf[i] = xp[0] + (xp[1] << 8) + (xp[2] << 16) + (xp[3] << 24);
    }

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

    /* Round 1. */
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define SET(a, b, c, d, k, s, Ti)\
  t = a + F(b,c,d) + X[k] + Ti;\
  a = ROTATE_LEFT(t, s) + b
    SET(a, b, c, d,  0,  7, T1);
    SET(d, a, b, c,  1, 12, T2);
    SET(c, d, a, b,  2, 17, T3);
    SET(b, c, d, a,  3, 22, T4);
    SET(a, b, c, d,  4,  7, T5);
    SET(d, a, b, c,  5, 12, T6);
    SET(c, d, a, b,  6, 17, T7);
    SET(b, c, d, a,  7, 22, T8);
    SET(a, b, c, d,  8,  7, T9);
    SET(d, a, b, c,  9, 12, T10);
    SET(c, d, a, b, 10, 17, T11);
    SET(b, c, d, a, 11, 22, T12);
    SET(a, b, c, d, 12,  7, T13);
    SET(d, a, b, c, 13, 12, T14);
    SET(c, d, a, b, 14, 17, T15);
    SET(b, c, d, a, 15, 22, T16);
#undef SET
    /* Round 2. */
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
  t = a + G(b,c,d) + X[k] + Ti;\
  a = ROTATE_LEFT(t, s) + b
    SET(a, b, c, d,  1,  5, T17);
    SET(d, a, b, c,  6,  9, T18);
    SET(c, d, a, b, 11, 14, T19);
    SET(b, c, d, a,  0, 20, T20);
    SET(a, b, c, d,  5,  5, T21);
    SET(d, a, b, c, 10,  9, T22);
    SET(c, d, a, b, 15, 14, T23);
    SET(b, c, d, a,  4, 20, T24);
    SET(a, b, c, d,  9,  5, T25);
    SET(d, a, b, c, 14,  9, T26);
    SET(c, d, a, b,  3, 14, T27);
    SET(b, c, d, a,  8, 20, T28);
    SET(a, b, c, d, 13,  5, T29);
    SET(d, a, b, c,  2,  9, T30);
    SET(c, d, a, b,  7, 14, T31);
    SET(b, c, d, a, 12, 20, T32);
#undef SET
    /* Round 3. */
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define SET(a, b, c, d, k, s, Ti)\
  t = a + H(b,c,d) + X[k] + Ti;\
  a = ROTATE_LEFT(t, s) + b
    SET(a, b, c, d,  5,  4, T33);
    SET(d, a, b, c,  8, 11, T34);
    SET(c, d, a, b, 11, 16, T35);
    SET(b, c, d, a, 14, 23, T36);
    SET(a, b, c, d,  1,  4, T37);
    SET(d, a, b, c,  4, 11, T38);
    SET(c, d, a, b,  7, 16, T39);
    SET(b, c, d, a, 10, 23, T40);
    SET(a, b, c, d, 13,  4, T41);
    SET(d, a, b, c,  0, 11, T42);
    SET(c, d, a, b,  3, 16, T43);
    SET(b, c, d, a,  6, 23, T44);
    SET(a, b, c, d,  9,  4, T45);
    SET(d, a, b, c, 12, 11, T46);
    SET(c, d, a, b, 15, 16, T47);
    SET(b, c, d, a,  2, 23, T48);
#undef SET
    /* Round 4. */
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define SET(a, b, c, d, k, s, Ti)\
  t = a + I(b,c,d) + X[k] + Ti;\
  a = ROTATE_LEFT(t, s) + b
    SET(a, b, c, d,  0,  6, T49);
    SET(d, a, b, c,  7, 10, T50);
    SET(c, d, a, b, 14, 15, T51);
    SET(b, c, d, a,  5, 21, T52);
    SET(a, b, c, d, 12,  6, T53);
    SET(d, a, b, c,  3, 10, T54);
    SET(c, d, a, b, 10, 15, T55);
    SET(b, c, d, a,  1, 21, T56);
    SET(a, b, c, d,  8,  6, T57);
    SET(d, a, b, c, 15, 10, T58);
    SET(c, d, a, b,  6, 15, T59);
    SET(b, c, d, a, 13, 21, T60);
    SET(a, b, c, d,  4,  6, T61);
    SET(d, a, b, c, 11, 10, T62);
    SET(c, d, a, b,  2, 15, T63);
    SET(b, c, d, a,  9, 21, T64);
#undef SET

     /* Then perform the following additions. (That is increment each
        of the four registers by the value it had before this block
        was started.) */
    pms->abcd[0] += a;
    pms->abcd[1] += b;
    pms->abcd[2] += c;
    pms->abcd[3] += d;
}

void __fastcall md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes)
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
        md5_process(pms, pms->buf);
    }

    /* Process full blocks. */
    for (; left >= 64; p += 64, left -= 64)
        md5_process(pms, p);

    /* Process a final partial block. */
    if (left)
        memcpy(pms->buf, p, left);
}
}
static void reference_block(unsigned int state[4], const unsigned char* input)
{
    static const unsigned int constants[64] = {
    0xd76aa478u, 0xe8c7b756u, 0x242070dbu, 0xc1bdceeeu, 0xf57c0fafu, 0x4787c62au, 0xa8304613u, 0xfd469501u,
    0x698098d8u, 0x8b44f7afu, 0xffff5bb1u, 0x895cd7beu, 0x6b901122u, 0xfd987193u, 0xa679438eu, 0x49b40821u,
    0xf61e2562u, 0xc040b340u, 0x265e5a51u, 0xe9b6c7aau, 0xd62f105du, 0x02441453u, 0xd8a1e681u, 0xe7d3fbc8u,
    0x21e1cde6u, 0xc33707d6u, 0xf4d50d87u, 0x455a14edu, 0xa9e3e905u, 0xfcefa3f8u, 0x676f02d9u, 0x8d2a4c8au,
    0xfffa3942u, 0x8771f681u, 0x6d9d6122u, 0xfde5380cu, 0xa4beea44u, 0x4bdecfa9u, 0xf6bb4b60u, 0xbebfbc70u,
    0x289b7ec6u, 0xeaa127fau, 0xd4ef3085u, 0x04881d05u, 0xd9d4d039u, 0xe6db99e5u, 0x1fa27cf8u, 0xc4ac5665u,
    0xf4292244u, 0x432aff97u, 0xab9423a7u, 0xfc93a039u, 0x655b59c3u, 0x8f0ccc92u, 0xffeff47du, 0x85845dd1u,
    0x6fa87e4fu, 0xfe2ce6e0u, 0xa3014314u, 0x4e0811a1u, 0xf7537e82u, 0xbd3af235u, 0x2ad7d2bbu, 0xeb86d391u
    };
    static const int shifts[16] = {7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21};
    unsigned int words[16];
    int j;
    for(j=0;j<16;++j) {
        words[j]=0;
        for(int byte_index=0;byte_index<4;++byte_index)
            words[j] |= ((unsigned int)input[j*4+byte_index]) << (byte_index*8);
    }
    unsigned int a=state[0],b=state[1],c=state[2],d=state[3];
    for(j=0;j<64;++j) {
        unsigned int f;int index;
        if(j<16) {f=(b&c)|((~b)&d);index=j;}
        else if(j<32) {f=(d&b)|((~d)&c);index=(5*j+1)%16;}
        else if(j<48) {f=b^c^d;index=(3*j+5)%16;}
        else {f=c^(b|(~d));index=(7*j)%16;}
        unsigned int sum=a+f+constants[j]+words[index];
        int shift=shifts[(j/16)*4+j%4];
        unsigned int next=b+((sum<<shift)|(sum>>(32-shift)));
        a=d;d=c;c=b;b=next;
    }
    state[0]+=a;state[1]+=b;state[2]+=c;state[3]+=d;
}
static unsigned int random_state=0x8a492f03u;
static unsigned int next_random()
{
    random_state=random_state*1664525u+1013904223u;
    return random_state;
}
static int test_blocks()
{
    for(int iteration=0;iteration<2048;++iteration) {
        md5_state_t actual,before;
        unsigned int expected[4];
        unsigned char storage[72],snapshot[72];
        for(unsigned int i=0;i<sizeof(actual);++i) ((unsigned char*)&actual)[i]=(unsigned char)next_random();
        for(int j=0;j<4;++j)actual.abcd[j]=expected[j]=next_random();
        for(unsigned int k=0;k<sizeof(storage);++k) storage[k]=(unsigned char)(next_random()>>24);
        memcpy(&before,&actual,sizeof(actual));memcpy(snapshot,storage,sizeof(storage));
        const unsigned char* data=storage+(iteration%8);
        reference_block(expected,data);md5_process(&actual,data);
        for(int n=0;n<4;++n) if(actual.abcd[n]!=expected[n]) return 1;
        if(memcmp(actual.count,before.count,sizeof(actual.count)) || memcmp(actual.buf,before.buf,sizeof(actual.buf)))return 2;
        if(memcmp(storage,snapshot,sizeof(storage)))return 3;
    }
    return 0;
}
static int check_digest(const char* message,const char* expected)
{
    unsigned int length=(unsigned int)strlen(message);
    unsigned char padded[256];
    if(length>=sizeof(padded)-72)return 1;
    memset(padded,0,sizeof(padded));memcpy(padded,message,length);padded[length]=0x80;
    unsigned int total=((length+8)/64+1)*64;
    unsigned int bits=length*8;
    for(unsigned int k=0;k<4;++k)padded[total-8+k]=(unsigned char)(bits>>(8*k));
    md5_state_t state;memset(&state,0,sizeof(state));
    state.abcd[0]=0x67452301u;state.abcd[1]=0xefcdab89u;state.abcd[2]=0x98badcfeu;state.abcd[3]=0x10325476u;
    for(unsigned int offset=0;offset<total;offset+=64)md5_process(&state,padded+offset);
    char result[33];
    for(unsigned int i=0;i<16;++i)sprintf(result+2*i,"%02x",(state.abcd[i/4]>>(8*(i%4)))&255u);
    return strcmp(result,expected)!=0;
}
int main()
{
    if(sizeof(unsigned int)!=4 || sizeof(md5_state_t)!=88)return 2;
    if(test_blocks())return 3;
    if(check_digest("","d41d8cd98f00b204e9800998ecf8427e"))return 4;
    if(check_digest("a","0cc175b9c0f1b6a831c399e269772661"))return 4;
    if(check_digest("abc","900150983cd24fb0d6963f7d28e17f72"))return 4;
    if(check_digest("message digest","f96b697d7cb7938d525a2f31aaf161d0"))return 4;
    if(check_digest("abcdefghijklmnopqrstuvwxyz","c3fcd3d76192e4007dfb496cca67e13b"))return 4;
    if(check_digest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789","d174ab98d277d9f5a5611c2c9f419d9f"))return 4;
    if(check_digest("12345678901234567890123456789012345678901234567890123456789012345678901234567890","57edf4a22be3c955ac49da2e2107b67a"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","ef1772b6dff9a122358552954ad0df65"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","3b0c8ac703f828b04c6c197006d17218"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","652b906d60af96844ebd21b674f35e93"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","b06521f39153d618550606be297466d5"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","014842d480b571495a4a0363793f7367"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","c743a45e0d2e6a95cb859adae0248435"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","8a7bd0732ed6a28ce75f6dabc90e1613"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","5f61c0ccad4cac44c75ff505e1f1e537"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","020406e1d05cdc2aa287641f7ae2cc39"))return 4;
    if(check_digest("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","e510683b3f5ffe4093d021808bc6ff70"))return 4;
    puts("00a3d5b0_TEST PASS");return 0;
}