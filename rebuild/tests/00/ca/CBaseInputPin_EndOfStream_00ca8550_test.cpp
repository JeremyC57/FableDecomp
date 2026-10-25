// Behaviour test for 00ca8550: CBaseInputPin::EndOfStream -- always S_OK, the pin untouched
#include <cstdio>
struct IPinish { virtual long __stdcall EndOfStream() = 0; };
class CBaseInputPin : public IPinish { public: int m_state; long __stdcall EndOfStream() { return 0; } };
int main()
{
    CBaseInputPin pin; pin.m_state = 5; IPinish* p = &pin;
    int ok = p->EndOfStream() == 0 && pin.EndOfStream() == 0 && pin.m_state == 5;
    std::printf(ok ? "00ca8550_TEST PASS\n" : "00ca8550_TEST FAIL\n");
    return ok ? 0 : 1;
}
