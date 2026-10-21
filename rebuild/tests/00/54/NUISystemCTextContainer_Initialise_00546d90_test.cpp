// Behaviour test for NUISystem::CTextContainer::Initialise (0x00546D90): every child after the first gets all of its
// state records' Y set to the summed heights of the children above, moves to its state-0 position, and the
// container's height becomes the sum of all child heights.
#include <cstdio>
#include <map>
#include <vector>

struct SState { float X, Y; };
struct CChild {
    float Height; std::map<unsigned long, SState> States; SState Pos;
    void SetPosition(const SState& p) { Pos = p; }
};
struct CContainer {
    std::vector<CChild*> Children; float SizeY; int BaseInit;
    void Initialise()
    {
        ++BaseInit;
        float y = 0.0f;
        unsigned int i = 1;
        if (i < Children.size()) {
            unsigned long state = 0;
            do {
                y += Children[i - 1]->Height;
                for (std::map<unsigned long, SState>::iterator it = Children[i]->States.begin(); it != Children[i]->States.end(); ++it)
                    it->second.Y = y;
                Children[i]->SetPosition(Children[i]->States[state]);
            } while (++i < Children.size());
        }
        if (!Children.empty())
            SizeY = y + Children[Children.size() - 1]->Height;
    }
};

int main()
{
    CChild a, b, c;
    a.Height = 10; b.Height = 20; c.Height = 5;
    SState s0 = { 1, 0 }, s1 = { 2, 99 };
    b.States[0] = s0; b.States[1] = s1; c.States[0] = s1;
    CContainer box; box.SizeY = -1; box.BaseInit = 0;
    box.Children.push_back(&a); box.Children.push_back(&b); box.Children.push_back(&c);
    box.Initialise();
    bool ok = box.BaseInit == 1 && b.States[1].Y == 10 && b.Pos.Y == 10 && b.Pos.X == 1 && c.Pos.Y == 30 && box.SizeY == 35;
    CContainer empty; empty.SizeY = -1; empty.BaseInit = 0; empty.Initialise();
    ok = ok && empty.SizeY == -1;
    CContainer one; one.SizeY = 0; one.BaseInit = 0; one.Children.push_back(&c); one.Initialise();
    ok = ok && one.SizeY == 5;
    if (!ok) { std::printf("00546d90_TEST FAIL\n"); return 1; }
    std::printf("00546d90_TEST PASS\n");
    return 0;
}