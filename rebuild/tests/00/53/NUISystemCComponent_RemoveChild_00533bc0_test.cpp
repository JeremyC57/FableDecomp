// Behaviour test for 0x00533BC0 NUISystem::CComponent::RemoveChild(unsigned long index)
#include <cstdio>
#include <cstring>

struct CComponent;
struct CPtr {
    CComponent* Data;
    int* Info;
};
struct PtrVec {
    CPtr Items[8];
    unsigned int Count;
    CPtr* begin() { return Items; }
    CPtr* end() { return Items + Count; }
    void push_back(const CPtr& p) { Items[Count++] = p; }
    void erase(CPtr* pos) { for (CPtr* q = pos + 1; q != end(); ++q) q[-1] = *q; --Count; }
};
struct IndexVec {
    unsigned int Items[8];
    unsigned int Count;
    unsigned int* begin() { return Items; }
    unsigned int* end() { return Items + Count; }
    void erase(unsigned int* pos)
    {
        if (pos + 1 != end())
            std::memmove(pos, pos + 1, (char*)end() - (char*)(pos + 1));
        --Count;
    }
};
static CPtr* FindPtr(CPtr* first, CPtr* last, const CPtr& v)
{
    for (; first != last; ++first) if (first->Data == v.Data) return first;
    return last;
}
static unsigned int* FindIndex(unsigned int* first, unsigned int* last, const unsigned int& v)
{
    for (; first != last; ++first) if (*first == v) return first;
    return last;
}
struct CComponent {
    int Died;
    PtrVec Children;
    PtrVec ChildrenToDelete;
    IndexVec ShapeChildren;
    void Die() { ++Died; }
    void RemoveChild(unsigned int index)
    {
        CPtr* child = FindPtr(Children.begin(), Children.end(), Children.Items[index]);
        child->Data->Die();
        ChildrenToDelete.push_back(*child);
        Children.erase(child);
        unsigned int* shape = FindIndex(ShapeChildren.begin(), ShapeChildren.end(), index);
        if (shape != ShapeChildren.end())
            ShapeChildren.erase(shape);
        for (unsigned int i = 0; i < ShapeChildren.Count; ++i)
            if (ShapeChildren.Items[i] > index)
                --ShapeChildren.Items[i];
    }
};

int main()
{
    static CComponent parent, a, b, c;
    CPtr pa = { &a, 0 }, pb = { &b, 0 }, pc = { &c, 0 };
    parent.Children.push_back(pa); parent.Children.push_back(pb); parent.Children.push_back(pc);
    parent.ShapeChildren.Items[0] = 0; parent.ShapeChildren.Items[1] = 1; parent.ShapeChildren.Items[2] = 2;
    parent.ShapeChildren.Count = 3;
    parent.RemoveChild(1);
    if (b.Died != 1 || a.Died != 0 || c.Died != 0) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    if (parent.Children.Count != 2 || parent.Children.Items[0].Data != &a || parent.Children.Items[1].Data != &c) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    if (parent.ChildrenToDelete.Count != 1 || parent.ChildrenToDelete.Items[0].Data != &b) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    if (parent.ShapeChildren.Count != 2 || parent.ShapeChildren.Items[0] != 0 || parent.ShapeChildren.Items[1] != 1) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    parent.RemoveChild(0);
    if (a.Died != 1 || parent.Children.Count != 1 || parent.Children.Items[0].Data != &c) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    if (parent.ShapeChildren.Count != 1 || parent.ShapeChildren.Items[0] != 0) { std::printf("00533bc0_TEST FAIL\n"); return 1; }
    std::printf("00533bc0_TEST PASS\n");
    return 0;
}