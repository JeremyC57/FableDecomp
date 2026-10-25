// Behaviour test for DecipherMorsel (0x0055E180): the XTEA decipher model inverts the published XTEA test vectors.
#include <cstdio>

static void DecipherMorsel(unsigned int* in, unsigned int* out, unsigned int* key)
{
    unsigned int v0 = in[0];
    unsigned int v1 = in[1];
    unsigned int sum = 0xC6EF3720u;
    for (int i = 0; i < 32; ++i) {
        v1 -= (((v0 >> 5) ^ (v0 << 4)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= 0x9E3779B9u;
        v0 -= (((v1 >> 5) ^ (v1 << 4)) + v1) ^ (sum + key[sum & 3]);
    }
    out[0] = v0;
    out[1] = v1;
}

int main()
{
    unsigned int key0[4] = { 0, 0, 0, 0 };
    unsigned int in0[2] = { 0xDEE9D4D8u, 0xF7131ED9u };
    unsigned int out0[2] = { 1, 1 };
    DecipherMorsel(in0, out0, key0);
    if (out0[0] != 0 || out0[1] != 0) { std::printf("0055e180_TEST FAIL\n"); return 1; }

    unsigned int key1[4] = { 0x00010203u, 0x04050607u, 0x08090A0Bu, 0x0C0D0E0Fu };
    unsigned int in1[2] = { 0x497DF3D0u, 0x72612CB5u };
    unsigned int out1[2] = { 0, 0 };
    DecipherMorsel(in1, out1, key1);
    if (out1[0] != 0x41424344u || out1[1] != 0x45464748u) { std::printf("0055e180_TEST FAIL\n"); return 1; }

    std::printf("0055e180_TEST PASS\n");
    return 0;
}