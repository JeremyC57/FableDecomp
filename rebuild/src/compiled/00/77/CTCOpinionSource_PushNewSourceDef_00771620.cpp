// CTCOpinionSource::PushNewSourceDef(const CCharString&) -- looks the def up by name and pushes its index; 1 when pushed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771620, length 54 bytes.
// Callees: CTCBase::GetWorld() const @ 0x00686A80 (label "CViewBase::GetDisplayEngine" is propagated),
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410 on the world's DefinitionManager (+0x10),
// CTCOpinionSource::PushSourceDefIndex(long) @ 0x007714B0 (bool; label "CTCCreatureGroupTag::DeactivateMember" is propagated).
// Shape: the DefinitionManager pointer is a named local (`mov eax,[eax+10h] ; mov ecx,eax`; reached inline as the
// call's object it loads straight into ecx); a positive index is pushed,
// `if (a > 0 && Push(a)) return 1; return 0;` gives the `mov eax,1` / shared `xor eax,eax` tails.
struct CCharString;

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CWorld {
    unsigned char       _pad00[0x10];
    CDefinitionManager* DefinitionManager;          // +0x10
};

struct CTCBase_00771620 {
    virtual void v00();
    unsigned char _pad04[8];
    const CWorld* GetWorld() const;                 // 0x00686A80
};

struct CTCOpinionSource : CTCBase_00771620 {
    bool PushSourceDefIndex(long index);            // 0x007714B0
    long PushNewSourceDef(const CCharString& name);
};

long CTCOpinionSource::PushNewSourceDef(const CCharString& name)
{
    CDefinitionManager* defs = GetWorld()->DefinitionManager;
    long index = defs->GetDefGlobalIndexFromName(name);
    if (index > 0 && PushSourceDefIndex(index))
        return 1;
    return 0;
}