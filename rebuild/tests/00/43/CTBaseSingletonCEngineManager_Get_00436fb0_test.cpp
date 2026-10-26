// Behaviour test for CTBaseSingleton<CEngineManager>::Get (0x00436FB0): the first call creates the manager with an empty engine
// list (sentinel pointing at itself) and an empty changed-area vector and hands it to the counted pointer; later calls return the
// same object without another Reset.
#include <cstdio>
#include <cstdlib>
template <class T> class CCountedPointer {
public:
    T* Data; void* Info; int resets;
    void Reset(T* data) { Data = data; ++resets; }
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() {}
    static CCountedPointer<T> Instance;
    static T& Get() { if (!Instance.Data) Instance.Reset(new T); return *Instance.Data; }
};
struct Node { Node* next; Node* prev; void* data; };
class EngineList { public: EngineList() : node(0) { Node* n = (Node*)std::malloc(sizeof(Node)); n->next = n; n->prev = n; node = n; } Node* node; };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class BoxVector { public: BoxVector() : start(0), finish(0), end(0) {} C2DBoxI* start; C2DBoxI* finish; C2DBoxI* end; };
class CEngineManager : public CTBaseSingleton<CEngineManager> {
public:
    EngineList Engines; BoxVector ChangedAreas;
};
template <> CCountedPointer<CEngineManager> CTBaseSingleton<CEngineManager>::Instance = { 0, 0, 0 };
int main()
{
    int bad = 0;
    CEngineManager& m = CTBaseSingleton<CEngineManager>::Get();
    if (CTBaseSingleton<CEngineManager>::Instance.resets != 1 || CTBaseSingleton<CEngineManager>::Instance.Data != &m) bad = 1;
    if (!m.Engines.node || m.Engines.node->next != m.Engines.node || m.Engines.node->prev != m.Engines.node) bad = 1;
    if (m.ChangedAreas.start || m.ChangedAreas.finish || m.ChangedAreas.end) bad = 1;
    CEngineManager& again = CTBaseSingleton<CEngineManager>::Get();
    if (&again != &m || CTBaseSingleton<CEngineManager>::Instance.resets != 1) bad = 1;
    if (bad) { std::printf("00436fb0_TEST FAIL\n"); return 1; }
    std::printf("00436fb0_TEST PASS\n");
    return 0;
}