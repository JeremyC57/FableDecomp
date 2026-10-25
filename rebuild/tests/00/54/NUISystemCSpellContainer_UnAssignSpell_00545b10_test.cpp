// Behaviour test for NUISystem::CSpellContainer::UnAssignSpell (0x00545B10): when the slot holds the pending new spell,
// that spell is marked for deletion (method 1), the slot's base spell is re-added there and the pending spell cleared;
// otherwise the slot's child is marked for deletion and a new empty-slot component (from the EmptySlot definition)
// copies the child's per-state position and colour, is added in the slot and becomes the slot's base spell -- through a
// copy of the body over recording stand-in components.
#include <cstdio>
#include <map>
#include <vector>

struct State {
    float X, Y;
    unsigned int Colour;
    float Zoom;
};

struct Component {
    int id;
    int deletionMethod;
    std::map<unsigned int, State> states;
    Component(int i) : id(i), deletionMethod(0) { }
    void SetDeletion(int method) { deletionMethod = method; }
};

struct Container {
    std::vector<Component*> Children;
    std::vector<Component*> BaseSpells;
    Component* NewSpellComponent;
    unsigned int NewSpellIndex;
    int EmptySlot;
    std::vector<Component*> made;
    int addedIndex;
    Component* added;
    Container() : NewSpellComponent(0), NewSpellIndex(0), EmptySlot(77), addedIndex(-1), added(0) { }
    ~Container()
    {
        for (unsigned int i = 0; i < made.size(); ++i)
            delete made[i];
    }
    Component* CreateComponent(int definition)
    {
        Component* c = new Component(definition);
        State s0 = { 0.0f, 0.0f, 0u, 1.0f };
        c->states[0] = s0;                                   // the definition's states (keys only matter)
        c->states[2] = s0;
        made.push_back(c);
        return c;
    }
    void AddChildDynamically(Component* child, unsigned int index)
    {
        added = child;
        addedIndex = (int)index;
        Children[index] = child;
    }
    void UnAssignSpell(unsigned int slot)
    {
        if (NewSpellComponent != 0 && NewSpellIndex == slot) {
            NewSpellComponent->SetDeletion(1);
            AddChildDynamically(BaseSpells[NewSpellIndex], NewSpellIndex);
            NewSpellComponent = 0;
        }
        else {
            Children[slot]->SetDeletion(1);
            Component* emptySlot = CreateComponent(EmptySlot);
            std::map<unsigned int, State>& states = emptySlot->states;
            for (std::map<unsigned int, State>::iterator it = states.begin(); it != states.end(); ++it) {
                states[it->first].X = Children[slot]->states[it->first].X;
                states[it->first].Y = Children[slot]->states[it->first].Y;
                states[it->first].Colour = Children[slot]->states[it->first].Colour;
            }
            AddChildDynamically(emptySlot, slot);
            BaseSpells[slot] = emptySlot;
        }
    }
};

int main()
{
    Component base0(10), base1(11), pending(20);
    Container c;
    c.Children.push_back(&base0);
    c.Children.push_back(&pending);
    c.BaseSpells.push_back(&base0);
    c.BaseSpells.push_back(&base1);
    c.NewSpellComponent = &pending;
    c.NewSpellIndex = 1;
    c.UnAssignSpell(1);
    if (pending.deletionMethod != 1 || c.added != &base1 || c.addedIndex != 1 || c.NewSpellComponent != 0 ||
        c.Children[1] != &base1) {
        std::printf("00545b10_TEST FAIL\n");
        return 1;
    }
    State a = { 5.0f, 6.0f, 0xff0000ffu, 2.0f };
    State b = { 7.0f, 8.0f, 0x00ff00ffu, 3.0f };
    base0.states[0] = a;
    base0.states[2] = b;
    c.UnAssignSpell(0);
    Component* fresh = c.Children[0];
    if (base0.deletionMethod != 1 || fresh == &base0 || fresh->id != 77 || c.BaseSpells[0] != fresh ||
        c.addedIndex != 0 || fresh->states[0].X != 5.0f || fresh->states[0].Y != 6.0f ||
        fresh->states[0].Colour != 0xff0000ffu || fresh->states[0].Zoom != 1.0f || fresh->states[2].X != 7.0f ||
        fresh->states[2].Colour != 0x00ff00ffu || fresh->states.size() != 2) {
        std::printf("00545b10_TEST FAIL\n");
        return 1;
    }
    std::printf("00545b10_TEST PASS\n");
    return 0;
}