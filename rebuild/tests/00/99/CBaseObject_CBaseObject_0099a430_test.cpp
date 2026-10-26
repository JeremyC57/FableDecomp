#include <cstdio>
class CBaseObject;
struct CBaseObjectPointer { CBaseObject* object; unsigned long refcount; };
static int g_freed = 0;
static void Free(CBaseObjectPointer* p) { ++g_freed; delete p; }
class CBaseClass { public: virtual void v00() { } ~CBaseClass() { } };
class CBaseObject : public CBaseClass { public:
    CBaseObjectPointer* IntelligentPointer;
    virtual void v00() { }
    ~CBaseObject()
    {
        if (IntelligentPointer) {
            IntelligentPointer->object = 0;
            if (IntelligentPointer->refcount == 0)
                Free(IntelligentPointer);
            IntelligentPointer = 0;
        }
    }
};
int main()
{
    CBaseObjectPointer* kept = new CBaseObjectPointer;
    {
        CBaseObject o; kept->object = &o; kept->refcount = 1; o.IntelligentPointer = kept;
    }
    if (kept->object != 0 || g_freed != 0) { std::printf("0099a430_TEST FAIL\n"); return 1; }
    {
        CBaseObject o; CBaseObjectPointer* p = new CBaseObjectPointer; p->object = &o; p->refcount = 0; o.IntelligentPointer = p;
    }
    if (g_freed != 1) { std::printf("0099a430_TEST FAIL\n"); return 1; }
    { CBaseObject o; o.IntelligentPointer = 0; }
    if (g_freed != 1) { std::printf("0099a430_TEST FAIL\n"); return 1; }
    delete kept;
    std::printf("0099a430_TEST PASS\n");
    return 0;
}