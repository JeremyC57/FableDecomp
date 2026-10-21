// CQuestManager::AddNewObjective(const CCharString& textTag, unsigned long id) -- append an objective whose text is the tag's game text (the tag itself, widened, when the bank has no text for it)
// Leaf: AddNewObjective
// Retail 0x004B1BA0, 387 bytes, `ret 8`. Callees: CCharString::ToWideString 0x0099E480, the CRC Calc 0x004014A0
// (`__fastcall`), NGameText::CDataBank GetTagFromCRC 0x009CC410 / GetTextInfo 0x009C7D00 / GetPGroupEntry 0x009C9070,
// CGroupEntry::GetText 0x009C7A40, CWideString copy 0x0099B720 / operator= 0x0099B7D0 / dtor 0x0099B510, malloc 0x00BFEA0E.
// Globals: the engine 0x013B86A0 (+0x14 the game-text bank), "" 0x0129AAF4.
// Layout: engine/CQuestManager.h is the PDB layout (Objectives +0xC8); retail sits 0x54 lower here (Objectives, the
// STLport list header pointer, at +0x74; see the landed UninitializeDaySection 0x004B3260), hence a local model.
// Shape: the bank comes from an inline getter (a function call, so the bank load lands between the CRC's arguments and
// the Calc call; the plain `g_pEngine->PGameTextDataBank->` member expression loads it after Calc); the `GetTextInfo &&
// GetPGroupEntry(...).Data` test keeps its conditional counted-pointer temporary (the `test bl,1` destroy flag);
// list push_back is STLport insert(end()) with the `prev` local (one load of header->prev).
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
struct CCharStringData {
    char* PData;
    unsigned long StringLength;
};
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
namespace NGameText {
struct CDataBankTextEntry;
struct CGroupEntry : CRefVirtual, CRefCounted {
    CWideString GetText() const;                        // 0x009C7A40
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    unsigned long GetTagFromCRC(unsigned long crc) const;                             // 0x009CC410
    CDataBankTextEntry* GetTextInfo(unsigned long tag) const;                         // 0x009C7D00
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool b) const;   // 0x009C9070
};
}
struct CEngine {
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;            // +0x14
};
extern CEngine* g_pEngine;                              // 0x013B86A0
inline NGameText::CDataBank* GetGameTextBank() { return g_pEngine->PGameTextDataBank; }

struct CQuestManager {
    struct CObjective {                                 // 0x0c bytes
        CWideString   Text;
        unsigned long ID;
        long          State;
        CObjective(const CWideString& text, unsigned long id) : Text(text), ID(id), State(0) { }
    };
    struct CObjectiveNode {                             // STLport _List_node<CObjective>
        CObjectiveNode* _M_next;
        CObjectiveNode* _M_prev;
        CObjective      _M_data;
    };
    struct CObjectiveList {                             // STLport list<CObjective>: the header node pointer
        CObjectiveNode* _M_node;
        CObjectiveNode* _M_create_node(const CObjective& x)
        {
            CObjectiveNode* tmp = (CObjectiveNode*)malloc(sizeof(CObjectiveNode));
            new (&tmp->_M_data) CObjective(x);
            return tmp;
        }
        CObjectiveNode* insert(CObjectiveNode* position, const CObjective& x)
        {
            CObjectiveNode* tmp = _M_create_node(x);
            CObjectiveNode* n = position;
            CObjectiveNode* prev = n->_M_prev;
            tmp->_M_next = n;
            tmp->_M_prev = prev;
            prev->_M_next = tmp;
            n->_M_prev = tmp;
            return tmp;
        }
        void push_back(const CObjective& x) { insert(_M_node, x); }
    };
    unsigned char  _pad00[0x74];
    CObjectiveList Objectives;                          // +0x74
    void AddNewObjective(const CCharString& textTag, unsigned long id);
};

void CQuestManager::AddNewObjective(const CCharString& textTag, unsigned long id)
{
    CWideString text = textTag.ToWideString();
    unsigned long tag = GetGameTextBank()->GetTagFromCRC(textTag.GetCRC());
    if (tag) {
        NGameText::CDataBank* bank = GetGameTextBank();
        bool hasText = bank->GetTextInfo(tag) && bank->GetPGroupEntry(tag, true).Data != 0;
        if (hasText)
            text = bank->GetPGroupEntry(tag, true)->GetText();
    }
    CObjective objective(text, id);
    Objectives.push_back(objective);
}
