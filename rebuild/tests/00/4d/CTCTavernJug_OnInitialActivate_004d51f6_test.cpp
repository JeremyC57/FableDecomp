// Behaviour test for 004d51f6 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CTCTavernJug {
public:
    virtual ~CTCTavernJug() { }
    virtual void OnInitialActivate() { Fullness = 0; OnBar = true; }
    void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; int Fullness; bool OnBar;
};
int main() {
    CTCTavernJug j; j.Fullness = 7; j.OnBar = false; j.ToBeRemoved = true;
    CTCTavernJug* p = &j; p->OnInitialActivate();
    if (j.Fullness != 0 || !j.OnBar || !j.ToBeRemoved) { std::printf("004d51f6_TEST FAIL\n"); return 1; }
    { std::printf("004d51f6_TEST PASS\n"); return 0; }
}