#pragma optimize("s",on)
struct CThing;

class CAIStateGroup_MinionSitOnSeat
{
public:
    bool GetPOwnerObject(CThing** out);
};

bool __fastcall CAIStateGroup_MinionSitOnSeat_GetPOwnerObject(CAIStateGroup_MinionSitOnSeat* stateGroup, void* /*unused_edx*/, CThing** out)
{
    *out = 0;
    return true;
}