// CEngineSubPrimitiveRenderer::AddSubPrimitive(void* a, void* b, void* c, void* d, void* e) -- forward the five arguments plus a zeroed 8-byte key local to the six-argument overload
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B846E0, length 54 bytes.
// Callees: the six-argument AddSubPrimitive @ 0x00B84580 (on `this`, ecx passed straight through). Shape: the two-word local is zeroed from eax in the 8-byte frame,
// its address pushed first, then the five arguments re-read last-first in alternating edx/eax; `add esp,8 ; ret 14h`.
struct SSortKey_00b846e0 { long m_a; long m_b; };

struct CEngineSubPrimitiveRenderer {
    void AddSubPrimitive(void* a, void* b, void* c, void* d, void* e, SSortKey_00b846e0* key);   // 0x00B84580
    void AddSubPrimitive(void* a, void* b, void* c, void* d, void* e);
};

void CEngineSubPrimitiveRenderer::AddSubPrimitive(void* a, void* b, void* c, void* d, void* e)
{
    SSortKey_00b846e0 key;
    key.m_a = 0;
    key.m_b = 0;
    AddSubPrimitive(a, b, c, d, e, &key);
}