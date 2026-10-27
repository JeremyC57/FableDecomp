// Behaviour test for 004d8b7c (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CTCTavernTankard {
public:
    virtual ~CTCTavernTankard() { }
    virtual void OnInitialActivate() { Fullness = 0.0f; }
    void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; float Fullness;
};
int main() {
    CTCTavernTankard t; t.Fullness = 0.75f; CTCTavernTankard* p = &t; p->OnInitialActivate();
    if (t.Fullness != 0.0f) { std::printf("004d8b7c_TEST FAIL\n"); return 1; }
    { std::printf("004d8b7c_TEST PASS\n"); return 0; }
}