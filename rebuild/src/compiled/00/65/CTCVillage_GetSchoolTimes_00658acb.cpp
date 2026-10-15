#pragma optimize("s", on)
// CTCVillage::GetSchoolTimes(float&, float&) const -- current school start/end times from the village def's SchoolStartTimes and SchoolDuration through GetTimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658ACB, length 85 bytes (PDB `?GetSchoolTimes@CTCVillage@@QBE_NAAM0@Z`).
// Size TU (`push dword ptr [esp+14h]` pass-through, `lea eax,[ecx+94h]` shared by both copies): `optimize("s",on)`
// with the counted pointer's copy constructor, destructor and GetPDef `__forceinline` (else /Os calls them out of line).
// Callees: CTCVillage::GetTimes(const vector<long>&, long, float&, float&) const @ 0x00658716 (this still in ecx;
// manifest "GetSchoolTimes_ConvertFloatToInt" is a scratch label), the def's virtual slot 1 (destroy at zero count).
// Shape: both arguments come from GetPDef() -- the inline `CDefPointer<const CVillageDef> GetPDef() const` returns
// a counted copy by value, so each use builds an add-ref'd temporary (`test ; je ; inc [p+4]`): the duration
// (rightmost argument, evaluated first) in edi, the vector in esi; the temporaries die in reverse order after the call
// (`dec [p+4] ; jne ; call [vt+4]`), the bool result parked in bl.
// Layout: CTCVillage.h is the PDB layout (PDef_Object +0x120); with the retail one-pointer lists (15 x 8 bytes),
// the 12-byte set and two more lists the def pointer sits at +0x94, so a local model. CVillageDef (PDB, 12-byte
// retail vectors): SchoolStartTimes +0x54, SchoolDuration +0x60.
struct CLongVector {                        // retail std::vector<long>, 12 bytes
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};

struct CVillageDef {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long          RefCount;                 // +0x04
    unsigned char _pad08[0x4C];
    CLongVector   SchoolStartTimes;         // +0x54
    long          SchoolDuration;           // +0x60
};

struct CDefPointerVillageDef {
    CVillageDef* Object;
    __forceinline CDefPointerVillageDef(const CDefPointerVillageDef& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    __forceinline ~CDefPointerVillageDef()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    const CVillageDef* operator->() const { return Object; }
};

struct CTCVillage {
    void*                 __vftable;        // +0x00
    unsigned char         _pad_0x04[0x90];
    CDefPointerVillageDef PDef;             // +0x94
    __forceinline CDefPointerVillageDef GetPDef() const { return PDef; }
    bool GetTimes(const CLongVector& startTimes, long duration, float& start, float& end) const;   // 0x00658716
    bool GetSchoolTimes(float& start, float& end) const;
};

bool CTCVillage::GetSchoolTimes(float& start, float& end) const
{
    return GetTimes(GetPDef()->SchoolStartTimes, GetPDef()->SchoolDuration, start, end);
}