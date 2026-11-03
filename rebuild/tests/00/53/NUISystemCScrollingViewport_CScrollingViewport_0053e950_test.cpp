// Behaviour test for NUISystem::CScrollingViewport::~CScrollingViewport (0x0053E950): the destructor removes the
// viewport's CObserver subobject from the manager and clears the mouse-listener global only when it points at this.
#include <cstdio>
struct Observer { virtual ~Observer() {} };
struct Manager { int removed; Observer* last; void RemoveObserver(Observer* o) { ++removed; last = o; } };
static Manager g_manager = { 0, 0 };
struct Viewport;
static Viewport* g_pActiveScrollingViewport = 0;
struct UIBase { virtual ~UIBase() {} int pad; };
struct Viewport : UIBase, Observer {
    ~Viewport() {
        g_manager.RemoveObserver(this);
        if (g_pActiveScrollingViewport == this) g_pActiveScrollingViewport = 0;
    }
};
int main()
{
    Viewport* a = new Viewport; Viewport* b = new Viewport;
    g_pActiveScrollingViewport = a;
    Observer* expected = b;
    delete b;
    if (g_manager.removed != 1 || g_manager.last != expected || g_pActiveScrollingViewport != a) { std::printf("0053e950_TEST FAIL\n"); return 1; }
    expected = a;
    delete a;
    if (g_manager.removed != 2 || g_manager.last != expected || g_pActiveScrollingViewport != 0) { std::printf("0053e950_TEST FAIL\n"); return 1; }
    std::printf("0053e950_TEST PASS\n");
    return 0;
}