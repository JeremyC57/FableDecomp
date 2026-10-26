#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCCameraPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    ((CTCBase*)(building))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(building);
    return building;
}