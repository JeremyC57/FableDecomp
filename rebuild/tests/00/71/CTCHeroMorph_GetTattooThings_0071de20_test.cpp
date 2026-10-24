// Behaviour test for 0x0071DE20 CTCHeroMorph::GetTattooThings: the caller's vector is emptied (each old def released) and
// refilled with the hero's tattoo defs in order (each copy add-refs).
#include <cstdio>

struct SDef { int RefCount; };
struct SDefVector {
    SDef* items[8]; int size;
    void clear() { for (int i = 0; i < size; ++i) --items[i]->RefCount; size = 0; }
    void push_back(SDef* d) { ++d->RefCount; items[size++] = d; }
};
struct SHeroMorph {
    SDefVector Tattoos;
    void GetTattooThings(SDefVector& things) const
    {
        things.clear();
        for (int i = 0; i < Tattoos.size; ++i) things.push_back(Tattoos.items[i]);
    }
};
int main()
{
    SDef a = { 1 }, b = { 1 }, old = { 2 };
    SHeroMorph h; h.Tattoos.size = 2; h.Tattoos.items[0] = &a; h.Tattoos.items[1] = &b;
    SDefVector out; out.size = 1; out.items[0] = &old;
    h.GetTattooThings(out);
    bool ok = out.size == 2 && out.items[0] == &a && out.items[1] == &b && a.RefCount == 2 && b.RefCount == 2 && old.RefCount == 1;
    if (!ok) { std::printf("0071de20_TEST FAIL\n"); return 1; }
    std::printf("0071de20_TEST PASS\n");
    return 0;
}