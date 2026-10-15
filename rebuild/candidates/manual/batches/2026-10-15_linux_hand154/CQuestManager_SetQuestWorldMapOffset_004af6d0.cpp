// CQuestManager::SetQuestWorldMapOffset(const CCharString& questName, const C2DCoordI& offset) -- set the world-map marker position of the active quest with this script name (nothing when it is not active)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF6D0, length 98 bytes (padded to 112), `ret 8`.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`) inside the inlined CCharString equality.
// Layout: retail CQuestManager ActiveQuests (STLport list of CCountedPointer<CActiveQuest>) at +0x38, CActiveQuest
// MarkerPosition +0x1c and QuestName +0x30 (the landed GetActiveQuestFromScriptName 0x004AF5B0 model).
// Shape: the inlined GetActiveQuestFromScriptName walk (head in ebp, the name's block in esi read once after the empty
// test) written as a __forceinline bool finder with an out-pointer so esi/ebx are pushed only on the non-empty path and
// the hit goes straight to the two-dword marker copy.
struct CCharStringData {                           // the shared string block
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
// The CCharString equality on the two string blocks: the same block, or both present with equal lengths and
// Compare == 0 (`neg eax ; sbb al,al ; inc al`).
inline bool SameString(CCharStringData* a, CCharStringData* b)
{
    if (a == b)
        return true;
    if (a && b && a->m_length == b->m_length)
        return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}
struct CCharString {
    CCharStringData* PStringData;
};
struct C2DCoordI {
    long X, Y;
};
struct CActiveQuest {                              // CQuestManager::CActiveQuest, retail layout (landed 0x004AF5B0 model)
    long          ID;                              // +0x00
    void*         PScriptInfo;                     // +0x04 CScriptInfo*
    void*         PScript[2];                      // +0x08 CCountedPointer<CScriptBase>
    void*         Regions[3];                      // +0x10 vector<const CRegion*> (retail: 12 bytes)
    C2DCoordI     MarkerPosition;                  // +0x1c
    bool          Persistent;                      // +0x24
    bool          LoadedResources;                 // +0x25
    void*         PScriptData[2];                  // +0x28 CCountedPointer<CScriptDataBase>
    CCharString   QuestName;                       // +0x30
};
struct CCPPointerInfo;
struct SActiveQuestNode {                          // STLport list node of CCountedPointer<CActiveQuest>
    SActiveQuestNode* next;                        // +0x00
    SActiveQuestNode* prev;                        // +0x04
    CActiveQuest*     m_pData;                     // +0x08
    CCPPointerInfo*   m_pInfo;                     // +0x0c
};
struct CQuestManagerRetail {                       // retail view of CQuestManager
    unsigned char     _pad00[0x38];                // +0x00 vptr, PGameScriptThingManager, RuntimeQuestPersistence, RegisteredQuestNames
    SActiveQuestNode* ActiveQuests;                // +0x38 STLport list: the sentinel node pointer
    __forceinline bool FindActiveQuest(const CCharString& name, CActiveQuest*& found) const
    {
        SActiveQuestNode* head = ActiveQuests;
        SActiveQuestNode* node = head->next;
        if (node != head) {
            CCharStringData* key = name.PStringData;
            do {
                CActiveQuest* quest = node->m_pData;
                if (SameString(quest->QuestName.PStringData, key)) {
                    found = quest;
                    return true;
                }
                node = node->next;
            } while (node != head);
        }
        return false;
    }
    void SetQuestWorldMapOffset(const CCharString& questName, const C2DCoordI& offset);
};
void CQuestManagerRetail::SetQuestWorldMapOffset(const CCharString& questName, const C2DCoordI& offset)
{
    CActiveQuest* quest;
    if (FindActiveQuest(questName, quest)) {
        quest->MarkerPosition.X = offset.X;
        quest->MarkerPosition.Y = offset.Y;
    }
}
