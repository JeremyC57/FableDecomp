// _global::AudioSample_SetPlaying(CAudioSample* sample, int play) -- libpk audio API: refuses (error -7, returns 0) a sample that is neither streamed (+0x78) nor holding complete data (+0x5C data, +0x6C loaded); otherwise switches the sample's playing state, reporting a non-zero result code as the error and returning 0, else returns 1
// Grade: DIFFER(85v89) -- retail keeps a dead second `test eax,eax ; je <success>` on the +0x5C data word right after the `je <error>` on it; every form tried (|| expression, early-return inline predicates, nested inlines, one && / || expression) lets VC7.1 fold the re-test away; everything else matches
// Leaf: AudioSample_SetPlaying
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35DD0, 89 bytes, __stdcall `ret 8`. Descriptive name (no PDB covers libpk):
//   the catalog's `CDrawAutoPickups_ShouldRender` is propagated. Callers: 0x00C274E8, 0x00C2771D, 0x00C279C8,
//   0x00C495C2, 0x00C49A41.
// Callees: the global audio system (0x0143BB58)'s SetError @ 0x00C3D090 and the sample's state setter @ 0x00C43FD0
//   (__thiscall, one bool, returns an error code; ecx still holds the sample).
// Shape: retail tests the data word twice in a row (`je` to the error, then a dead `je` to the success arm); this
//   two-inline-predicate model is the closest form (the re-test is folded).
struct CAudioSystem {
    void SetError(long code);                // 0x00C3D090
};
extern CAudioSystem g_audioSystem;           // 0x0143BB58

struct CAudioSample {
    unsigned char _pad00[0x5c];
    void*         m_pData;                   // +0x5C
    unsigned char _pad60[0x6c - 0x60];
    long          m_loaded;                  // +0x6C
    unsigned char _pad70[0x78 - 0x70];
    long          m_streamed;                // +0x78
    bool IsComplete() const { return m_pData == 0 || m_loaded != 0; }
    bool IsReady() const { return m_streamed != 0 || (m_pData != 0 && IsComplete()); }
    long SetPlaying(bool play);              // 0x00C43FD0
};

int __stdcall AudioSample_SetPlaying(CAudioSample* sample, int play)
{
    if (!sample->IsReady()) {
        g_audioSystem.SetError(-7);
        return 0;
    }
    long error = sample->SetPlaying(play != 0);
    if (error) {
        g_audioSystem.SetError(error);
        return 0;
    }
    return 1;
}
