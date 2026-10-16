#include <cstdio>
typedef unsigned short wch;
static int g_count = 0;
static char g_empty[1] = { 0 };
struct CharData { char* Text; int Length; };
struct WideData { wch buf[32]; wch* start; int NoRefs; };
static WideData g_pool;
static WideData* NewData(int n, wch fill) { for (int i = 0; i < n; ++i) g_pool.buf[i] = fill; g_pool.start = g_pool.buf; g_pool.NoRefs = 1; return &g_pool; }
static WideData* Widen(const CharData* s)
{
    ++g_count;
    int length = s ? s->Length : 0;
    WideData* d = NewData(length, ' ');
    for (int i = 0; i < length; ++i) d->start[i] = (wch)((s ? s->Text : g_empty)[i] & 0xFF);
    return d;
}
int main()
{
    char text[4] = { 'H', 'i', (char)0xE9, 0 };
    CharData s; s.Text = text; s.Length = 3;
    WideData* d = Widen(&s);
    if (d->start[0] != 'H' || d->start[1] != 'i' || d->start[2] != 0xE9 || d->NoRefs != 1 || g_count != 1)
        { std::printf("0099b440_TEST FAIL\n"); return 1; }
    Widen(0);
    if (g_count != 2) { std::printf("0099b440_TEST FAIL\n"); return 1; }
    std::printf("0099b440_TEST PASS\n");
    return 0;
}