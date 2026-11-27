// Standalone C++03 behavior fixture for the exact 0054d660 source body.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>

#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "line %d: %s\n", __LINE__, #expression); exit(1); } } while (0)

class CCharString {
public:
    CCharString(const char* text) : Text(text) {}
    const char* Text;
};

template<class T> class CCountedPointer {
public:
    explicit CCountedPointer(T* value = 0) : Value(value) {}
    T* operator->() const { return Value; }
    T* Get() const { return Value; }
    template<class U> void SetStaticCast(const CCountedPointer<U>& value) { Value = static_cast<T*>(value.Get()); }
private:
    T* Value;
};

template<class T> class CDefPointer {
public:
    explicit CDefPointer(T* def) : Def(def) { ++Def->References; }
    CDefPointer(const CDefPointer& other) : Def(other.Def) { ++Def->References; }
    ~CDefPointer() { --Def->References; }
    T* operator->() const { return Def; }
private:
    T* Def;
};

template<class From, class To> To GFDebugPDynamicCast(From value, To) { return static_cast<To>(value); }

namespace NUISystem {
struct CUIDef {
    CUIDef() : PCStyle(false), UpArrow(0), DownArrow(0), UpLimit(0), DownLimit(0), MeshIndex(0), AnimationIndex(0), References(0) {}
    bool PCStyle;
    unsigned long UpArrow, DownArrow, UpLimit, DownLimit, MeshIndex, AnimationIndex;
    mutable int References;
};

class CComponent {
public:
    CComponent() : Parent(0), PositionChild(0), Calls(0) {}
    virtual ~CComponent() {}
    virtual void Initialise() { CHECK(Calls == 0); ++Calls; }
    virtual std::map<unsigned long, long>& GetStates() { return States; }
    virtual void SetParent(CComponent* parent) { CHECK(Calls == 1); ++Calls; Parent = parent; }
    virtual void AddPositionChild(CComponent* child) { CHECK(Calls == 2); ++Calls; PositionChild = child; }
    CComponent* Parent;
    CComponent* PositionChild;
    unsigned int Calls;
    std::map<unsigned long, long> States;
};

class CChangingStateComponent : public CComponent {
public:
    CChangingStateComponent() : Time(0) {}
    virtual void Initialise() { CHECK(Calls == 0); ++Calls; Time = 12.5f; }
    float Time;
};

class CListArrow : public CComponent {
public:
    CListArrow() : Minimum(-3), Maximum(-3), Limits(0) {}
    virtual void SetMinToDraw(long value) { CHECK(Calls == 0 && Limits == 0); Minimum = value; ++Limits; }
    virtual void SetMaxToDraw(long value) { CHECK(Calls == 0 && Limits == 1); Maximum = value; ++Limits; }
    virtual void Initialise() { CHECK(Limits == 2); CComponent::Initialise(); }
    long Minimum, Maximum;
    int Limits;
};

class CScrollBarOutside : public CComponent {
public:
    CScrollBarOutside() : Bar(0) {}
    virtual void SetScrollBar(CComponent* bar) { CHECK(Calls == 2); CHECK(Bar == 0); Bar = bar; }
    CComponent* Bar;
};

class CManager {
public:
    static CManager* GetInstance() { return Instance; }
    CComponent* CreateComponent(long index) {
        CreationOrder.push_back(index);
        switch (index) {
        case 11: return &Up;
        case 12: return &Down;
        case 13: return &Bar;
        case 14: return &Outside;
        }
        abort();
    }
    CComponent* CreateComponent(const CCharString& name, bool flag) {
        CHECK(strcmp(name.Text, "PC_LIST_SCROLL_BAR_POSITION_DUMMY") == 0 && !flag);
        CHECK(CreationOrder.size() && (CreationOrder.back() == 13 || CreationOrder.back() == 14));
        unsigned long kind = CreationOrder.back();
        CreationOrder.push_back(-1);
        return kind == 13 ? &BarDummy : &OutsideDummy;
    }
    static CManager* Instance;
    std::vector<long> CreationOrder;
    CListArrow Up, Down;
    CComponent Bar, BarDummy, OutsideDummy;
    CScrollBarOutside Outside;
};
CManager* CManager::Instance = 0;

class CFrontEndList : public CChangingStateComponent {
public:
    long SelectedChild;
    CFrontEndList() : SelectedChild(-10), LastInputTime(-1), PListUp(0), PListDown(0), PScrollBar(0), PScrollBarOutside(0), DefLoads(0) {}
    virtual void Initialise();
    virtual void SetSelectedChild(long child) { CHECK(Calls == 1); Selection.push_back(child); }
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() { ++DefLoads; return CDefPointer<const CUIDef>(&Definition); }
    float LastInputTime;
    std::vector<CComponent*> ScrollingChildren, NonScrollingChildren;
    std::vector<std::map<unsigned long, long> > ScrollingChildrenStates;
    std::vector<CCountedPointer<CComponent> > Children;
    CComponent* PListUp;
    CComponent* PListDown;
    CComponent* PScrollBar;
    CComponent* PScrollBarOutside;
    CUIDef Definition;
    unsigned int DefLoads;
    std::vector<long> Selection;
};
}

void NUISystem::CFrontEndList::Initialise()
{
    CChangingStateComponent::Initialise();
    for (unsigned long count = 0; count < ScrollingChildren.size(); count++)
        ScrollingChildrenStates.push_back(ScrollingChildren[count]->GetStates());
    LastInputTime = Time;
    SelectedChild = 0;
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    CCountedPointer<CListArrow> pchild_up;
    if (pdefinition->UpArrow) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->UpArrow)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        pchild_up.SetStaticCast(Children[Children.size() - 1]);
        pchild_up->SetMinToDraw(pdefinition->UpLimit);
        pchild_up->SetMaxToDraw(ScrollingChildren.size());
        pchild_up->Initialise();
        Children[Children.size() - 1]->SetParent(this);
    }
    CCountedPointer<CListArrow> pchild_down;
    if (pdefinition->DownArrow) {
        Children.push_back(CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent(pdefinition->DownArrow)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].Get());
        pchild_down.SetStaticCast(Children[Children.size() - 1]);
        pchild_down->SetMinToDraw(0);
        pchild_down->SetMaxToDraw(ScrollingChildren.size() - pdefinition->DownLimit - 1);
        pchild_down->Initialise();
        Children[Children.size() - 1]->SetParent(this);
    }
}

static void CheckInitialisation(unsigned int bits, bool pc_style, unsigned int count)
{
    using namespace NUISystem;
    CManager manager;
    CManager::Instance = &manager;
    CFrontEndList list;
    CComponent children[5];
    list.Definition.PCStyle = pc_style;
    list.Definition.UpArrow = (bits & 1) ? 11 : 0;
    list.Definition.DownArrow = (bits & 2) ? 12 : 0;
    list.Definition.MeshIndex = (bits & 4) ? 13 : 0;
    list.Definition.AnimationIndex = (bits & 8) ? 14 : 0;
    list.Definition.UpLimit = 4;
    list.Definition.DownLimit = 2;
    for (unsigned int i = 0; i < count; ++i) {
        children[i].States[99] = i * 3;
        list.ScrollingChildren.push_back(&children[i]);
        list.Children.push_back(CCountedPointer<CComponent>(&children[i]));
    }
    list.Initialise();
    CHECK(list.Calls == 1 && list.LastInputTime == 12.5f);
    CHECK(list.Definition.References == 0 && list.DefLoads == 1U);
    CHECK(list.ScrollingChildrenStates.size() == count);
    for (unsigned int i = 0; i < count; ++i) {
        CHECK(list.ScrollingChildrenStates[i] == children[i].States);
        CHECK(list.Children[i].Get() == &children[i] && children[i].Calls == 0);
    }
    CHECK(list.Selection.empty() && list.SelectedChild == 0);

    std::vector<CComponent*> expected;
    std::vector<long> creations;
    if (bits & 1) {
        expected.push_back(&manager.Up);
        creations.push_back(11);
        CHECK(list.PListUp == 0 && manager.Up.Minimum == 4 && manager.Up.Maximum == count);
    } else {
        CHECK(list.PListUp == 0 && manager.Up.Calls == 0);
    }
    if (bits & 2) {
        expected.push_back(&manager.Down);
        creations.push_back(12);
        CHECK(list.PListDown == 0 && manager.Down.Minimum == 0 && manager.Down.Maximum == static_cast<long>(count) - 3);
    } else {
        CHECK(list.PListDown == 0 && manager.Down.Calls == 0);
    }
    CHECK(!list.PScrollBar && !list.PScrollBarOutside && !manager.Bar.Calls && !manager.Outside.Calls);
    CHECK(manager.CreationOrder == creations && list.NonScrollingChildren == expected);
    CHECK(list.Children.size() == count + expected.size());
    for (unsigned int i = 0; i < expected.size(); ++i) {
        CHECK(list.Children[i + count].Get() == expected[i]);
        CHECK(expected[i]->Parent == &list && expected[i]->Calls >= 2);
    }
}

int main()
{
    static const unsigned int counts[] = {0, 1, 2, 5};
    for (unsigned int mask = 0; mask < 16; ++mask)
        for (unsigned int pc = 0; pc < 2; ++pc)
            for (unsigned int i = 0; i < sizeof(counts) / sizeof(counts[0]); ++i)
                CheckInitialisation(mask, pc != 0, counts[i]);
    puts("0054d660_TEST PASS");
    return 0;
}