// CDistanceFilter_006af220::operator()(CThing* thing) -- true (1) when the vertex buffer this filter wraps (+0) passes the thing through either of its two checks (0x00833230, then 0x008332E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF220, length 49 bytes.
// Leaf: operator()
// Callees: 0x00833230 / 0x008332E0 (bool, thiscall on the wrapped object with the thing). The chunk label (CMessageEventFilter_CanBeSeenOrHeard::operator()) is propagated; named by address. Shape: the wrapped object in esi, the thing in edi, a long result (`mov eax,1`), `ret 4`.
class CThing;
struct CDistanceChecker_006af220 {
    bool DistanceCheck(CThing* thing);             // 0x00833230
    bool SurfaceCheck(CThing* thing);              // 0x008332E0
};

struct CDistanceFilter_006af220 {
    CDistanceChecker_006af220* m_pChecker;         // +0x00
    long operator()(CThing* thing);
};

long CDistanceFilter_006af220::operator()(CThing* thing)
{
    CDistanceChecker_006af220* checker = m_pChecker;
    if (checker->DistanceCheck(thing) || checker->SurfaceCheck(thing))
        return 1;
    return 0;
}