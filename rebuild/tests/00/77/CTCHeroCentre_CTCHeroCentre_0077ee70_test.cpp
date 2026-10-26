#include <cstdio>
#include <cstdlib>
#include <new>
static int g_ptrDtors = 0, g_defDestroys = 0, g_baseDtors = 0;
class CIntelligentPointer { public: virtual ~CIntelligentPointer() { ++g_ptrDtors; } void* m_pItem; };
class C3DVector { public: float X, Y, Z; };
template <class T> inline void DestroyRange(T* first, T* last) { for (; first != last; ++first) first->~T(); }
struct CCardVector {
    CIntelligentPointer* m_pStart; CIntelligentPointer* m_pFinish; CIntelligentPointer* m_pEnd;
    ~CCardVector() { DestroyRange(m_pStart, m_pFinish); if (m_pStart) std::free(m_pStart); }
};
struct CPosVector { C3DVector* m_pStart; C3DVector* m_pFinish; C3DVector* m_pEnd; ~CPosVector() { if (m_pStart) std::free(m_pStart); } };
class CDef { public: long RefCount; virtual ~CDef() { } virtual void Destroy() { ++g_defDestroys; } void Release() { if (--RefCount == 0) Destroy(); } };
struct CDefPointer { CDef* m_pDef; ~CDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } } };
class CTCBase { public: ~CTCBase() { ++g_baseDtors; } };
class CTCHeroCentre : public CTCBase { public:
    CDefPointer PDef; long CurrentGUIMode; unsigned int FrameQuestCardListFrom;
    CCardVector QuestCards; CPosVector QuestCardPositions; bool WaitingForTutorialToFinish;
    ~CTCHeroCentre() { }
};
int main()
{
    CDef def; def.RefCount = 1;
    {
        CTCHeroCentre h;
        h.PDef.m_pDef = &def;
        void* mem = std::malloc(3 * sizeof(CIntelligentPointer));
        CIntelligentPointer* cards = (CIntelligentPointer*)mem;
        for (int i = 0; i < 3; ++i) new (&cards[i]) CIntelligentPointer();
        h.QuestCards.m_pStart = cards; h.QuestCards.m_pFinish = cards + 3; h.QuestCards.m_pEnd = cards + 3;
        h.QuestCardPositions.m_pStart = (C3DVector*)std::malloc(sizeof(C3DVector)); h.QuestCardPositions.m_pFinish = h.QuestCardPositions.m_pStart + 1;
    }
    if (g_ptrDtors != 3 || g_defDestroys != 1 || g_baseDtors != 1 || def.RefCount != 0) { std::printf("0077ee70_TEST FAIL\n"); return 1; }
    std::printf("0077ee70_TEST PASS\n");
    return 0;
}