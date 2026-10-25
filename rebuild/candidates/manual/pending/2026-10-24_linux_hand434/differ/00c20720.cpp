// CSoundEvent::CheckState(long expected) -- release-build state check every sound-event method calls first: a switch over the 0..6 state word whose cases all compiled to nothing
// Grade: DIFFER(3v43) -- lane gap: VC7.1 folds an all-empty switch to `ret 4`; retail keeps a 7-entry byte-index table whose every entry (and the default) is the same `ret 4`
// Leaf: ?CheckState@CSoundEvent@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20720, 43 bytes (`ret 4`; the byte-index table 0x00C20744 holds seven zeroes,
//   the dword table 0x00C20740 one entry, 0x00C2073A). Descriptive name; the manifest's "EmptyStub" is propagated.
//   Unique in the exe (no other switch with an all-zero byte table). Callers: every CSoundEvent (libpk sound event) method of this chunk
//   (0x00C20750 .. 0x00C20EF0) with the state it expects, -1 for any. The libpk audio library this class belongs to
//   was LTCG-built (chunk 433: static helpers take esi/edi/ebx unsaved), so the case bodies were probably calls to an
//   empty cross-module trace/assert function that link-time code generation removed only after the switch was lowered.
// Callees: none.
// Tried (probes, all fold to `ret 4` or `movzx ; mov [esp],eax ; ret 4`): `break`-only cases, __noop / __assume
//   bodies, dead stores (local, address-taken local, parameter), `if (cond) {}`, `(void)(cond)`, empty inline
//   Check(bool), empty loops, an inner switch, an empty out-of-line Trace defined later in the TU, a value switch
//   feeding an empty loop, `#pragma optimize("g", off)` (ebp frame; breaks every caller's register use).
struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    unsigned char _pad00[0xac];
    unsigned short m_state;                 // +0xAC (0..6)
    void CheckState(long expected);
};

void CSoundEvent::CheckState(long expected)
{
    switch (m_state) {
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 5: break;
    case 6: break;
    }
}
