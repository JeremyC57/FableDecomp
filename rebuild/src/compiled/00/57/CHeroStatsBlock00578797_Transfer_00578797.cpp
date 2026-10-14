#pragma optimize("s", on)
// CHeroStatsBlock_00578797::Transfer(CPersistContext& context, long) -- persist the "OpenedExperienceSpending" flag at +0x1a0 and the "TutorialsPlayed" byte array at +0x248
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00578797, length 49 bytes.
// Callees: CPersistContext::Transfer<bool> @ 0x004107C0 (the manifest label "CEngineSceneGrid::GetCellBoundingBox" is propagated: the callee builds a one-byte
// temporary), CPersistContext::Transfer<signed char> @ 0x0049B8D0. Strings @ 0x0124F298 / 0x0124F288. Shape: the context is re-read from the stack for each
// call (ecx is clobbered by the first), the first member address is a `lea`, the second an `add esi,248h` (esi is dead afterwards); the second argument is unused.
// The chunk label "CEngineLocalDetailLayerDef::Transfer" is propagated from an unrelated body; the class is named by address.
struct CPersistContext {
    void TransferBool(const char* name, bool* value);                 // 0x004107C0 (Transfer<bool>)
    void TransferSignedChar(const char* name, signed char* value);    // 0x0049B8D0 (Transfer<signed char>)
};

struct CHeroStatsBlock_00578797 {
    unsigned char _pad0[0x1a0];
    bool          m_openedExperienceSpending;      // +0x1a0
    unsigned char _pad1[0xa7];
    signed char   m_tutorialsPlayed;               // +0x248
    void Transfer(CPersistContext& context, long version);
};

void CHeroStatsBlock_00578797::Transfer(CPersistContext& context, long version)
{
    context.TransferBool("OpenedExperienceSpending", &m_openedExperienceSpending);
    context.TransferSignedChar("TutorialsPlayed", &m_tutorialsPlayed);
}