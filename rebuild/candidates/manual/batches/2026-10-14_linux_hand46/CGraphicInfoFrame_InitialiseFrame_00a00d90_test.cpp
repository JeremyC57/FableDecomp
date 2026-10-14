#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; };
struct CGraphicInfoFrame; struct CFrameData : CIVCountedPointeeBase { CGraphicInfoFrame* m_pFrame; CFrameData(CGraphicInfoFrame* frame) { std::strcat(g_seq, "n"); m_pFrame = frame; IVRefCount = 1; } };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } void Assign(CIVCountedPointeeBase* object) { Release(); Object = object; } };

struct CGraphicInfoFrame { unsigned char _pad[8]; CIVCountedPointer m_data; CFrameData* InitialiseFrame() { if (!m_data.Object) m_data.Assign(new CFrameData(this)); return (CFrameData*)m_data.Object; } };

int main() {
    CGraphicInfoFrame f; f.m_data.Object = 0; CFrameData* d = f.InitialiseFrame(); if (!d || f.m_data.Object != d || d->m_pFrame != &f || std::strcmp(g_seq, "n")) { std::printf("00a00d90_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; if (f.InitialiseFrame() != d || g_seq[0]) { std::printf("00a00d90_TEST FAIL\n"); return 1; } d->IVRefCount = 1; f.m_data.Release(); if (f.m_data.Object != 0 || std::strcmp(g_seq, "D")) { std::printf("00a00d90_TEST FAIL\n"); return 1; }
    std::printf("00a00d90_TEST PASS\n"); return 0;
}
