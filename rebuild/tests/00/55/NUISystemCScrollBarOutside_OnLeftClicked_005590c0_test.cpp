// Behaviour test for NUISystem::CScrollBarOutside::OnLeftClicked (0x005590C0): a copy of the model with recording stubs.
#include <cstdio>

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    const C2DVector operator*(float scalar) const { return C2DVector(X * scalar, Y * scalar); }
};
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DCoordI GetTLPos() const { return C2DCoordI(TLX, TLY); }
    C2DCoordI GetBRPos() const { return C2DCoordI(BRX, BRY); }
};

struct CUIDef { long IVRefCount; long LeftClickedState; int deleted; };
struct CDefPointer {
    CUIDef* Object;
    CDefPointer(CUIDef* object) : Object(object) { ++Object->IVRefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { ++Object->IVRefCount; }
    ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->deleted = 1; }
    CUIDef* operator->() const { return Object; }
};

struct CComponent {
    C2DVector position, zoom, size;
    int sizeCalls;
    C2DVector GetPosition() const { return position; }
    C2DVector GetTargetRelativeRenderPosition() const { return position; }
    C2DVector GetTargetRelativeRenderZoom() const { return zoom; }
    C2DVector GetSize() { ++sizeCalls; return size; }
};

struct CManager {
    CComponent* PMouseCursor;
    C2DVector UIScale;
    static CManager* GetInstance() { static CManager manager; return &manager; }
    CComponent* GetMouseCursor() { return PMouseCursor; }
    C2DVector GetUIScale() const { return UIScale; }
};

enum EEvent { UI_EVENT_LEFT_MOUSE_RELEASE = 28 };
typedef int CActionList;

struct CScrollBarOutside {
    unsigned long TargetState;
    long StateBeforeLeftClicked;
    CComponent* PScrollBar;
    CActionList* PActionsOnLeftClickedAbove;
    CActionList* PActionsOnLeftClickedUnder;
    CUIDef* def;
    unsigned long changedTo;
    CActionList* executed;
    int executeCalls;
    int observed;
    int observeCalls;

    CDefPointer LoadAndReturnDef() const { return CDefPointer(def); }
    void ChangeState(unsigned long state) { changedTo = state; }
    void ExecuteActions(CActionList* actions) { executed = actions; ++executeCalls; }
    void ObserveEvent(EEvent event) { observed = event; ++observeCalls; }

    void OnLeftClicked()
    {
        StateBeforeLeftClicked = TargetState;
        ChangeState(LoadAndReturnDef()->LeftClickedState);
        CComponent* pmouse_cursor = CManager::GetInstance()->GetMouseCursor();
        if (PScrollBar && pmouse_cursor) {
            C2DVector mouse_position = pmouse_cursor->GetPosition();
            C2DVector position = PScrollBar->GetTargetRelativeRenderPosition();
            C2DVector zoom = PScrollBar->GetTargetRelativeRenderZoom();
            C2DBoxI box;
            box.TLX = (long)position.X;
            box.TLY = (long)position.Y;
            box.BRX = (long)((PScrollBar->GetSize() * CManager::GetInstance()->GetUIScale().X).X * zoom.X + position.X);
            box.BRY = (long)((PScrollBar->GetSize() * CManager::GetInstance()->GetUIScale().Y).Y * zoom.Y + position.Y);
            if (box.GetBRPos().Y < mouse_position.Y)
                ExecuteActions(PActionsOnLeftClickedUnder);
            else if (box.GetTLPos().Y > mouse_position.Y)
                ExecuteActions(PActionsOnLeftClickedAbove);
        }
        ObserveEvent(UI_EVENT_LEFT_MOUSE_RELEASE);
    }
};

static CActionList g_above, g_under;
static CUIDef g_def;
static CComponent g_bar, g_cursor;

// One click with the cursor at height mouseY; returns which list ran (0 none, 1 above, 2 under), -1 on a bookkeeping error.
static int Click(float mouseY, bool withBar, bool withCursor)
{
    CScrollBarOutside outside;
    outside.TargetState = 7;
    outside.StateBeforeLeftClicked = -1;
    outside.PScrollBar = withBar ? &g_bar : 0;
    outside.PActionsOnLeftClickedAbove = &g_above;
    outside.PActionsOnLeftClickedUnder = &g_under;
    outside.def = &g_def;
    outside.changedTo = 0;
    outside.executed = 0;
    outside.executeCalls = 0;
    outside.observed = 0;
    outside.observeCalls = 0;
    g_def.IVRefCount = 1;                       // the owner's reference: the temporary must not delete the def
    g_def.LeftClickedState = 42;
    g_def.deleted = 0;
    g_bar.sizeCalls = 0;
    g_cursor.position = C2DVector(300.0f, mouseY);
    CManager::GetInstance()->PMouseCursor = withCursor ? &g_cursor : 0;
    outside.OnLeftClicked();
    if (outside.StateBeforeLeftClicked != 7 || outside.changedTo != 42) return -1;
    if (g_def.IVRefCount != 1 || g_def.deleted) return -1;
    if (outside.observeCalls != 1 || outside.observed != 28) return -1;
    if (g_bar.sizeCalls != ((withBar && withCursor) ? 2 : 0)) return -1;
    if (outside.executeCalls == 0) return 0;
    if (outside.executeCalls != 1) return -1;
    return outside.executed == &g_above ? 1 : outside.executed == &g_under ? 2 : -1;
}

int main()
{
    // Bar at y = 100.5 (top 100), size 40 tall, UI scale 1.5, zoom 2: bottom = (long)(1.5 * 40 * 2 + 100.5) = 220.
    g_bar.position = C2DVector(10.0f, 100.5f);
    g_bar.zoom = C2DVector(1.0f, 2.0f);
    g_bar.size = C2DVector(16.0f, 40.0f);
    CManager::GetInstance()->UIScale = C2DVector(1.0f, 1.5f);
    bool ok = true;
    ok = ok && Click(50.0f, true, true) == 1;       // above the bar
    ok = ok && Click(99.5f, true, true) == 1;       // just above the truncated top
    ok = ok && Click(100.0f, true, true) == 0;      // on the top edge: neither
    ok = ok && Click(150.0f, true, true) == 0;      // on the bar
    ok = ok && Click(220.0f, true, true) == 0;      // on the bottom edge: neither
    ok = ok && Click(220.5f, true, true) == 2;      // just under the truncated bottom
    ok = ok && Click(400.0f, true, true) == 2;      // under the bar
    ok = ok && Click(400.0f, false, true) == 0;     // no scroll bar: only the state change and the event
    ok = ok && Click(50.0f, true, false) == 0;      // no cursor
    // A def held only by the temporary is deleted when the statement ends.
    {
        CScrollBarOutside outside;
        CUIDef def; def.IVRefCount = 0; def.LeftClickedState = 3; def.deleted = 0;
        outside.TargetState = 1; outside.PScrollBar = 0; outside.def = &def;
        outside.executeCalls = 0; outside.observeCalls = 0;
        outside.OnLeftClicked();
        ok = ok && def.deleted == 1 && outside.changedTo == 3 && outside.observeCalls == 1 && outside.executeCalls == 0;
    }
    if (!ok) { std::printf("005590c0_TEST FAIL\n"); return 1; }
    std::printf("005590c0_TEST PASS\n");
    return 0;
}