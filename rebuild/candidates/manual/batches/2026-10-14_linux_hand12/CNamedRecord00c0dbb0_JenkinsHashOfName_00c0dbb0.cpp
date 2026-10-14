// _unattributed::JenkinsHashOfName() -- the Jenkins hash (seed 0) of the C string at +0x00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DBB0, length 24 bytes.
// Callees: SafeStringLength(const char*) @ 0x00C0BCC0 (a __stdcall one-argument wrapper: 0 for null, else the
// imported strlen through 0x0143FD88), JenkinsHashBuffer(const char*, unsigned long, unsigned long) @ 0x00C1D500
// (__fastcall: data in ecx, length in edx, seed on the stack -- the `push 0` before the length call is its
// third argument, consumed by the hash's own `ret`).
// Shape: arguments evaluated right to left: the seed is pushed, the length computed by the call, the string
// re-read from `this` (esi) for ecx. The manifest's "CAnimationSet::BeginLoading" label is propagated.
unsigned long __stdcall SafeStringLength(const char* text);                                    // 0x00C0BCC0
unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed);   // 0x00C1D500

struct CNamedRecord_00c0dbb0 {
    const char* m_pName;                        // +0x00
    unsigned long JenkinsHashOfName() const;
};

unsigned long CNamedRecord_00c0dbb0::JenkinsHashOfName() const
{
    return JenkinsHashBuffer(m_pName, SafeStringLength(m_pName), 0);
}
