// _unattributed::ActivateCurrentTarget(CTargetSet* set, long value) -- report error -7 when the set has neither an override target nor a usable current entry; otherwise touch the current target and hand the value to the set
// Grade: DIFFER(106v110) -- only the duplicated presence test differs: retail re-tests the entry word (`test eax,eax ; je error ; test eax,eax ; je ok ; ...`) where this build folds the second test away (tried: `HasEntries() && (!HasEntries() || valid)`, an if-chain helper with `count ? valid != 0 : true` (adds setne)). Next: find the original accessor pair (probably two different types over the same word).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35E30, 108 bytes (+4 int3), __stdcall (two arguments, `this` unused), ret 8.
// Leaf: ActivateCurrentTarget
// The catalog label (CBankFile::OnInventoryChanged) is propagated and does not fit the body: it is one of a run of
// free `__stdcall` helpers called back to back on the same object by 0x00C274C8 (render-side code near
// CRenderTarget / CDisplayViewManager); the class could not be established, so the model is named by role.
// Callees: the error sink 0x00C3D090 on the global object @ 0x0143BB58 (argument -7), the target's member
// 0x00C3C4A0 (result unused) and the set's member 0x00C438F0 (takes the value).
// Layout (by use): an entry array at +0x54, an entry-count/presence word at +0x5C, the current index at
// +0x64, a validity word at +0x6C and an override target at +0x78.
// Shape: two inline accessors -- IsUsable() (override, else entries present and, re-testing the presence
// word, the validity word set: VC keeps the duplicated `test eax,eax`) and GetCurrentTarget() (override,
// else entries[current], else null); the override test is threaded straight into the call.
class CTarget {
public:
    void* Touch();                         // 0x00C3C4A0
};

class CErrorSink {
public:
    void Report(long code);                // 0x00C3D090
};
extern CErrorSink g_targetErrorSink;       // 0x0143BB58

class CTargetSet {
public:
    unsigned char _pad_0x00[0x54];
    CTarget** m_ppEntries;                 // +0x54
    unsigned long m_58;                    // +0x58
    unsigned long m_entryCount;            // +0x5C
    unsigned long m_60;                    // +0x60
    unsigned long m_currentIndex;          // +0x64
    unsigned long m_68;                    // +0x68
    unsigned long m_currentValid;          // +0x6C
    unsigned long m_70;                    // +0x70
    unsigned long m_74;                    // +0x74
    CTarget* m_pOverride;                  // +0x78
    void SetValue(long value);             // 0x00C438F0
    bool HasEntries() const { return m_entryCount != 0; }
    bool IsCurrentValid() const { return !HasEntries() || m_currentValid != 0; }
    bool IsUsable() const
    {
        if (m_pOverride)
            return true;
        return HasEntries() && IsCurrentValid();
    }
    CTarget* GetCurrentTarget() const
    {
        if (m_pOverride)
            return m_pOverride;
        if (HasEntries())
            return m_ppEntries[m_currentIndex];
        return 0;
    }
};

void __stdcall ActivateCurrentTarget(CTargetSet* set, long value)
{
    if (!set->IsUsable()) {
        g_targetErrorSink.Report(-7);
        return;
    }
    set->GetCurrentTarget()->Touch();
    set->SetValue(value);
}
