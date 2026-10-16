// CThingManager::ActivateAfterLoading(long level) -- for every thing on the all-things list: ticks the loading progress display, and takes living, activatable things of the given level out of limbo and tells the quest manager
// Leaf: ActivateAfterLoading
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E5A0, length 131 bytes (`ret 4`, + int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("Activate After Loading" @ 0x01244E34), DisplayProgress(const CCharString&,
// float, bool, bool) @ 0x009E9F40 (__fastcall: text in ecx, the third bool in dl, the float and fourth bool on the stack),
// CCharString::~CCharString @ 0x0099EAE0, CThing::SetInLimbo(bool) @ 0x004C8CF0, CQuestManager::OnActivateEntity(CThing*) @ 0x004AFA60.
// Globals: the quest manager pointer @ 0x013B89FC.
// Layout (retail, local model): CThingManager all-things list (STLport list<CThing*>, node data at +8) at +0x18; CThing level
// number (short) +0x8e, dead flag +0x91 bit 0, flag byte +0x93 (bits 2|3 block activation).
// Shape: the progress text is a named CCharString local in its own block (built before the other argument pushes, in the
// dead level argument slot, destroyed right after the call); the thing is a named local tested for null, the dead bit, the flags and the level.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
class CThing {
public:
    unsigned char _pad00[0x8e];
    short         m_level;                      // +0x8e
    unsigned char _pad90;
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91_1 : 7;
    unsigned char _pad92;
    unsigned char m_flags93;                    // +0x93
    void SetInLimbo(bool inLimbo);              // 0x004C8CF0
};
struct CQuestManager {
    void OnActivateEntity(CThing* thing);       // 0x004AFA60
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC
struct SThingListNode {                         // STLport list<CThing*> node
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;                    // +0x08
};
struct CThingManager {
    unsigned char   _pad00[0x18];
    SThingListNode* Things;                     // +0x18
    void ActivateAfterLoading(long level);
};

void CThingManager::ActivateAfterLoading(long level)
{
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        {
            CCharString text("Activate After Loading");
            DisplayProgress(text, -1.0f, false, false);
        }
        CThing* thing = node->_M_data;
        if (thing && !thing->m_dead && !(thing->m_flags93 & 0xc) && thing->m_level == level) {
            thing->SetInLimbo(false);
            g_pQuestManager->OnActivateEntity(thing);
        }
    }
}
