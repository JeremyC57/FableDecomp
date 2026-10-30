// Behaviour test for CEnumPins::Next (0x00CAAFC0): E_POINTER / E_INVALIDARG checks, the out-of-sync refresh, the
// min(available, requested) count with S_FALSE at 0, cache skipping, AddRef of every returned pin, and the
// S_OK / S_FALSE result, through a copy of the strmbase logic over a mock filter.
#include <cstdio>
typedef long HRESULT;
typedef unsigned long ULONG;
struct Pin {
    int refs;
    Pin() : refs(0) {}
    ULONG AddRef() { return ++refs; }
};
struct Filter {
    int version;
    int count;
    Pin* pins[8];
    virtual ~Filter() {}
    virtual long GetPinVersion() { return version; }
    virtual int GetPinCount() { return count; }
    virtual Pin* GetPin(int n) { return (n < count) ? pins[n] : 0; }
};
struct Cache {
    Pin* items[16];
    int n;
    Cache() : n(0) {}
    int Find(Pin* p) const { for (int i = 0; i < n; i++) if (items[i] == p) return i + 1; return 0; }
    void AddTail(Pin* p) { items[n++] = p; }
};
struct EnumPins {
    int m_Position;
    int m_PinCount;
    Filter* m_pFilter;
    long m_Version;
    Cache m_PinCache;
    int AreWeOutOfSync() { return (m_pFilter->GetPinVersion() == m_Version ? 0 : 1); }
    HRESULT Refresh() { m_Version = m_pFilter->GetPinVersion(); m_PinCount = m_pFilter->GetPinCount(); m_Position = 0; return 0; }
    HRESULT Next(ULONG cPins, Pin** ppPins, ULONG* pcFetched)
    {
        if (ppPins == 0) return (HRESULT)0x80004003L;
        if (pcFetched != 0) *pcFetched = 0;
        else if (cPins > 1) return (HRESULT)0x80070057L;
        ULONG cFetched = 0;
        if (AreWeOutOfSync() == 1) Refresh();
        int cRealPins = ((m_PinCount - m_Position) < (int)cPins) ? (m_PinCount - m_Position) : (int)cPins;
        if (cRealPins == 0) return 1;
        while (cRealPins && (m_PinCount - m_Position)) {
            Pin* pPin = m_pFilter->GetPin(m_Position++);
            if (pPin == 0) return (HRESULT)0x80040203L;
            if (0 == m_PinCache.Find(pPin)) {
                *ppPins = pPin;
                pPin->AddRef();
                cFetched++;
                ppPins++;
                m_PinCache.AddTail(pPin);
                cRealPins--;
            }
        }
        if (pcFetched != 0) *pcFetched = cFetched;
        return (cPins == cFetched ? 0 : 1);
    }
};
static int fail() { std::printf("00caafc0_TEST FAIL\n"); return 1; }
int main()
{
    Pin p[4];
    Filter f; f.version = 1; f.count = 3; f.pins[0] = &p[0]; f.pins[1] = &p[1]; f.pins[2] = &p[2]; f.pins[3] = &p[3];
    EnumPins e; e.m_Position = 0; e.m_PinCount = 3; e.m_pFilter = &f; e.m_Version = 1;
    Pin* out[8]; ULONG got = 99;
    if (e.Next(1, 0, &got) != (HRESULT)0x80004003L) return fail();
    if (e.Next(2, out, 0) != (HRESULT)0x80070057L) return fail();
    if (e.Next(2, out, &got) != 0 || got != 2 || out[0] != &p[0] || out[1] != &p[1] || p[0].refs != 1) return fail();
    if (e.Next(5, out, &got) != 1 || got != 1 || out[0] != &p[2] || e.m_Position != 3) return fail();
    if (e.Next(1, out, &got) != 1 || got != 0) return fail();          // nothing left: S_FALSE
    // a fourth pin appears: version moves, the refresh rewinds, the cache skips the three already returned
    f.count = 4; f.version = 2;
    if (e.Next(4, out, &got) != 1 || got != 1 || out[0] != &p[3] || p[3].refs != 1 || p[0].refs != 1) return fail();
    if (e.m_Version != 2 || e.m_PinCount != 4) return fail();
    // a pin vanishes behind the enumerator's back: out of sync
    EnumPins e2; e2.m_Position = 0; e2.m_PinCount = 4; e2.m_pFilter = &f; e2.m_Version = 2; f.count = 1;
    if (e2.Next(3, out, &got) != (HRESULT)0x80040203L) return fail();
    std::printf("00caafc0_TEST PASS\n");
    return 0;
}