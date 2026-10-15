// CQuestManager::IsCreatureGenerationEnabled() -- false while any quest named in CreatureGenerationDisabledQuests is active, true otherwise
// Grade: DIFFER(ours v retail) -- 65v63: entry block only: retail `mov eax,[esi+44h] ; mov ebx,[esi+40h] ; sub eax,ebx`, the lane loads the running name pointer first and copies it (`mov ebx,[esi+40h] ; mov eax,[esi+44h] ; mov ecx,ebx ; sub eax,ecx`); the loop body and both exits are identical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFC60, length 63 bytes.
// Leaf: IsCreatureGenerationEnabled
// Callees: CQuestManager::IsQuestActive(const CCharString&) @ 0x004AF610 (walks the ActiveQuests list at +0x38
// comparing each quest's name; manifest label "CQuest_Array_FindByName").
// Identity: the manifest label "COptimisedPrimitive::DetectFoldOver" is propagated and rejected: both callers
// (0x0049EA53, 0x00892D89) load `this` from the quest-manager global 0x013B89FC and the body sits among the
// CQuestManager methods (0x004AF670..0x004AFCA0). Ego_r.pdb CQuestManager: ActiveQuests (list) then
// PendingQuestsToDeactivate (list) then the vector<CCharString> CreatureGenerationDisabledQuests; with the
// retail 4-byte STLport lists the vector lands at +0x40 (the list head pointer at +0x38 is what 0x004AF610 walks).
// The element pointer is a running local (retail strength-reduces it into ebx, `add ebx,4`, while the
// count is re-derived from the vector after every call).
// Tried: indexed `[i]` (re-derives the element address every iteration, 61 bytes), the running pointer
// incremented in the for-header (this file) or in the body (same bytes), "s" size optimisation (58/59).
struct CCharString {
    char* m_pString;
};

struct CCharStringVector {                  // retail std::vector<CCharString> (12 bytes)
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    CCharString& operator[](unsigned long index) { return m_pFirst[index]; }
};

struct CQuestManager {
    unsigned char     _pad[0x40];
    CCharStringVector CreatureGenerationDisabledQuests;     // +0x40
    bool IsQuestActive(const CCharString& name);            // 0x004AF610
    bool IsCreatureGenerationEnabled();
};

bool CQuestManager::IsCreatureGenerationEnabled()
{
    CCharString* questName = CreatureGenerationDisabledQuests.m_pFirst;
    for (unsigned long i = 0; i < CreatureGenerationDisabledQuests.size(); ++i, ++questName) {
        if (IsQuestActive(*questName))
            return false;
    }
    return true;
}
