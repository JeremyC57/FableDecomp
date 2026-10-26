// Behaviour test for 0x00807370 STLport swap<CIntelligentPointer<CThing> >: the two pointers exchange their things.
#include <cstdio>
class CThing { public: int id; };
struct CPointer {
    CThing* Item;
    CPointer() : Item(0) { }
    CPointer(const CPointer& o) : Item(0) { SetPItem(o.GetPItem()); }
    CThing* GetPItem() const { return Item; }
    void SetPItem(CThing* item) { Item = item; }
    CPointer& operator=(const CPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
static void Swap(CPointer& a, CPointer& b)
{
    CPointer tmp = a;
    a = b;
    b = tmp;
}
int main()
{
    CThing x = { 1 }, y = { 2 };
    CPointer a, b, n;
    a.SetPItem(&x); b.SetPItem(&y);
    Swap(a, b);
    bool ok = a.GetPItem() == &y && b.GetPItem() == &x;
    Swap(a, n);                                           // with a null pointer
    ok = ok && a.GetPItem() == 0 && n.GetPItem() == &y;
    Swap(b, b);                                           // self swap keeps the thing
    ok = ok && b.GetPItem() == &x;
    if (!ok) { std::printf("00807370_TEST FAIL\n"); return 1; }
    std::printf("00807370_TEST PASS\n");
    return 0;
}