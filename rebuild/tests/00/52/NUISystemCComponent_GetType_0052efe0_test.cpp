// Behaviour test for 0052efe0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
enum EType { T0 = 0, T5 = 5, T200 = 200 };
class CComponent {
public:
    virtual ~CComponent() { }
    virtual EType GetType() const { return (EType)Type; }
    unsigned char Flags; unsigned char Type;
};
int main() {
    CComponent c; c.Flags = 0xFF; c.Type = 5; const CComponent* p = &c;
    if (p->GetType() != T5) { std::printf("0052efe0_TEST FAIL\n"); return 1; }
    c.Type = 200; if (p->GetType() != T200) { std::printf("0052efe0_TEST FAIL\n"); return 1; }
    { std::printf("0052efe0_TEST PASS\n"); return 0; }
}