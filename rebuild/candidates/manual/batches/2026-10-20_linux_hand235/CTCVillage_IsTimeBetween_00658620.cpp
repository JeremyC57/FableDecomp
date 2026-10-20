// CTCVillage::IsTimeBetween(const vector<long>& startTimes, long duration) const -- true when the game time of day lies inside any window that starts at one of the HHMM start times and lasts the HHMM duration (a window running past midnight wraps)
// Leaf: IsTimeBetween
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658620, 246 bytes (`ret 8`; Ego_r.pdb `?IsTimeBetween@CTCVillage@@QBE_NABV?$vector@J...@@J@Z`).
// Callees: CTCBase::GetWorld() const 0x00686A80; __ftol2 0x00BFEA70 (the wrap's `(long)end`).
// Constants: 1/1440 (minutes to a day fraction) 0x0125A88C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout (retail): CWorld::PGameTimeManager +0x1c, CGameTimeManager::Time +0x08 (as the landed CTCTimeAppearanceFade
//          FrameUpdate 0x007786F0).
// Shape: size TU with a frame pointer (`push 64h ; pop ecx`, `leave`); the HHMM conversion writes the minutes back into
//        the `duration` parameter slot (and reuses that slot as each start time's conversion temporary); the time and
//        the constant 1.0f stay on the x87 stack across the loop; the windows are three `if ... return true;`
//        statements in retail's order, the wrapped end `end - (long)end` stored in the dead parameter slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CGameTimeManager {
    void*  World;                               // +0x00
    void*  GameComponent;                       // +0x04
    float  Time;                                // +0x08
};
struct CWorld {
    unsigned char     _pad_0x00[0x1c];
    CGameTimeManager* PGameTimeManager;         // +0x1c
};
struct CLongVector {                            // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
};

struct CTCVillage {
    virtual void v00();
    const CWorld* GetWorld() const;             // 0x00686A80 (CTCBase)
    bool IsTimeBetween(const CLongVector& startTimes, long duration) const;
};

bool CTCVillage::IsTimeBetween(const CLongVector& startTimes, long duration) const
{
    float time = GetWorld()->PGameTimeManager->Time;
    duration = duration / 100 * 60 + duration % 100;
    float length = duration * (1.0f / 1440.0f);
    for (const long* it = startTimes.begin(); it != startTimes.end(); ++it) {
        long minutes = *it / 100 * 60 + *it % 100;
        float start = minutes * (1.0f / 1440.0f);
        float end = length + start;
        if (end <= 1.0f && time > start && time < end)
            return true;
        if (end > 1.0f) {
            float wrapped = end - (long)end;
            if (time > start && time < 1.0f)
                return true;
            if (time > 0.0f && time < wrapped)
                return true;
        }
    }
    return false;
}
