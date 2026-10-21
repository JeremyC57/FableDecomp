#include <cstdio>
// Behaviour test for CTCHeroReceiveItems::PrepareToShowItems (0x007CA670): same model and body as src/, with the
// renderer projection, the display manager's TV area and the intelligent-pointer primitives stubbed.
class CThing;
class CThingObject;
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    void Set(float tlx, float tly, float brx, float bry) { TLX = tlx; TLY = tly; BRX = brx; BRY = bry; }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() { }
    void Set(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
};
static int g_setCalls = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_setCalls; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct SItemNode { SItemNode* _M_next; SItemNode* _M_prev; };
struct CItemList {
    SItemNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
struct CRenderer {
    bool visible; C2DVector pos;
    bool GetScreenPosition(const C3DVector& position, C2DVector* out)
    {
        if (!visible) return false;
        out->X = position.X + pos.X; out->Y = position.Y + pos.Y; return true;
    }
};
struct CDisplayEngine { CRenderer* Renderer; };
struct CMainGameComponent { CDisplayEngine* DisplayEngine; };
struct CDisplayManager {
    C2DBoxF area;
    C2DBoxF GetAVisibleTelevisionScreenArea() { return area; }
};
CMainGameComponent* g_pMainGameComponent;
CDisplayManager* g_pDisplayManager;
C3DVector g_ItemUp(0.0f, 1.0f, 0.0f);

struct CTCHeroReceiveItems {
    CItemList Items;
    CIntelligentPointer<CThingObject> PCurrentItem;
    bool ReceivingItems;
    bool DisplayingItems;
    float AmountThroughForAreaResize;
    C2DBoxF InitialAreaItemTook;
    C2DBoxF OldAreaItemTaking;
    C2DBoxF CurrentAreaItemTaking;
    CRightHandedSet OldItemRHSet;
    CRightHandedSet CurrentItemRHSet;
    void PrepareToShowItems(const C3DVector& position);
};

void CTCHeroReceiveItems::PrepareToShowItems(const C3DVector& position)
{
    if (Items.empty())
        return;
    if (DisplayingItems)
        return;
    AmountThroughForAreaResize = 0.0f;
    C2DVector screen;
    if (g_pMainGameComponent->DisplayEngine->Renderer->GetScreenPosition(position, &screen)) {
        InitialAreaItemTook.Set(screen.X, screen.Y, screen.X, screen.Y);
    } else {
        C2DBoxF area = g_pDisplayManager->GetAVisibleTelevisionScreenArea();
        float x = (area.BRX - area.TLX) * 0.5f + area.TLX;
        float y = (area.BRY - area.TLY) * 0.5f + area.TLY;
        InitialAreaItemTook.Set(x, y, x, y);
    }
    CurrentAreaItemTaking = InitialAreaItemTook;
    OldAreaItemTaking = InitialAreaItemTook;
    ReceivingItems = true;
    OldItemRHSet.Set(g_ItemUp, C3DVector(0.0f, 0.0f, -1.0f));
    CurrentItemRHSet = OldItemRHSet;
    PCurrentItem = CIntelligentPointer<CThingObject>(0);
}

int main()
{
    int fails = 0;
    CRenderer renderer; renderer.visible = true; renderer.pos.X = 10.0f; renderer.pos.Y = 20.0f;
    CDisplayEngine engine = { &renderer };
    CMainGameComponent game = { &engine };
    CDisplayManager display; display.area.Set(100.0f, 50.0f, 300.0f, 150.0f);
    g_pMainGameComponent = &game; g_pDisplayManager = &display;
    SItemNode sentinel = { &sentinel, &sentinel };
    SItemNode node = { &sentinel, &sentinel };
    CTCHeroReceiveItems tc;
    tc.Items._M_node = &sentinel;
    tc.PCurrentItem.m_pItem = (CThing*)&node;
    tc.ReceivingItems = false; tc.DisplayingItems = false; tc.AmountThroughForAreaResize = 0.5f;
    C3DVector pos(1.0f, 2.0f, 3.0f);
    tc.PrepareToShowItems(pos);                         // empty list: untouched
    if (tc.ReceivingItems || tc.AmountThroughForAreaResize != 0.5f) ++fails;
    sentinel._M_next = &node;
    tc.DisplayingItems = true;
    tc.PrepareToShowItems(pos);                         // already displaying: untouched
    if (tc.ReceivingItems) ++fails;
    tc.DisplayingItems = false;
    g_setCalls = 0;
    tc.PrepareToShowItems(pos);
    if (!tc.ReceivingItems || tc.AmountThroughForAreaResize != 0.0f) ++fails;
    if (tc.InitialAreaItemTook.TLX != 11.0f || tc.InitialAreaItemTook.TLY != 22.0f
        || tc.InitialAreaItemTook.BRX != 11.0f || tc.InitialAreaItemTook.BRY != 22.0f) ++fails;
    if (tc.OldAreaItemTaking.BRY != 22.0f || tc.CurrentAreaItemTaking.TLX != 11.0f) ++fails;
    if (tc.OldItemRHSet.Up.Y != 1.0f || tc.OldItemRHSet.Forward.Z != -1.0f || tc.OldItemRHSet.Forward.X != 0.0f) ++fails;
    if (tc.CurrentItemRHSet.Up.Y != 1.0f || tc.CurrentItemRHSet.Forward.Z != -1.0f) ++fails;
    if (tc.PCurrentItem.m_pItem != 0 || g_setCalls != 2) ++fails;
    renderer.visible = false;
    tc.PrepareToShowItems(pos);                         // off screen: centre of the TV area
    if (tc.InitialAreaItemTook.TLX != 200.0f || tc.InitialAreaItemTook.TLY != 100.0f
        || tc.CurrentAreaItemTaking.BRX != 200.0f || tc.OldAreaItemTaking.BRY != 100.0f) ++fails;
    if (fails) { std::printf("007ca670_TEST FAIL\n"); return 1; }
    std::printf("007ca670_TEST PASS\n");
    return 0;
}