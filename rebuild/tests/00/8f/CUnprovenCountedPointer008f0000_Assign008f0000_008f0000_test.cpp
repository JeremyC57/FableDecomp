// Behaviour test for 0x008F0000 CUnprovenCountedPointer_008f0000::Assign_008f0000(const holder&): a no-op when both hold
// the same object; otherwise the old object is released (slot-1 virtual on the last reference) and the new one shared.
#include <cstdio>

static int g_released = 0;

class Countee {
public:
    long RefCount;
    virtual ~Countee() { }
    virtual void Release() { ++g_released; }
};
class Holder {
public:
    Countee* Object;
    void Assign(const Holder& other)
    {
        Countee* current = Object;
        if (current != other.Object) {
            if (current) {
                if (--current->RefCount == 0)
                    current->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
    }
};

int main()
{
    Countee a, b;
    a.RefCount = 1; b.RefCount = 1;
    Holder h; h.Object = &a;
    Holder hb; hb.Object = &b;
    h.Assign(h);
    if (a.RefCount != 1 || g_released != 0) { std::printf("008f0000_TEST FAIL\n"); return 1; }
    h.Assign(hb);
    if (h.Object != &b || b.RefCount != 2 || a.RefCount != 0 || g_released != 1) { std::printf("008f0000_TEST FAIL\n"); return 1; }
    Holder nul; nul.Object = 0;
    h.Assign(nul);
    if (h.Object != 0 || b.RefCount != 1 || g_released != 1) { std::printf("008f0000_TEST FAIL\n"); return 1; }
    h.Assign(hb);
    if (h.Object != &b || b.RefCount != 2) { std::printf("008f0000_TEST FAIL\n"); return 1; }
    std::printf("008f0000_TEST PASS\n");
    return 0;
}