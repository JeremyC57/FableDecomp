// Behaviour test for 0x00533BA0 NUISystem::CComponent::AddChildFront: inserts the child pointer at the front of the Children vector
#include <cstdio>
class CComponent;
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* insert(T* position, const T& x)
    {
        T* p;
        for (p = _M_finish; p != position; --p)
            *p = *(p - 1);
        *position = x;
        ++_M_finish;
        return position;
    }
};
class CComponent {
public:
    virtual void AddChildFront(CCountedPointer<CComponent>& child);
    virtual ~CComponent() { }
    CRetailVector<CCountedPointer<CComponent> > Children;
};
void CComponent::AddChildFront(CCountedPointer<CComponent>& child)
{
    Children.insert(Children.begin(), child);
}
int main()
{
    CCountedPointer<CComponent> store[4];
    CComponent parent, a, b;
    parent.Children._M_start = store; parent.Children._M_finish = store; parent.Children._M_end_of_storage = store + 4;
    CCountedPointer<CComponent> pa; pa.Data = &a; pa.Info = 0;
    CCountedPointer<CComponent> pb; pb.Data = &b; pb.Info = 0;
    parent.AddChildFront(pa);
    parent.AddChildFront(pb);
    if (parent.Children._M_finish != store + 2 || store[0].Data != &b || store[1].Data != &a)
        { std::printf("00533ba0_TEST FAIL\n"); return 1; }
    std::printf("00533ba0_TEST PASS\n");
    return 0;
}