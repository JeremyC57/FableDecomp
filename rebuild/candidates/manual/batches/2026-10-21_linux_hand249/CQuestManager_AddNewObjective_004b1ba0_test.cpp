// test for 0x004B1BA0
#include <new>
#include <cstdlib>
unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) seed = seed * 31 + p[i];
    return seed;
}
struct CCharStringData {
    char* PData;
    unsigned long StringLength;
};
class CWideString {
public:
    void* PStringData;
    CWideString() : PStringData(0) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { }
    ~CWideString() { }
    CWideString& operator=(const CWideString& other) { PStringData = other.PStringData; return *this; }
};
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }
    CWideString ToWideString() const { CWideString w; w.PStringData = PStringData; return w; }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyed = 0;
struct CRefVirtual {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }           // slot 1
};
namespace NGameText {
struct CDataBankTextEntry;
struct CGroupEntry : CRefVirtual, CRefCounted {
    void* TextPtr;
    CWideString GetText() const { CWideString w; w.PStringData = TextPtr; return w; }
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
    CIVCountedPointer() : Data(0) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    unsigned long KnownCRC; CGroupEntry* Entry;
    unsigned long GetTagFromCRC(unsigned long crc) const { return crc == KnownCRC ? 7 : 0; }
    CDataBankTextEntry* GetTextInfo(unsigned long tag) const { return tag == 7 ? (CDataBankTextEntry*)this : 0; }
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool b) const { CIVCountedPointer<CGroupEntry> p; p.Data = Entry; ++Entry->m_count; return p; }
};
}
struct CEngine {
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;            // +0x14
};
CEngine* g_pEngine;
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

#include <cstdio>
static char g_dummy[] = "QUEST_TEXT";
static char g_dummy2[] = "OTHER";
int main()
{
    static NGameText::CGroupEntry entry;
    entry.m_count = 1; entry.TextPtr = (void*)0x1234;
    static NGameText::CDataBank bank;
    CCharStringData d = { g_dummy, 10 };
    bank.KnownCRC = Calc(0, g_dummy, 10); bank.Entry = &entry;
    static CEngine engine; engine.PGameTextDataBank = &bank; g_pEngine = &engine;
    static CQuestManager qm;
    static void* hbuf[8]; CQuestManager::CObjectiveNode& header = *(CQuestManager::CObjectiveNode*)hbuf; header._M_next = &header; header._M_prev = &header;
    qm.Objectives._M_node = &header;
    CCharString tag; tag.PStringData = &d;
    qm.AddNewObjective(tag, 5);
    CCharStringData d2 = { g_dummy2, 5 };
    CCharString tag2; tag2.PStringData = &d2;
    qm.AddNewObjective(tag2, 6);
    int fails = 0;
    CQuestManager::CObjectiveNode* n1 = header._M_next;
    CQuestManager::CObjectiveNode* n2 = n1->_M_next;
    if (n1->_M_data.Text.PStringData != (void*)0x1234 || n1->_M_data.ID != 5 || n1->_M_data.State != 0) { printf("FAIL first\n"); ++fails; }
    if (n2->_M_data.Text.PStringData != (void*)&d2 || n2->_M_data.ID != 6) { printf("FAIL second\n"); ++fails; }
    if (n2->_M_next != &header || header._M_prev != n2 || n2->_M_prev != n1) { printf("FAIL links\n"); ++fails; }
    if (entry.m_count != 1) { printf("FAIL refcount %ld\n", entry.m_count); ++fails; }
    if (!fails) printf("004b1ba0_TEST PASS\n");
    return fails;
}
