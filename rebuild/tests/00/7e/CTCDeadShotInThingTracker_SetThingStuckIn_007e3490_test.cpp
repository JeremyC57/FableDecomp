// Behaviour test for CTCDeadShotInThingTracker::SetThingStuckIn(const CThing&) @ 0x007E3490: the member pointer ends up
// holding the thing (through a temporary) and HasThingStuckIn is set; the temporary is destroyed.
#include <cstdio>
class CBaseObject { public: int id; };
class CThing : public CBaseObject { public: int extra; };
static int g_dtors = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* PData;
    CBaseIntelligentPointer() : PData(0) { }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() const { return static_cast<CBaseObject*>(PData); }
    void SetPItem(CBaseObject* item) { PData = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(const CThing* item = 0) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CTCDeadShotInThingTracker { public:
    virtual void v00() { }
    unsigned char       _base[0x8];
    CIntelligentPointer PThingStuckIn;
    bool                HasThingStuckIn;
    void SetThingStuckIn(const CThing& thing);
};
void CTCDeadShotInThingTracker::SetThingStuckIn(const CThing& thing)
{
    PThingStuckIn = CIntelligentPointer(&thing);
    HasThingStuckIn = true;
}
int main()
{
    int result = 0;
    {
        CTCDeadShotInThingTracker t; t.HasThingStuckIn = false; CThing wall;
        t.SetThingStuckIn(wall);
        if (t.PThingStuckIn.GetPItem() != static_cast<CBaseObject*>(&wall) || !t.HasThingStuckIn || g_dtors != 1) result = 1;
    }
    if (result) { std::printf("007e3490_TEST FAIL\n"); return 1; }
    std::printf("007e3490_TEST PASS\n");
    return 0;
}