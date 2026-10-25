// 00545f00 behaviour test: CSpellContainer::AddChildDynamically -- for each state the new child has, the child's
// position and colour are taken from the same state of the child in slot index; a valid slot then gets the new child
// inserted before it, parented, initialised and switched to the state of the component now in that slot (itself).
#include <cstdio>

struct CUIState { float X, Y; float ZoomX, ZoomY; unsigned long Colour; };

struct CComponent {
    unsigned long keys[4]; CUIState states[4]; int stateCount;
    CComponent* parent; int initialised; unsigned long state;
    CComponent() : stateCount(0), parent(0), initialised(0), state(0) {}
    CUIState& Get(unsigned long key)
    {
        for (int i = 0; i < stateCount; ++i) if (keys[i] == key) return states[i];
        keys[stateCount] = key; CUIState z = { 0, 0, 0, 0, 0 }; states[stateCount] = z; return states[stateCount++];
    }
};

struct CSpellContainer : CComponent {
    CComponent* children[8]; unsigned int count;
    void AddChildDynamically(CComponent* child, unsigned long index)
    {
        int n = child->stateCount;          // the walk visits the child's own states
        for (int i = 0; i < n; ++i) {
            unsigned long key = child->keys[i];
            CUIState& src = children[index]->Get(key);
            child->Get(key).X = src.X; child->Get(key).Y = src.Y;
            child->Get(key).Colour = children[index]->Get(key).Colour;
        }
        if (index < count) {
            for (unsigned int j = count; j > index; --j) children[j] = children[j - 1];
            children[index] = child; ++count;
            child->parent = this;
            child->initialised = 1;
            child->state = children[index]->state;
        }
    }
};

static int Fail(const char* why) { std::printf("00545f00_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    CComponent a, b, fresh;
    a.Get(0).X = 10; a.Get(0).Y = 20; a.Get(0).Colour = 0xff00ff00; a.Get(1).X = 11; a.Get(1).Colour = 0x80808080;
    b.Get(0).X = 30; b.state = 1;
    fresh.Get(0).ZoomX = 2.0f; fresh.Get(1);
    CSpellContainer c; c.count = 2; c.children[0] = &a; c.children[1] = &b; a.state = 1;
    c.AddChildDynamically(&fresh, 0);
    if (fresh.Get(0).X != 10 || fresh.Get(0).Y != 20 || fresh.Get(0).Colour != 0xff00ff00 || fresh.Get(0).ZoomX != 2.0f) return Fail("state 0 copy");
    if (fresh.Get(1).X != 11 || fresh.Get(1).Colour != 0x80808080) return Fail("state 1 copy");
    if (c.count != 3 || c.children[0] != &fresh || c.children[1] != &a || c.children[2] != &b) return Fail("insert");
    if (fresh.parent != &c || !fresh.initialised || fresh.state != 0) return Fail("set up");
    CComponent late; late.Get(0); late.state = 7;
    c.AddChildDynamically(&late, 2);
    if (late.Get(0).X != 30 || c.count != 4 || c.children[2] != &late || late.state != 7) return Fail("insert before b");
    std::printf("00545f00_TEST PASS\n");
    return 0;
}