// CQuestManager::GetActiveQuestFromScriptName(const CCharString& name) const -- the active quest whose QuestName equals the name, or 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF5B0, length 87 bytes.
// Leaf: ?GetActiveQuestFromScriptName@CQuestManager
// Retail bytes: push ebx ; push ebp ; mov ebp,[ecx+38h] ; push esi ; push edi ; mov edi,[ebp] ; cmp edi,ebp ; je N ; mov eax,[esp+14h] ; mov esi,[eax] ;
//               L: mov ebx,[edi+8] ; mov eax,[ebx+30h] ; cmp eax,esi ; je F ; (null/length tests, Compare, neg/sbb/inc) ; jne F ; mov edi,[edi] ; cmp edi,ebp ; jne L ;
//               N: ... xor eax,eax ; ret 4 ; F: ... mov eax,ebx ; ret 4
// Callee: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`) inside the inlined CCharString equality.
// The manifest label "CScriptBase::OnRegionLoaded" is propagated (that one is 0x00CB7B00): the callers are CQuestManager
// code (ProcessQuestRegions 0x004B01B0, 0x004B11A0, SetQuestAsCompleted 0x004B1D30), `this` + 0x38 is ActiveQuests (retail
// CQuestManager: RegisteredQuestNames is the 12-byte vector at +0x2c, as in the landed GetRegisteredQuestFromIndex, so the
// 4-byte STLport list follows at +0x38), each node holds a CCountedPointer<CActiveQuest> (object at node + 8), and
// CActiveQuest::QuestName sits at +0x30 (PDB +0x34 less the 4 bytes of the retail Regions vector). The PDB lists
// ?GetActiveQuestFromScriptName@CQuestManager@@QBEPAVCActiveQuest@1@ABVCCharString@@@Z as the only const CQuestManager
// member returning a CActiveQuest* from a name. Shape: retail keeps the list head in ebp and the argument's string block in
// esi for the whole walk although the loop calls Compare (the retail build knows that callee writes no memory; the lane
// re-reads both after the call), so both are explicit locals here, the block read once after the empty test; the found
// quest is returned from ebx.
#include "engine/CQuestManager.h"             // PDB layout: ActiveQuests at +0x44 (debug containers); retail +0x38, see above

// NF52 (round 7): the name test is fable_string.h's inline CCharString::operator==(const CCharString&) (data pointers, null tests,
// then CBasicString<char>::operator==: StringLength and !Compare), which binds the protected static
// ?Compare@?$CBasicString@D@@KIJPBD0@Z (0x00411570). Were local CCharStringData / CCharString models, the SameString helper and the
// made-up Compare_00411570. The CQuestManagerRetail / CActiveQuest / list-node stand-ins are pre-existing (the self leaf is still
// CQuestManagerRetail; NAME_FIXES.tsv). The Compare template body is visible (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY, as row 0x009C9280):
// VC7.1 still calls it out of line but knows it writes no memory, so the name's data pointer stays in esi across the loop (retail
// 0x004AF5BE..0x004AF5C2 loads it once).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "fable_string.h"

struct CActiveQuest {                          // CQuestManager::CActiveQuest, retail layout
    long          ID;                          // +0x00
    void*         PScriptInfo;                 // +0x04 CScriptInfo*
    void*         PScript[2];                  // +0x08 CCountedPointer<CScriptBase>
    void*         Regions[3];                  // +0x10 vector<const CRegion*> (retail: 12 bytes)
    long          MarkerPosition[2];           // +0x1c C2DCoordI
    bool          Persistent;                  // +0x24
    bool          LoadedResources;             // +0x25
    void*         PScriptData[2];              // +0x28 CCountedPointer<CScriptDataBase>
    CCharString   QuestName;                   // +0x30
};

class CCPPointerInfo;
struct SActiveQuestNode {                      // STLport list node of CCountedPointer<CActiveQuest>
    SActiveQuestNode* next;                    // +0x00
    SActiveQuestNode* prev;                    // +0x04
    CActiveQuest*     m_pData;                 // +0x08
    CCPPointerInfo*   m_pInfo;                 // +0x0c
};

struct CQuestManagerRetail {                   // retail view of CQuestManager
    unsigned char     _pad00[0x38];            // +0x00 vptr, PGameScriptThingManager, RuntimeQuestPersistence, RegisteredQuestNames
    SActiveQuestNode* ActiveQuests;            // +0x38 STLport list: the sentinel node pointer
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& name) const;
};

CActiveQuest* CQuestManagerRetail::GetActiveQuestFromScriptName(const CCharString& name) const
{
    SActiveQuestNode* head = ActiveQuests;
    SActiveQuestNode* node = head->next;
    if (node != head) {
        do {
            CActiveQuest* quest = node->m_pData;
            if (quest->QuestName == name)
                return quest;
            node = node->next;
        } while (node != head);
    }
    return 0;
}