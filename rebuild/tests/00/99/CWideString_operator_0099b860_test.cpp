#include <cstdio>
#include <stdlib.h>
#define FAIL { std::printf("0099b860_TEST FAIL\n"); return 1; }
class CWideStringData { public: unsigned short* Start; unsigned short* Finish; unsigned short* EndOfStorage; long NoRefs; };
static int g_freed = 0, g_deleted = 0;
class CWideString { public:
    CWideStringData* PStringData;
    CWideStringData* AllocStringData(const unsigned short* text, long)
    {
        CWideStringData* d = new CWideStringData; unsigned long n = 0; while (text[n]) ++n;
        d->Start = (unsigned short*)malloc((n + 1) * 2); for (unsigned long i = 0; i <= n; ++i) d->Start[i] = text[i];
        d->Finish = d->Start + n; d->EndOfStorage = d->Finish + 1; d->NoRefs = 1; return d;
    }
    void UnassignString()
    {
        CWideStringData* data = PStringData;
        if (data != 0) {
            if (--data->NoRefs <= 0) {
                CWideStringData* block = PStringData;
                if (block != 0) { if (block->Start != 0) { free(block->Start); ++g_freed; } delete block; ++g_deleted; }
            }
            PStringData = 0;
        }
    }
    const CWideString& operator=(unsigned short character)
    {
        unsigned short text[2]; text[0] = character; text[1] = 0;
        UnassignString();
        if (text[0] != 0) PStringData = AllocStringData(text, -1);
        return *this;
    }
};
int main()
{
    CWideString s; s.PStringData = 0;
    s = (unsigned short)'Q';
    if (!s.PStringData || s.PStringData->Start[0] != 'Q' || s.PStringData->Start[1] != 0 || s.PStringData->Finish - s.PStringData->Start != 1) FAIL;
    CWideStringData* first = s.PStringData; first->NoRefs = 2;
    s = (unsigned short)'R';
    if (first->NoRefs != 1 || g_deleted != 0 || s.PStringData->Start[0] != 'R') FAIL;
    s = (unsigned short)0;
    if (s.PStringData != 0 || g_deleted != 1 || g_freed != 1) FAIL;
    std::printf("0099b860_TEST PASS\n");
    return 0;
}