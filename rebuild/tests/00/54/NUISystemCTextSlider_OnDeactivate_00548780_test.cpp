// Behaviour test for 00548780 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
enum EEvent { EEVENT_19 = 0x19 };
class CObserver {
public:
    int Ignored[64]; int Count;
    CObserver() : Count(0) { }
    virtual ~CObserver() { }
    virtual void IgnoreEvent(EEvent e) { Ignored[Count++] = e; }
};
class CUIInterface { public: virtual ~CUIInterface() { } };
class CTextSlider : public CUIInterface, public CObserver {
public:
    virtual void OnDeactivate() { IgnoreEvent(EEVENT_19); }
};
int main() {
    CTextSlider s; s.OnDeactivate();
    if (s.Count != 1 || s.Ignored[0] != 0x19) { std::printf("00548780_TEST FAIL\n"); return 1; }
    { std::printf("00548780_TEST PASS\n"); return 0; }
}