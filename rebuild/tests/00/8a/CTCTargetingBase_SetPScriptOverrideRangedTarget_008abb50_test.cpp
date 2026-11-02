// Behaviour test for 0x008abb50 CTCTargetingBase::SetPScriptOverrideRangedTarget:
// the member intelligent pointer ends up holding the new item; the temporary pointer is constructed, read and destroyed once.
#include <cstdio>
static int g_ctors = 0;
static int g_dtors = 0;
static int g_sets = 0;
struct BaseObject { int Id; };
class BaseIntelligentPointer {
public:
    BaseIntelligentPointer() : PItem(0) { ++g_ctors; }
    virtual ~BaseIntelligentPointer() { ++g_dtors; }
    BaseObject* GetPItem() const { return PItem; }
    void SetPItem(BaseObject* item) { ++g_sets; PItem = item; }
private:
    BaseObject* PItem;
};
template <class T> class IntelligentPointer : public BaseIntelligentPointer {
public:
    IntelligentPointer() { }
    IntelligentPointer(T* item) { SetPItem(item); }
    IntelligentPointer& operator=(const IntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct Owner {
    bool Flag;
    IntelligentPointer<BaseObject> Member;
    void Set(BaseObject* thing) { Member = thing; }
};
int main()
{
    BaseObject a = { 1 }, b = { 2 };
    Owner o;
    o.Flag = false;
    int ctors0 = g_ctors, dtors0 = g_dtors;
    o.Set(&a);
    if (o.Member.GetPItem() != &a || g_ctors - ctors0 != 1 || g_dtors - dtors0 != 1 || g_sets != 2) { std::printf("008abb50_TEST FAIL\n"); return 1; }
    o.Set(&b);
    if (o.Member.GetPItem() != &b || g_ctors - ctors0 != 2 || g_dtors - dtors0 != 2 || g_sets != 4) { std::printf("008abb50_TEST FAIL\n"); return 1; }
    o.Set(0);
    if (o.Member.GetPItem() != 0) { std::printf("008abb50_TEST FAIL\n"); return 1; }
    std::printf("008abb50_TEST PASS\n");
    return 0;
}