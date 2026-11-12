// Behaviour test for 00a69ad0: src/00a69ad0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CFrameDataHandle::Clear()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CFrameDataHandle::Clear() -- true when empty; otherwise, when the frame data is shared by exactly two holders (count == 2), close it (0x00A69A80) and report true; then release the handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69AD0, length 73 bytes (+ int3 padding). Callers: CFrameData::StopLoading/BeginLoading
// (0x009FDC20, 0x009FDE80), ~CFrameData (0x00B2B910) and others (11 sites). The catalog label "CViewport::ClearHandle" is propagated.
// Leaf: Clear
// Callees: the frame data's Close @ 0x00A69A80 (clears its load state and frees its buffers), the counted object's virtual slot 1 (destroy).
// Shape: early `mov al,1 ; ret` for the empty handle, the result flag in bl, the CIVCountedPointer release with its double clear.
// Shape: the counted pointer is a CIVCountedPointer MEMBER at +4 whose inline GetRefCount()/Reset() run on the sub-object
// (`this+4`): VC7.1 then re-reads the pointer for the count test (`mov eax,[esi+4]`) instead of reusing the first load
// kept in ecx for Close; the same accessors on a flat handle member let it CSE the cached register.
struct CFrameData {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long m_count;                               // +0x04
    void Close();                               // 0x00A69A80
};
struct CIVCountedPointer_CFrameData {             // CIVCountedPointer<CFrameData> (PDB: one pointer, `Object`)
    CFrameData* Object;
    void Reset()
    {
        CFrameData* cur = Object;
        if (cur) {
            if (--cur->m_count == 0)
                cur->Destroy();
            Object = 0;
        }
        Object = 0;
    }
    long GetRefCount() const { return Object ? Object->m_count : 0; }
};
struct CFrameDataHandle {
    void*     m_owner;                          // +0x00
    CIVCountedPointer_CFrameData Ptr;           // +0x04
    bool Clear();
};
bool CFrameDataHandle::Clear()
{
    CFrameData* object = Ptr.Object;
    if (!object)
        return true;
    bool closed = false;
    if (Ptr.GetRefCount() == 2) {
        object->Close();
        closed = true;
    }
    Ptr.Reset();
    return closed;
}

// ---- stubs and driver ----
void CFrameData::v00() {}
static char g_log[32]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 31) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
static int g_destroyed = 0, g_closed = 0; static CFrameData* g_closeThis;
void CFrameData::Destroy() { ++g_destroyed; Log('X'); }
void CFrameData::Close() { ++g_closed; g_closeThis = this; Log('C'); }
int main()
{
    CFrameData data; CFrameDataHandle h; h.m_owner = 0;
    h.Ptr.Object = 0;
    CHECK(h.Clear() == true); CHECK(g_logn == 0);
    data.m_count = 2; h.Ptr.Object = &data;                 // shared by two: closed, count drops to 1
    CHECK(h.Clear() == true); CHECK(g_closed == 1); CHECK(g_closeThis == &data); CHECK(data.m_count == 1); CHECK(h.Ptr.Object == 0);
    CHECK(strcmp(g_log, "C") == 0);
    data.m_count = 3; h.Ptr.Object = &data;                 // three holders: not closed
    CHECK(h.Clear() == false); CHECK(g_closed == 1); CHECK(data.m_count == 2); CHECK(h.Ptr.Object == 0);
    data.m_count = 1; h.Ptr.Object = &data;                 // last holder: released and destroyed, not closed
    CHECK(h.Clear() == false); CHECK(g_destroyed == 1); CHECK(data.m_count == 0); CHECK(strcmp(g_log, "CX") == 0);
    if (fails == 0) printf("00a69ad0_TEST PASS\n"); else printf("00a69ad0_TEST FAIL\n");
    return fails;
}