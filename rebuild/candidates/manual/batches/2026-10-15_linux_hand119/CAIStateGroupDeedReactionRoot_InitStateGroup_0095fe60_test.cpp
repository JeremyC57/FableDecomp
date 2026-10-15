// Behaviour test for 0x0095FE60: InitStateGroup resets every reaction field.
#include <cstdio>
struct CThing { int id; };
struct CIP { CThing* p; };
struct CRoot { CIP ThingReactingTo; long CurrentDeedType; long CurrentDeedHandle; bool DeedIsVirtual;
    unsigned long LastUIDLo, LastUIDHi; long LastOpinionSourceVirtualDeedID; bool DeliveredReaction; bool DeliveredSpeech;
    void Init()
    {
        DeliveredReaction = false; DeliveredSpeech = false;
        CIP none = { 0 }; ThingReactingTo = none;
        CurrentDeedType = 0; CurrentDeedHandle = -1; DeedIsVirtual = false;
        LastUIDLo = 0; LastUIDHi = 0; LastOpinionSourceVirtualDeedID = -1;
    }
};
int main()
{
    CThing t = { 1 };
    CRoot r; r.ThingReactingTo.p = &t; r.CurrentDeedType = 5; r.CurrentDeedHandle = 7; r.DeedIsVirtual = true;
    r.LastUIDLo = 3; r.LastUIDHi = 4; r.LastOpinionSourceVirtualDeedID = 9; r.DeliveredReaction = r.DeliveredSpeech = true;
    r.Init();
    if (r.ThingReactingTo.p || r.CurrentDeedType || r.CurrentDeedHandle != -1 || r.DeedIsVirtual || r.LastUIDLo || r.LastUIDHi
        || r.LastOpinionSourceVirtualDeedID != -1 || r.DeliveredReaction || r.DeliveredSpeech) {
        std::printf("0095fe60_TEST FAIL\n"); return 1;
    }
    std::printf("0095fe60_TEST PASS\n");
    return 0;
}
