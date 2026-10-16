#include <cstdio>
#include <cstring>
// Behaviour test for NUISystem::CComponent::FindChild / FindChildForward search order (0x00530830).
struct Name { const char* text; unsigned long length; };
struct Str {
    Name* data;
    bool operator==(const Str& o) const
    {
        if (data == o.data) return true;
        if (data && o.data && data->length == o.data->length) return std::strcmp(data->text, o.data->text) == 0;
        return false;
    }
};
struct Comp {
    Str name; Comp* kids[4]; unsigned long count; int visits;
    Comp* FindChild(Str n)
    {
        ++visits;
        if (name == n) return this;
        for (long i = (long)count - 1; i != -1; --i) { Comp* f = kids[i]->FindChild(n); if (f) return f; }
        return 0;
    }
    Comp* FindChildForward(Str n)
    {
        ++visits;
        if (name == n) return this;
        for (unsigned long i = 0; i < count; ++i) { Comp* f = kids[i]->FindChildForward(n); if (f) return f; }
        return 0;
    }
};
static int Fail() { std::printf("00530830_TEST FAIL\n"); return 1; }
int main()
{
    Name nr = {"root", 4}, na = {"item", 4}, nb = {"item", 4}, nz = {"zzzz", 4}, nl = {"longer", 6};
    Comp root, a, b; std::memset(&root, 0, sizeof root); std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b);
    root.name.data = &nr; a.name.data = &na; b.name.data = &nb;
    root.kids[0] = &a; root.kids[1] = &b; root.count = 2;
    Str q; q.data = &na;
    Comp* back = root.FindChild(q);
    Comp* fwd = root.FindChildForward(q);
    if (back != &b || fwd != &a) return Fail();
    Str r; r.data = &nr; if (root.FindChild(r) != &root) return Fail();
    Str z; z.data = &nz; if (root.FindChild(z) != 0 || root.FindChildForward(z) != 0) return Fail();
    Str l; l.data = &nl; if (root.FindChild(l) != 0) return Fail();
    std::printf("00530830_TEST PASS\n");
    return 0;
}