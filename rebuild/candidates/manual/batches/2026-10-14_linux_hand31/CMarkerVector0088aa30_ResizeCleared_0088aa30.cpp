// CMarkerVector_0088aa30::ResizeCleared(unsigned int count) -- resize the marker vector to `count` copies of a zeroed 12-byte marker, then run the `__fastcall` fix-up @ 0x006B67A0 with two nulls and the count's address
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088AA30, length 50 bytes.
// Callees: vector<CUIMiniMapMarker>::resize(unsigned int, const CUIMiniMapMarker&) @ 0x0088A9C0 (on `this`), the fix-up @ 0x006B67A0 (`__fastcall`: ecx = 0,
// edx = 0, stack: the address of the count argument's slot; manifest "_Copy_backward<C3DVertex2*,...>" is propagated). Shape: the 12-byte local is zeroed
// through eax before its address and the count are pushed; the argument slot is live (its address escapes). The chunk label "CEngine::ClipBox" is propagated;
// the class is named by address.
struct CUIMiniMapMarker_0088aa30 {
    long m_a, m_b, m_c;
};

void __fastcall FixUpMarkers_006b67a0(void* first, void* last, unsigned int* pCount);   // 0x006B67A0

struct CMarkerVector_0088aa30 {
    void resize(unsigned int count, const CUIMiniMapMarker_0088aa30& value);   // 0x0088A9C0
    void ResizeCleared(unsigned int count);
};

void CMarkerVector_0088aa30::ResizeCleared(unsigned int count)
{
    CUIMiniMapMarker_0088aa30 blank;
    blank.m_a = 0;
    blank.m_b = 0;
    blank.m_c = 0;
    resize(count, blank);
    FixUpMarkers_006b67a0(0, 0, &count);
}
