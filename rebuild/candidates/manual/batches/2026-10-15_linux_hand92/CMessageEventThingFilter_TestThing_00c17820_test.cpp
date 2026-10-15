#include <cstdio>
// Behaviour test for CMessageEventThingFilter::TestThing (0x00C17820): same logic over a copy of the model.
struct CThing { int valid; };
struct CMessageEvent {
    long m_type; CThing* m_pThing;
    CMessageEvent(CThing* thing) : m_type(0), m_pThing(thing) { }
};
struct CMessageEventThingFilter {
    int calls;
    bool Test(CMessageEvent& event) { ++calls; return event.m_type == 0 && event.m_pThing != 0 && event.m_pThing->valid != 0; }
    bool TestThing(CThing* thing) { CMessageEvent event(thing); return Test(event); }
};
int main()
{
    CThing good = { 1 }, bad = { 0 };
    CMessageEventThingFilter f; f.calls = 0;
    if (!f.TestThing(&good) || f.TestThing(&bad) || f.TestThing(0) || f.calls != 3) { std::printf("00c17820_TEST FAIL\n"); return 1; }
    std::printf("00c17820_TEST PASS\n");
    return 0;
}
