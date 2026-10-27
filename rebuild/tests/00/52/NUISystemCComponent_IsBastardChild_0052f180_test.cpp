// Behaviour test for 0052f180 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CComponent {
public:
    virtual ~CComponent() { }
    virtual bool IsBastardChild() const { return (Flags >> 7) & 1; }
    unsigned char Flags; unsigned char Type; unsigned char FlagsSetTwo; char Layer; int DefIndex;
};
int main() {
    CComponent c; const CComponent* p = &c;
    c.Flags = 0x80; if (!p->IsBastardChild()) { std::printf("0052f180_TEST FAIL\n"); return 1; }
    c.Flags = 0x7F; if (p->IsBastardChild()) { std::printf("0052f180_TEST FAIL\n"); return 1; }
    c.Flags = 0xC5; if (!p->IsBastardChild()) { std::printf("0052f180_TEST FAIL\n"); return 1; }
    { std::printf("0052f180_TEST PASS\n"); return 0; }
}