// Behaviour test for NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::~CQuestInfoElementMiniGameBar @ 0x0064A515
#include <cstdio>
#include <stdlib.h>

static long g_removed[8];
static int  g_removedCount = 0;
static int  g_handleVectorDestroyed = 0;

struct CRetailHandleVector {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    ~CRetailHandleVector() { ++g_handleVectorDestroyed; }
};

struct CRetailLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    long& operator[](unsigned long index) { return m_pFirst[index]; }
    ~CRetailLongVector() { if (m_pFirst) free(m_pFirst); }
};

struct CDrawQuestInfo {
    void RemoveElement(long id) { if (g_removedCount < 8) g_removed[g_removedCount] = id; ++g_removedCount; }
};

struct CPlayerGui {
    CDrawQuestInfo* m_pDrawQuestInfo;
};
static CDrawQuestInfo g_drawQuestInfo;
static CPlayerGui     g_playerGui = { &g_drawQuestInfo };
static CPlayerGui*    g_pPlayerGui = &g_playerGui;

struct CQuestInfoElement {
    virtual ~CQuestInfoElement() { }
    long ID, ScriptID;
    float Scale;
    CRetailHandleVector PrimitiveHandles;
};

struct CQuestInfoElementMiniGameBar : CQuestInfoElement {
    long Type;
    CRetailLongVector ExtraElements;
    virtual ~CQuestInfoElementMiniGameBar()
    {
        for (unsigned long i = 0; i < ExtraElements.size(); ++i)
            g_pPlayerGui->m_pDrawQuestInfo->RemoveElement(ExtraElements[i]);
    }
};

int main()
{
    {
        CQuestInfoElementMiniGameBar bar;
        long* data = (long*)malloc(3 * sizeof(long));
        data[0] = 11; data[1] = 22; data[2] = 33;
        bar.ExtraElements.m_pFirst = data;
        bar.ExtraElements.m_pLast = data + 3;
        bar.ExtraElements.m_pEnd = data + 3;
    }
    if (g_removedCount != 3 || g_removed[0] != 11 || g_removed[1] != 22 || g_removed[2] != 33 || g_handleVectorDestroyed != 1) { std::printf("0064a515_TEST FAIL\n"); return 1; }
    {
        CQuestInfoElementMiniGameBar empty;
        empty.ExtraElements.m_pFirst = 0; empty.ExtraElements.m_pLast = 0; empty.ExtraElements.m_pEnd = 0;
    }
    if (g_removedCount != 3 || g_handleVectorDestroyed != 2) { std::printf("0064a515_TEST FAIL\n"); return 1; }
    std::printf("0064a515_TEST PASS\n");
    return 0;
}
