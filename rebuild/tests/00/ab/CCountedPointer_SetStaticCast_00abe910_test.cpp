// Behaviour test for 0x00ABE910 CCountedPointer<T>::SetStaticCast<U>(const CCountedPointer<U>&)
// Model: share the other pointer's data and count block -- release the old block (destroy + free on the last reference), add a reference to the new one.
#include <cstdio>
static int g_deleted = 0;
static void DeleteInt(void* data) { ++g_deleted; (void)data; }
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
class Base { public: int V; };
class Derived : public Base { };
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                if (--Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other) { ShareData(static_cast<T*>(other.Data), other.Info); }
};
int main()
{
    Derived d1, d2;
    CCPPointerInfo* i1 = new CCPPointerInfo; i1->RefCount = 1; i1->DeleteFunc = DeleteInt; i1->Data = &d1;
    CCPPointerInfo* i2 = new CCPPointerInfo; i2->RefCount = 1; i2->DeleteFunc = DeleteInt; i2->Data = &d2;
    CCountedPointer<Derived> target; target.Data = &d1; target.Info = i1;
    CCountedPointer<Base> source; source.Data = &d2; source.Info = i2;
    target.SetStaticCast(source);                          // i1 drops to 0 -> deleted; i2 gains a reference
    if (g_deleted != 1 || target.Data != &d2 || target.Info != i2 || i2->RefCount != 2) { std::printf("00abe910_TEST FAIL\n"); return 1; }
    target.SetStaticCast(source);                          // same block: no change
    if (i2->RefCount != 2) { std::printf("00abe910_TEST FAIL\n"); return 1; }
    CCountedPointer<Base> empty; empty.Data = 0; empty.Info = 0;
    target.SetStaticCast(empty);                           // i2 back to 1, target cleared
    if (i2->RefCount != 1 || target.Info != 0 || target.Data != 0 || g_deleted != 1) { std::printf("00abe910_TEST FAIL\n"); return 1; }
    delete i2;
    std::printf("00abe910_TEST PASS\n");
    return 0;
}