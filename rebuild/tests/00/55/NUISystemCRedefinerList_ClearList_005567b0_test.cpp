// test: NUISystem::CRedefinerList::ClearList -- RemoveChild every non-null child (removal shifts the next one into place), skip nulls, reset counts
#include <stdio.h>
#include <vector>
static int resets = 0, removes = 0;
struct CountedPtr { int* p; CountedPtr(int* q) : p(q) { } bool IsNull() const { return p == 0; } };
static void ResetButtonCounts() { ++resets; }
struct Component {
    std::vector<CountedPtr> Children;
    virtual ~Component() { }
    virtual void RemoveChild(unsigned int index) { ++removes; Children.erase(Children.begin() + index); }
};
struct RedefinerList : Component {
    void ClearList() {
        for (unsigned int i = 0; i < Children.size(); ) {
            if (!Children[i].IsNull()) RemoveChild(i); else ++i;
        }
        ResetButtonCounts();
    }
};
int main() {
    int a = 1, b = 2, c = 3;
    RedefinerList l;
    l.Children.push_back(CountedPtr(&a)); l.Children.push_back(CountedPtr(0)); l.Children.push_back(CountedPtr(&b));
    l.Children.push_back(CountedPtr(&c)); l.Children.push_back(CountedPtr(0));
    l.ClearList();
    if (removes != 3 || resets != 1 || l.Children.size() != 2 || !l.Children[0].IsNull() || !l.Children[1].IsNull()) { printf("005567b0_TEST FAIL\n"); return 1; }
    printf("005567b0_TEST PASS\n");
    return 0;
}