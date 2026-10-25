// Behaviour test for 0x00C35990 AudioObject_SetSource: a source with flag bits 2 and 3 set and a pending word is
// refused (error -6, returns 0); otherwise (or for a null source) the object's setter result is returned as 0/1.
#include <cstdio>

static long g_error = 0;
struct CAudioSystem { void SetError(long code) { g_error = code; } };
static CAudioSystem g_audioSystem;
struct CAudioSource {
    long m_pending; unsigned int m_flags;
    bool IsStreamed() const { return (m_flags >> 2) & 1; }
    bool IsLocked() const { return (m_flags >> 3) & 1; }
};
struct CAudioObject {
    CAudioSource* m_source; bool m_accept;
    bool SetSource(CAudioSource* s) { m_source = s; return m_accept; }
};
static int AudioObject_SetSource(CAudioObject* object, CAudioSource* source)
{
    CAudioSource* checked;
    if (source == 0) {
        checked = 0;
    } else {
        checked = source;
        if (checked->IsStreamed() && checked->IsLocked() && checked->m_pending) {
            g_audioSystem.SetError(-6);
            return 0;
        }
    }
    return object->SetSource(checked) ? 1 : 0;
}

int main()
{
    CAudioObject o; o.m_source = 0; o.m_accept = true;
    CAudioSource s; s.m_pending = 1; s.m_flags = 0x0C;
    int ok = 1;
    if (AudioObject_SetSource(&o, &s) != 0 || g_error != -6 || o.m_source != 0) ok = 0;      // refused
    g_error = 0; s.m_pending = 0;
    if (AudioObject_SetSource(&o, &s) != 1 || g_error != 0 || o.m_source != &s) ok = 0;      // no pending word
    s.m_pending = 1; s.m_flags = 0x04;
    if (AudioObject_SetSource(&o, &s) != 1 || o.m_source != &s) ok = 0;                      // bit 3 clear
    o.m_accept = false;
    if (AudioObject_SetSource(&o, 0) != 0 || o.m_source != 0 || g_error != 0) ok = 0;        // null source, setter says no
    std::printf(ok ? "00c35990_TEST PASS\n" : "00c35990_TEST FAIL\n");
    return ok ? 0 : 1;
}