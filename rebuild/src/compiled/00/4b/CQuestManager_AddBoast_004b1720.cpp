// CQuestManager::AddBoast(const CCharString& scriptName, const CCharString& textTag, long id, long cost, long reward, bool evil, long groupIndex) -- offer a boast: look up its text (tag) and title (tag + "_TITLE") in the game-text bank (falling back to the widened tag), append it to Boasts and stamp the boast frame
// Leaf: AddBoast
// Retail 0x004B1720, 576 bytes, `ret 0x1c` (Ego_r.pdb ?AddBoast@CQuestManager@@QAEXABVCCharString@@0JJJ_NJ@Z).
// Callees: CWideString() 0x0099AED0 / operator= 0x0099B7D0 / copy 0x0099B720 / dtor 0x0099B510, the CRC CCRC::Calc 0x004014A0
// (`__fastcall`), NGameText::CDataBank GetTagFromCRC 0x009CC410 / GetPGroupEntry 0x009C9070, CGroupEntry::GetText
// 0x009C7A40, CCharString::ToWideString 0x0099E480, operator+(const CCharString&, const char*) 0x0099F600 (`__fastcall`,
// hidden result in ecx), CCharString copy 0x0099EC30 / dtor 0x0099EAE0, malloc 0x00BFEA0E, CBoast(const CBoast&)
// 0x004B73A0, CWorld::GetWorldFrame 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated).
// Globals: the engine 0x013B86A0 (+0x14 the game-text bank), "" 0x0129AAF4, "_TITLE" 0x0122E240.
// Layout: retail STLport CQuestManager (Boasts list header +0x68, FrameBoastsLastUpdated +0x64; landed ctor 0x004B4590).
// Shape: the bank is read once into a local; each lookup assigns GetPGroupEntry(tag)->GetText() (the counted group
// entry released inline through its +4 count) or the widened tag (the title fallback widens the text tag, not the
// title tag); the boast is a temporary pushed with the landed STLport push_back, so it dies before the frame stamp.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
class CCharStringData { public:
    char* PData;
    unsigned long StringLength;
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetString(), GetLength()); }
    CWideString ToWideString() const;                   // 0x0099E480
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct CRefCounted {
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy() const;                             // slot 1
};
namespace NGameText {
class CTextEntry : public CRefVirtual, public CRefCounted { public:
    CWideString GetText() const;                        // 0x009C7A40
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long tag, bool b) const;   // 0x009C9070
public:
    unsigned long LookupValue(unsigned long) const;   // 0x009CC410
};
}
class CEngine { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;            // +0x14
};
extern CEngine* g_pEngine;                              // 0x013B86A0
class CWorld { public: static long __fastcall GetFrame(); };         // 0x0049D870

struct CBoast {                                         // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;                                     // +0x00
    long        Cost;                                   // +0x04
    long        Reward;                                 // +0x08
    CCharString ScriptName;                             // +0x0c
    CWideString Text;                                   // +0x10
    CWideString Title;                                  // +0x14
    bool        Evil;                                   // +0x18
    bool        Failed;                                 // +0x19
    bool        Completed;                              // +0x1a
    long        GroupIndex;                             // +0x1c
    CBoast(long id, long cost, long reward, const CCharString& scriptName, const CWideString& text,
           const CWideString& title, bool evil, long groupIndex)
        : ID(id), Cost(cost), Reward(reward), ScriptName(scriptName), Text(text), Title(title), Evil(evil),
          Failed(false), Completed(false), GroupIndex(groupIndex)
    {
    }
    CBoast(const CBoast& other);                        // 0x004B73A0
};
struct CBoastNode {
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                                  // +0x08
};
struct CBoastList {                                     // STLport list<CBoast>
    struct iterator {
        CBoastNode* _M_node;
        iterator(CBoastNode* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    CBoastNode* m_node;
    iterator end() { return iterator(m_node); }
    CBoastNode* _M_create_node(const CBoast& x)
    {
        CBoastNode* tmp = static_cast<CBoastNode*>(malloc(sizeof(CBoastNode)));
        new (&tmp->value) CBoast(x);
        return tmp;
    }
    void insert(iterator position, const CBoast& x)
    {
        CBoastNode* tmp = _M_create_node(x);
        CBoastNode* pos = position._M_node;
        CBoastNode* prev = pos->prev;
        tmp->next = pos;
        tmp->prev = prev;
        prev->next = tmp;
        pos->prev = tmp;
    }
    void push_back(const CBoast& x) { insert(end(), x); }
};

class CQuestManager { public:
    unsigned char _pad00[0x64];
    unsigned long FrameBoastsLastUpdated;               // +0x64
    CBoastList    Boasts;                               // +0x68
    void AddBoast(const CCharString& scriptName, const CCharString& textTag, long id, long cost, long reward,
                  bool evil, long groupIndex);
};

void CQuestManager::AddBoast(const CCharString& scriptName, const CCharString& textTag, long id, long cost, long reward,
                             bool evil, long groupIndex)
{
    CWideString text;
    CWideString title;
    NGameText::CDataBank* bank = g_pEngine->PGameTextDataBank;
    if (unsigned long tag = bank->LookupValue(textTag.GetCRC()))
        text = bank->GetPTextEntry(tag, true)->GetText();
    else
        text = textTag.ToWideString();
    CCharString titleTag = textTag + "_TITLE";
    if (unsigned long tag = bank->LookupValue(titleTag.GetCRC()))
        title = bank->GetPTextEntry(tag, true)->GetText();
    else
        title = textTag.ToWideString();
    Boasts.push_back(CBoast(id, cost, reward, scriptName, text, title, evil, groupIndex));
    FrameBoastsLastUpdated = CWorld::GetFrame();
}