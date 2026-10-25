// CTCVillage::GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const -- for HHMM start times and an HHMM duration, report whether a session is running now (timeUntilStart 0, endTime its end) or else the day fraction until the nearest next start (today or tomorrow) and that session's end
// Leaf: ?GetTimes@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658716, 441 bytes, unaligned in the /Os frame-pointer CTCVillage TU, thiscall
//   `ret 10h`, always returns true. Ego_r.pdb name set: ?GetTimes@CTCVillage@@ABE_NABV?$vector@JV?$allocator@J@std@@@std@@JAAM1@Z
//   (private const); its caller is the landed CTCVillage::GetSchoolTimes 0x00658ACB, whose header already names it.
//   Catalog label "GetSchoolTimes_ConvertFloatToInt" is a scratch label.
// Callees: CTCBase::PeekWorld() const 0x00686A80 (this still in ecx), _ftol 0x00BFEA70 (the `(long)end` cast).
//   Constants: 1/1440 float 0x0125A88C (minutes to day fraction), 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0.
// Layout: CWorld PGameTimeManager +0x1C (scoped_ptr), CGameTimeManager Time +0x08 (PDB = retail).
// Shape: /Os + no frame-pointer omission: 2 dword locals (now, length), `best` homed in the dead endTime slot [ebp+14]
//   (needs it declared after the two zero stores), the converted start in the dead duration slot, the _ftol result in
//   the dead startTimes slot. HHMM -> minutes is `t / 100 * 60 + t % 100` (cdq/idiv by `push 64h ; pop ecx`), then
//   fild/fmul. The callee-saved tie (it esi, startTimes edi, endTime ebx) needs the time manager read through a const
//   pointer local: `PeekWorld().PGameTimeManager->Time` in one expression gives endTime edi / startTimes ebx. The two
//   in-session returns tail-merge at the end; the today/tomorrow candidate blocks keep 1.0 on the x87 stack (`fsubr
//   now` builds now - 1). `best == -1.0f` is the fucompp/`test ah,44h` equality form.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}

class CGameTimeManager {
public:
    class CWorld* World;                               // +0x00
    void*         GameComponent;                       // +0x04
    float         Time;                                // +0x08 time of day, 0..1
};
class CWorld {
public:
    unsigned char     _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;                // +0x1C scoped_ptr<CGameTimeManager>
};
class CTCBase {
public:
    const CWorld& PeekWorld() const;                   // 0x00686A80
};

static inline float HHMMToDayFraction(long time)
{
    return (time / 100 * 60 + time % 100) * (1.0f / 1440.0f);
}

class CTCVillage : public CTCBase {
public:
    bool GetSchoolTimes(float& start, float& end) const;
private:
    bool GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const;
};

bool CTCVillage::GetTimes(const std::vector<long>& startTimes, long duration, float& timeUntilStart, float& endTime) const
{
    const CGameTimeManager* timeManager = PeekWorld().PGameTimeManager;
    float now = timeManager->Time;
    float length = HHMMToDayFraction(duration);
    timeUntilStart = 0.0f;
    endTime = 0.0f;
    float best = -1.0f;
    for (std::vector<long>::const_iterator it = startTimes.begin(); it != startTimes.end(); ++it) {
        float start = HHMMToDayFraction(*it);
        float end = start + length;
        if (end < 1.0f) {
            if (now > start && now < end) {
                timeUntilStart = 0.0f;
                endTime = end;
                return true;
            }
        }
        if (end > 1.0f) {
            float wrappedEnd = end - (long)end;
            if ((now > start && now < 1.0f) || (now > 0.0f && now < wrappedEnd)) {
                timeUntilStart = 0.0f;
                endTime = end;
                return true;
            }
        }
        if (now < start) {
            if (best == -1.0f || start - now < best) {
                best = start - now;
                endTime = end;
            }
        }
        if (now - 1.0f < start) {
            if (best == -1.0f || start - (now - 1.0f) < best) {
                best = start - (now - 1.0f);
                endTime = end;
            }
        }
    }
    timeUntilStart = best;
    if (endTime > 1.0f)
        endTime -= 1.0f;
    return true;
}