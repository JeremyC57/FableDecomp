// CTCSearchableContainer::OnSerialise(CPersistContext*, bool) -- persists the container: the CTCContainerRewardHero base first, then NumberOfSearchesRemaining (+0x1c, saved under the key "NumberOfTimesToSearch") with a zero default
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812370, length 53 bytes; vtable CTCSearchableContainer 0x012431F4 slot 1.
// Callees: CTCContainerRewardHero::OnSerialise(CPersistContext*, bool) @ 0x0074E240 (direct base call),
// CPersistContext::Transfer<long>(const char*, long&, const long&) @ 0x00410BE0 (landed as in 0x00D21B10); the literal
// 0x01272408 is "NumberOfTimesToSearch".
// Label: the manifest name "CTCSearchableContainer::vfunc_1" is a slot placeholder; the name
// follows the base's manifest name. The same +0x1c counter is the one Search (0x00812340) counts down.
// Shape: the context argument is cached in edi; the zero default is a named local that VC homes in the dead `bool`
// argument slot (`mov dword ptr [esp+1Ch],0`, `lea ecx,[esp+10h]`); `add esi,1Ch` forms the member address.
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // Transfer<long> @ 0x00410BE0
};

struct CTCContainerRewardHero {
    virtual void v00();
    virtual void OnSerialise(CPersistContext* context, bool loading);        // 0x0074E240
    unsigned char _body[0x1c - 4];
};

struct CTCSearchableContainer : CTCContainerRewardHero {
    long NumberOfSearchesRemaining;          // +0x1c (PDB +0x20: the retail CTCContainerRewardHero is 4 bytes narrower)
    virtual void OnSerialise(CPersistContext* context, bool loading);
};

void CTCSearchableContainer::OnSerialise(CPersistContext* context, bool loading)
{
    CTCContainerRewardHero::OnSerialise(context, loading);
    long defaultValue = 0;
    context->Transfer("NumberOfTimesToSearch", NumberOfSearchesRemaining, defaultValue);
}
