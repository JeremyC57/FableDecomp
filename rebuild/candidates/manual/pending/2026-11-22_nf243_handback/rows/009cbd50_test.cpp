// Behaviour of 0x009CBD50 CBankFile::SetBankUpdateProgress: forwards to the bank's progress callback (+0xEC) only when one is set, on the
// shared engine/CBankFile.h (the wrapper declared only by FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL). Access is opened the way the
// landed CBankFile::Update 0x009CEC60 test does, so the test carries the row's body itself (the access change alters the mangling);
// the bank is never constructed (only +0xEC is read).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_UPDATE_PASS27
#define FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL
#include <stdio.h>
#define private public
#define protected public
#include "engine/CBankFile.h"
void CBankFile::SetBankUpdateProgress(unsigned long done, unsigned long total)
{
    if (ProgressCallback)
        ProgressCallback->SetProgress(done, total);
}
static int g_slot = -1, g_hits = 0; static unsigned long g_a = 0, g_b = 0; static const void* g_text = 0;
class CTestCallback : public CBankFile::CProgressCallback {
public:
    virtual void SetProgress(unsigned long done, unsigned long total) { g_slot = 0; ++g_hits; g_a = done; g_b = total; }
    virtual void SetStatus(const CCharString& text) { g_slot = 1; ++g_hits; g_text = &text; }
    virtual void WriteLog(const CCharString& text) { g_slot = 2; ++g_hits; g_text = &text; }
};
static unsigned long g_storage[0x120 / 4];
int main()
{
    CBankFile* bank = reinterpret_cast<CBankFile*>(g_storage);
    unsigned long word = 0;
    const CCharString& text = *reinterpret_cast<const CCharString*>(&word);
    (void)text;
    bank->ProgressCallback = 0;
    bank->SetBankUpdateProgress(3, 7);
    if (g_hits != 0) { printf("009cbd50_TEST FAIL\n"); return 1; }
    CTestCallback callback;
    bank->ProgressCallback = &callback;
    bank->SetBankUpdateProgress(3, 7);
    if (!(g_hits == 1 && g_slot == 0 && g_a == 3 && g_b == 7)) { printf("009cbd50_TEST FAIL\n"); return 1; }
    printf("009cbd50_TEST PASS\n");
    return 0;
}
