// Behaviour test for 0x00546C90 NUISystem::CTextContainer::AddChild: only text children are accepted; each gets the container's text window, then the base AddChild
#include <cstdio>
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
enum EType { TYPE_SPRITE = 1, TYPE_TEXT = 6 };
class CComponent;
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
static int g_baseAdds = 0;
class CComponent {
public:
    virtual void AddChild(CCountedPointer<CComponent>& child);
    virtual EType GetType() const { return TYPE_SPRITE; }
    virtual ~CComponent() { }
};
void CComponent::AddChild(CCountedPointer<CComponent>&) { ++g_baseAdds; }
class CText : public CComponent {
public:
    C2DBoxF Window;
    virtual EType GetType() const { return TYPE_TEXT; }
    virtual void SetTextWindow(C2DBoxF& window) { Window = window; }
};
class CTextContainer : public CComponent {
public:
    virtual void AddChild(CCountedPointer<CComponent>& child);
    C2DBoxF Window;
};
void CTextContainer::AddChild(CCountedPointer<CComponent>& child)
{
    if (child->GetType() == TYPE_TEXT) {
        static_cast<CText*>(child.Data)->SetTextWindow(Window);
        CComponent::AddChild(child);
    }
}
int main()
{
    CTextContainer box;
    box.Window.TLX = 1.0f; box.Window.TLY = 2.0f; box.Window.BRX = 3.0f; box.Window.BRY = 4.0f;
    CComponent sprite;
    CText text;
    text.Window.TLX = text.Window.TLY = text.Window.BRX = text.Window.BRY = 0.0f;
    CCountedPointer<CComponent> ps; ps.Data = &sprite; ps.Info = 0;
    CCountedPointer<CComponent> pt; pt.Data = &text; pt.Info = 0;
    box.AddChild(ps);
    if (g_baseAdds != 0) { std::printf("00546c90_TEST FAIL\n"); return 1; }
    box.AddChild(pt);
    if (g_baseAdds != 1 || text.Window.TLX != 1.0f || text.Window.BRY != 4.0f) { std::printf("00546c90_TEST FAIL\n"); return 1; }
    std::printf("00546c90_TEST PASS\n");
    return 0;
}