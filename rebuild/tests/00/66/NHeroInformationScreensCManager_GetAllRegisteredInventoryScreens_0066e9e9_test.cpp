#include <cstdio>
class CWorld;
namespace NHeroInformationScreens {
class CBase {
public:
    virtual void _v00() const;
    virtual void _v01() const;
    virtual void _v02() const;
    virtual bool IsDisplayable() const;                  // slot 3 (+0x0c, name not confirmed)
};
}
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T* erase(T* first, T* last);                         // 0x0066ECE6 for const CBase*
    void push_back(const T& x);                          // 0x0066EDA6 for const CBase*
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
namespace NHeroInformationScreens {
class CManager {
public:
    CWorld*                         World;               // +0x00
    std::vector<const CBase*>       Screens;             // +0x04
    std::vector<const CBase*>       InventoryScreens;    // +0x10
    std::vector<long>               ScrollBarIndices;    // +0x1c
private:
    void GetAllRegisteredInventoryScreens(std::vector<const CBase*>& screens) const;
    friend struct CManager_Test;                         // behaviour-test hook
};
void CManager::GetAllRegisteredInventoryScreens(std::vector<const CBase*>& screens) const
{
    screens.clear();
    const long count = Screens.size();
    for (long i = 0; i < count; ++i)
        if (Screens[i] && Screens[i]->IsDisplayable())
            screens.push_back(Screens[i]);
}
}

namespace NHeroInformationScreens {
void CBase::_v00() const {} void CBase::_v01() const {} void CBase::_v02() const {}
bool CBase::IsDisplayable() const { return false; }
class CTestScreen : public CBase { public: bool D; bool IsDisplayable() const { return D; } };
struct CManager_Test {
    static void Run(const CManager& m, std::vector<const CBase*>& out) { m.GetAllRegisteredInventoryScreens(out); }
};
}
namespace std {
template <class T, class A> T* vector<T, A>::erase(T* first, T* last) { T* d = first; for (T* s = last; s != _M_finish; ++s) *d++ = *s; _M_finish = d; return first; }
template <class T, class A> void vector<T, A>::push_back(const T& x) { *_M_finish++ = x; }
}
using namespace NHeroInformationScreens;
int main()
{
    CTestScreen s[4]; s[0].D = true; s[1].D = false; s[2].D = true; s[3].D = true;
    const CBase* reg[5] = { &s[0], &s[1], 0, &s[2], &s[3] };
    const CBase* outbuf[8] = { &s[1], &s[1] };
    CManager m; m.Screens._M_start = reg; m.Screens._M_finish = reg + 5; m.Screens._M_end_of_storage = reg + 5;
    std::vector<const CBase*> out; out._M_start = outbuf; out._M_finish = outbuf + 2; out._M_end_of_storage = outbuf + 8;
    CManager_Test::Run(m, out);
    bool ok = out._M_finish == outbuf + 3 && outbuf[0] == &s[0] && outbuf[1] == &s[2] && outbuf[2] == &s[3];
    m.Screens._M_finish = reg; CManager_Test::Run(m, out);
    ok = ok && out._M_finish == outbuf;
    std::printf(ok ? "0066e9e9_TEST PASS\n" : "0066e9e9_TEST FAIL\n");
    return ok ? 0 : 1;
}