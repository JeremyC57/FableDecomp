#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_finished = true, g_upload = true;
struct CBankFileAsyncData { bool IsFinished() { std::strcat(g_seq, "F"); return g_finished; } void WaitUntilFinished() { std::strcat(g_seq, "W"); } };
struct CFrameData { unsigned char _pad[0x30]; CBankFileAsyncData* m_pAsyncRead; void CompleteLoad() { std::strcat(g_seq, "C"); } bool UploadFrame(long flags) { std::strcat(g_seq, flags == 1 ? "U1" : "U?"); return g_upload; } void SetAsUnloadable() { std::strcat(g_seq, "M"); } };
struct CGraphicInfoFrame { unsigned char _pad[8]; CFrameData* m_pData;
    bool InternalGetFrameForcedLoading(long flags) { if (!m_pData->m_pAsyncRead) return false; if (!m_pData->m_pAsyncRead->IsFinished()) m_pData->m_pAsyncRead->WaitUntilFinished(); m_pData->CompleteLoad(); if (m_pData->UploadFrame(1)) { if (flags & 8) m_pData->SetAsUnloadable(); return true; } return false; } };

int main() {
    CGraphicInfoFrame f; CFrameData d; CBankFileAsyncData a; f.m_pData = &d; d.m_pAsyncRead = &a; g_finished = true; g_upload = true;
    if (!f.InternalGetFrameForcedLoading(8) || std::strcmp(g_seq, "FCU1M")) { std::printf("009ff6b0_TEST FAIL\n"); return 1; } g_seq[0] = 0; if (!f.InternalGetFrameForcedLoading(7) || std::strcmp(g_seq, "FCU1")) { std::printf("009ff6b0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_finished = false; if (!f.InternalGetFrameForcedLoading(8) || std::strcmp(g_seq, "FWCU1M")) { std::printf("009ff6b0_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_upload = false; if (f.InternalGetFrameForcedLoading(8) || std::strcmp(g_seq, "FWCU1")) { std::printf("009ff6b0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; d.m_pAsyncRead = 0; if (f.InternalGetFrameForcedLoading(8) || g_seq[0]) { std::printf("009ff6b0_TEST FAIL\n"); return 1; }
    std::printf("009ff6b0_TEST PASS\n"); return 0;
}
