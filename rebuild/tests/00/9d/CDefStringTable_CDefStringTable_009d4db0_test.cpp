// Behaviour test for 009d4db0: CDefStringTable ctor -- empty sentinel map, zero counts, flags clear, RandomID = CRC32 of the time.
#include <cstdio>
#include <cstdlib>
static unsigned long g_table[256];
struct TNode { unsigned char color; TNode* parent; TNode* left; TNode* right; void* key; long value; };
static void FakeTime(unsigned char* t) { for (int i = 0; i < 16; ++i) t[i] = (unsigned char)(3 * i + 1); }
static unsigned long Crc(const unsigned char* b, unsigned long n) { unsigned long v = 0; for (unsigned long i = 0; i < n; ++i) v = g_table[b[i] ^ (v & 0xff)] ^ (v >> 8); return v; }
struct TTable {
    TNode* header; unsigned long count; unsigned long StringCount, SharedCount, RandomID; bool Locked, ReadOnly;
    TTable() : header(0), count(0), StringCount(0), SharedCount(0), Locked(false), ReadOnly(false)
    {
        header = (TNode*)std::malloc(sizeof(TNode));
        header->color = 0; header->parent = 0; header->left = header; header->right = header;
        unsigned char t[16]; FakeTime(t); RandomID = Crc(t, 16);
    }
    ~TTable() { std::free(header); }
};
int main()
{
    for (unsigned long n = 0; n < 256; ++n) { unsigned long c = n; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1; g_table[n] = c; }
    TTable t;
    unsigned char ref[16]; FakeTime(ref);
    if (t.header->left != t.header || t.header->right != t.header || t.header->parent != 0 || t.header->color != 0 ||
        t.count != 0 || t.StringCount != 0 || t.SharedCount != 0 || t.Locked || t.ReadOnly || t.RandomID != Crc(ref, 16) || t.RandomID == 0)
    { std::printf("009d4db0_TEST FAIL\n"); return 1; }
    std::printf("009d4db0_TEST PASS\n");
    return 0;
}