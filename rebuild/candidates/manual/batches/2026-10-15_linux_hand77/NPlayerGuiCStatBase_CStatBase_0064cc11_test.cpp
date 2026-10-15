#include <cstdio>
#include <new>
// Behaviour test for 0064cc11: the stat-bar base stores the GUI def, clears every value/max/fade member, sets the GUI
// mode to 1 and starts with empty vectors.
struct CPlayerGuiDef { int m_id; };
struct CEnginePrimitiveHandle;
struct CParticleEmitterPointer;
enum EPlayerGuiMode { PLAYER_GUI_MODE_DEFAULT = 1 };
template<class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};
class CStatBase {
public:
    CStatBase(CPlayerGuiDef* guiDef);
    virtual ~CStatBase() {}
    CPlayerGuiDef*                          GuiDef;
    bool                                    ValueInitialised;
    float                                   CurrentValue;
    float                                   OldValue;
    float                                   LastOldValue;
    float                                   RecentMaxValue;
    float                                   OldRecentMaxValue;
    unsigned long                           FrameCurrentValueLastMoved;
    unsigned long                           FrameFullyOpenedOn;
    EPlayerGuiMode                          GuiMode;
    float                                   FadeInTimer;
    CRetailVector<CEnginePrimitiveHandle>   PrimitiveHandles;
    CRetailVector<CParticleEmitterPointer>  Effects;
};
CStatBase::CStatBase(CPlayerGuiDef* guiDef)
    : GuiDef(guiDef), ValueInitialised(false), CurrentValue(0.0f), OldValue(0.0f), LastOldValue(0.0f),
      RecentMaxValue(0.0f), OldRecentMaxValue(0.0f), FrameCurrentValueLastMoved(0), FrameFullyOpenedOn(0),
      GuiMode(PLAYER_GUI_MODE_DEFAULT), FadeInTimer(0.0f)
{
}
int main()
{
    CPlayerGuiDef def; def.m_id = 5;
    union { unsigned char raw[sizeof(CStatBase)]; double alignD; void* alignP; } storage;
    for (unsigned i = 0; i < sizeof(storage.raw); ++i) storage.raw[i] = 0xAB;
    CStatBase* s = new (storage.raw) CStatBase(&def);
    if (s->GuiDef != &def || s->ValueInitialised || s->CurrentValue != 0.0f || s->OldValue != 0.0f || s->LastOldValue != 0.0f
        || s->RecentMaxValue != 0.0f || s->OldRecentMaxValue != 0.0f || s->FrameCurrentValueLastMoved != 0 || s->FrameFullyOpenedOn != 0
        || s->GuiMode != 1 || s->FadeInTimer != 0.0f || s->PrimitiveHandles.m_pFirst != 0 || s->PrimitiveHandles.m_pEnd != 0
        || s->Effects.m_pFirst != 0 || s->Effects.m_pLast != 0) {
        std::printf("0064cc11_TEST FAIL\n"); return 1;
    }
    s->~CStatBase();
    std::printf("0064cc11_TEST PASS\n");
    return 0;
}
