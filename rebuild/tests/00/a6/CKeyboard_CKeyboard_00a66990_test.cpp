// test: CKeyboard::~CKeyboard -- empty user dtor body: the maintained-event list is destroyed first (every node's virtual dtor), then the 256 input events, then the base
#include <stdio.h>
#include <list>
static int order[600]; static int norder = 0;
struct InputEvent { int code; InputEvent() : code(0) { } ~InputEvent() { order[norder++] = 1; } };
struct MaintainedEvent { int id; MaintainedEvent(int i) : id(i) { } MaintainedEvent(const MaintainedEvent& o) : id(o.id) { } virtual ~MaintainedEvent() { order[norder++] = 2; } };
struct BaseClassNonCopyable { virtual ~BaseClassNonCopyable() { order[norder++] = 3; } };
class CKeyboard : public BaseClassNonCopyable { public:
    InputEvent EventStore[256];
    long NoStoredEvents;
    mutable long NoEventScansRunning;
    std::list<MaintainedEvent> MaintainedEventsList;
    void* InputManager;
    ~CKeyboard() { }
};
int main() {
    CKeyboard* d = new CKeyboard;
    d->MaintainedEventsList.push_back(MaintainedEvent(1)); d->MaintainedEventsList.push_back(MaintainedEvent(2));
    norder = 0;
    BaseClassNonCopyable* b = d; delete b;
    if (norder != 2 + 256 + 1 || order[0] != 2 || order[1] != 2 || order[2] != 1 || order[257] != 1 || order[258] != 3) {
        printf("00a66990_TEST FAIL %d\n", norder); return 1; }
    printf("00a66990_TEST PASS\n");
    return 0;
}