// Behaviour test for boost::scoped_ptr<CPhysicsForceModelBase>::~scoped_ptr (0x00850530): destroys and frees the owned object, tolerates null.
#include <cstdio>
static int g_destroyed = 0;
namespace NCameraModes {
class CPhysicsForceModelBase {
public:
    virtual void FrameUpdate() {}
    ~CPhysicsForceModelBase() { ++g_destroyed; }
    int body[4];
};
}
namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}
template <class T> class scoped_ptr {
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
private:
    T* ptr;
};
}
int main()
{
    {
        boost::scoped_ptr<NCameraModes::CPhysicsForceModelBase> p(new NCameraModes::CPhysicsForceModelBase);
    }
    if (g_destroyed != 1) { std::printf("00850530_TEST FAIL\n"); return 1; }
    {
        boost::scoped_ptr<NCameraModes::CPhysicsForceModelBase> q;
    }
    if (g_destroyed != 1) { std::printf("00850530_TEST FAIL\n"); return 1; }
    std::printf("00850530_TEST PASS\n");
    return 0;
}