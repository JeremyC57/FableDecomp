// Behaviour test for CEngineInternalPrimitiveChanger::SetPosition (0xbc5660): forward the argument to every non-null child primitive.
#include <cstdio>
struct Arg { int v; };
struct Child { int got; virtual void SetPosition(const Arg& a) { got = a.v; } virtual ~Child() {} };
struct Handle { Child* Object; bool IsNull() const { return Object == 0; } Child& GetData() const { return *Object; } };
struct Changer { Handle* start; Handle* finish;
    void SetPosition(const Arg& a) { unsigned int count = (unsigned int)(finish - start);
        for (unsigned int i = 0; i < count; ++i) { Handle& h = start[i]; if (!h.IsNull()) h.GetData().SetPosition(a); } } };
int main()
{
    Child c1, c2; c1.got = c2.got = 0; Handle hs[3]; hs[0].Object = &c1; hs[1].Object = 0; hs[2].Object = &c2;
    Changer ch; ch.start = hs; ch.finish = hs + 3; Arg a = { 17 };
    ch.SetPosition(a);
    if (c1.got != 17 || c2.got != 17) { std::printf("00bc5660_TEST FAIL\n"); return 1; }
    ch.finish = hs; a.v = 3; ch.SetPosition(a); if (c1.got != 17) { std::printf("00bc5660_TEST FAIL\n"); return 1; }
    std::printf("00bc5660_TEST PASS\n");
    return 0;
}