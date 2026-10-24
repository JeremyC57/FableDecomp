// Behaviour test for 0x00548760 NUISystem::CTextSlider::OnActivate: starts observing event 0x19 and registers its observer base with the front-end manager
#include <cstdio>
class CObserverBase {
public:
    long Observing;
    CObserverBase() : Observing(0) { }
    virtual void StartObserving(long eventType) { Observing = eventType; }
    virtual void StopObserving(long) { Observing = 0; }
    virtual ~CObserverBase() { }
};
class CFrontEndManager {
public:
    CObserverBase* LastObserver;
    virtual void AddObserver(CObserverBase* observer) { LastObserver = observer; }
    virtual ~CFrontEndManager() { }
    static CFrontEndManager* GetInstance();
};
static CFrontEndManager g_manager;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_manager; }
class CComponentRoot {
public:
    long RootData;
    virtual ~CComponentRoot() { }
};
class CTextSlider : public CComponentRoot, public CObserverBase {
public:
    virtual void OnActivate();
};
void CTextSlider::OnActivate()
{
    StartObserving(0x19);
    CFrontEndManager::GetInstance()->AddObserver(this);
}
int main()
{
    CTextSlider slider;
    g_manager.LastObserver = 0;
    slider.OnActivate();
    CObserverBase* expected = &slider;
    if (slider.Observing != 0x19 || g_manager.LastObserver != expected || (void*)expected == (void*)&slider)
        { std::printf("00548760_TEST FAIL\n"); return 1; }
    std::printf("00548760_TEST PASS\n");
    return 0;
}