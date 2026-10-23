// Grade: DIFFER(569v569) -- register colouring only, two sites: retail loads the quest name into edx and the local
//   list's address into ecx for GetAcceptedBoastsForScript (`mov edx,[ebx+8] ; lea ecx,[esp+14h] ; push ecx ; add edx,30h
//   ; push edx`), ours uses ecx/eax; and retail's GetInterface key address is in eax, ours edx. Everything else matches:
//   the IDs read into locals before the by-value name copies, the zero position built inside an inline CQuestManager
//   message helper (stores land after the 5000 push, as retail), and same-TU auto_inline(off) stand-ins for clear /
//   GetAcceptedBoastsForScript so the local list's header stays in edi across the calls (the landed
//   CTCWillLightning::InitialiseBeams 0x007A16F0 lever). Tried: a quest local, a GetName() accessor, a const self,
//   find with both bounds inline, the KickOffDeathScreen GetInterface form, the list as the parameter's own class,
//   stand-ins after the function, QFE.
// CQuestManager::PostBoastTransfer() -- move every temp boast into the accepted list (carrying its failed/completed flags, removing the offered one and posting "boast accepted" while it is still open); then, for the first active quest between its start and finish screens that has accepted boasts, name it on the hero's quest-completion UI and post the boast-UI message
// Leaf: PostBoastTransfer
// Retail 0x004B2890, 576 bytes, __thiscall, plain `ret` (Ego_r.pdb ?PostBoastTransfer@CQuestManager@@QAEXXZ).
// Callees: CQuestManager::GetBoast(long, CCharString) 0x004AF7A0 / RemoveBoast(long, CCharString) 0x004AFE20 (landed),
// CCharString copy 0x0099EC30 / operator= 0x0099EFB0, malloc 0x00BFEA0E, CBoast(const CBoast&) 0x004B73A0,
// CMessageEventManager::AddMessage 0x00687540 (World +0x60), CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetHeroCreature 0x00487DC0, STLport __find(first, last, val, random_access_iterator_tag) 0x004B8E40
// (`__fastcall`), GetAcceptedBoastsForScript 0x004B1960, list<CBoast>::clear 0x004B8C00, free 0x00BFEA14,
// CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xFC = CTCQuestCompletionUI, bit 28 of +0x3c).
// Layout: retail STLport CQuestManager (ActiveQuests +0x38, BetweenStartAndEndScreenQuests +0x4c, AcceptedBoasts +0x6c,
// TempBoasts +0x70, World +0x7c); CActiveQuest Name +0x30; CThing Dead +0x91 bit 0; CTCQuestCompletionUI quest name +0x50.
// Shape: the zero position is a temporary built during the argument pushes; GetBoast's result is re-read after
// RemoveBoast (retail reads the flags of the removed entry); the local boast list's dtor frees its sentinel unguarded.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& val,
                                   const random_access_iterator_tag& tag);   // 0x004B8E40
inline CCharString* find(CCharString* first, CCharString* last, const CCharString& val)
{
    return FindString(first, last, val, random_access_iterator_tag());
}
struct CWideString { void* PStringData; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long distance, const C3DVector& pos, CThing* source, CThing* extra, float a,
                     float b, const void* data);       // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessages;                  // +0x60
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCQuestCompletionUI {                           // interface 0xFC
    unsigned char _pad[0x50];
    CCharString QuestName;                              // +0x50
};
struct CPlayer { CThing* GetHeroCreature(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CEngine {
    unsigned char _pad00[0x1c];
    CPlayerManager* PPlayerManager;                     // +0x1c
};
extern CEngine* g_pEngine;                              // 0x013B86A0

struct CBoast {                                         // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;
    long        Cost;
    long        Reward;
    CCharString ScriptName;                             // +0x0c
    CWideString Text;
    CWideString Title;
    bool        Evil;
    bool        Failed;                                 // +0x19
    bool        Completed;                              // +0x1a
    long        GroupIndex;
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
    bool empty() const { return m_node->next == m_node; }
    CBoast& back() { return m_node->prev->value; }
    void clear();                                       // 0x004B8C00
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
struct CLocalBoastList : CBoastList {
    CLocalBoastList()
    {
        CBoastNode* n = (CBoastNode*)malloc(sizeof(CBoastNode));
        n->next = n;
        n->prev = n;
        m_node = n;
    }
    ~CLocalBoastList()
    {
        clear();
        free(m_node);
    }
};
struct CActiveQuest {
    unsigned char _pad[0x30];
    CCharString Name;                                   // +0x30
};
struct CQuestNode { CQuestNode* next; CQuestNode* prev; CActiveQuest* value; };
struct CQuestList { CQuestNode* m_node; };
struct CStringVector { CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage; };

struct CQuestManager {
    unsigned char _pad00[0x38];
    CQuestList    ActiveQuests;                         // +0x38
    unsigned char _pad3c[0x4c - 0x3c];
    CStringVector BetweenStartAndEndScreenQuests;       // +0x4c
    unsigned char _pad58[0x6c - 0x58];
    CBoastList    AcceptedBoasts;                       // +0x6c
    CBoastList    TempBoasts;                           // +0x70
    unsigned char _pad74[0x7c - 0x74];
    CWorld*       World;                                // +0x7c

    CBoast* GetBoast(long id, CCharString scriptName);                              // 0x004AF7A0
    void RemoveBoast(long id, CCharString scriptName);                              // 0x004AFE20
    void GetAcceptedBoastsForScript(const CCharString& scriptName, CBoastList& boasts) const;   // 0x004B1960
    void PostMessage(long type, CThing* source, const void* data)
    {
        World->m_pMessages->AddMessage(type, 5000, C3DVector(0.0f, 0.0f, 0.0f), source, 0, 0.0f, 0.0f, data);
    }
    void PostBoastTransfer();
};

// Same-TU stand-ins for list<CBoast>::clear 0x004B8C00 and GetAcceptedBoastsForScript 0x004B1960 (both sit in this
// TU): only their visibility matters -- VC7.1 then knows the local list's header word survives the calls and keeps it
// in a register, as retail's compiler did. Neither is graded.
#pragma auto_inline(off)
void CBoastList::clear()
{
    CBoastNode* cur = m_node->next;
    while (cur != m_node) {
        CBoastNode* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    m_node->next = m_node;
    m_node->prev = m_node;
}
void CQuestManager::GetAcceptedBoastsForScript(const CCharString& scriptName, CBoastList& boasts) const
{
    boasts.clear();
    for (CBoastNode* node = AcceptedBoasts.m_node->next; node != AcceptedBoasts.m_node; node = node->next)
        if (node->value.ScriptName.PStringData == scriptName.PStringData)
            boasts.push_back(node->value);
}
#pragma auto_inline(on)

void CQuestManager::PostBoastTransfer()
{
    for (CBoastNode* node = TempBoasts.m_node->next; node != TempBoasts.m_node; node = node->next) {
        long id = node->value.ID;
        CBoast* boast = GetBoast(id, node->value.ScriptName);
        if (boast) {
            if (node->value.Failed)
                boast->Failed = true;
            if (node->value.Completed)
                boast->Completed = true;
            AcceptedBoasts.push_back(*boast);
            long acceptedID = boast->ID;
            RemoveBoast(acceptedID, boast->ScriptName);
            if (!boast->Completed && !boast->Failed)
                PostMessage(0x29, 0, &AcceptedBoasts.back());
        }
    }
    CThing* hero = g_pEngine->PPlayerManager->GetMainPlayer()->GetHeroCreature();
    if (hero == 0 || hero->Dead)
        return;
    for (CQuestNode* q = ActiveQuests.m_node->next; q != ActiveQuests.m_node; q = q->next) {
        CCharString* end = BetweenStartAndEndScreenQuests._M_finish;
        if (find(BetweenStartAndEndScreenQuests._M_start, end, q->value->Name) != end) {
            CLocalBoastList boasts;
            GetAcceptedBoastsForScript(q->value->Name, boasts);
            if (!boasts.empty()) {
                CTCQuestCompletionUI* ui;
                if (hero->GetInterface(&ui, 0xfc)) {
                    ui->QuestName = q->value->Name;
                    PostMessage(0x49, hero, 0);
                }
                return;
            }
        }
    }
}
