// Grade: DIFFER(277v277) -- this/list-head registers swapped (retail ebx=this, edi=head) and the zero C3DVector stores sit before the AddMessage pushes instead of after `push 0x32`; the iterator insert(end()) form fixes the registers but not the link order/stores
// CQuestManager::SetBoastAsMade(long id, CCharString questName) -- accept a boast: bump the hero's boasts-made counter, append a copy of the boast to the accepted list, post a "boast made" message about it and remove it from the offered boasts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1A80, length 275 bytes, `ret 8` (questName by value, destroyed here).
// Callees: CCharString(const CCharString&) 0x0099EC30 (x2, the by-value arguments), CQuestManager::GetBoast(long,
// CCharString) 0x004AF7A0, CPlayerManager::GetMainPlayer 0x00449970 (g_pMainGameComponent 0x013B86A0 +0x1c),
// CPlayer::GetHeroThing 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (TC type 4),
// malloc 0x00BFEA0E (the list node), CBoast(const CBoast&) 0x004B73A0 (landed), CMessageEventManager::AddMessage
// 0x00687540 (World+0x60), CQuestManager::RemoveBoast(long, CCharString) 0x004AFE20, ~CCharString 0x0099EAE0.
// Layout (retail): AcceptedBoasts (STLport list<CBoast>, head pointer) +0x6c, World +0x7c; list node {next, prev,
// CBoast (0x20 bytes) +8}; CThing TC mask +0x20 (type 4 = word 0 bit 4), dead bit +0x91, Components +0x44; the TC's
// boast counter +0xcc.
// Shape: STLport `push_back` inlined (malloc'd node, null-tested placement copy, links prev-side first); the message
// position is a zero C3DVector temporary; HasTC + end/hit Find inline as in the landed 0x004B0D30 model.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
};
struct CBoast {                                        // Ego_r.pdb CQuestManager::CBoast (32 bytes)
    long        ID;                                    // +0x00
    long        Cost;                                  // +0x04
    long        Reward;                                // +0x08
    CCharString ScriptName;                            // +0x0c
    CWideString Text;                                  // +0x10
    CWideString Title;                                 // +0x14
    bool        Evil;                                  // +0x18
    bool        Failed;                                // +0x19
    bool        Completed;                             // +0x1a
    long        GroupIndex;                            // +0x1c
    CBoast(const CBoast& other);                       // 0x004B73A0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CTCBase;
struct CTCEntry {
    long     Type;
    CTCBase* TC;
};
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type);   // 0x0040F020
    __forceinline CTCBase* Find(long type)
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                            // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
};
struct CTCHeroBoasts {                                 // TC type 4 (the hero's stats component)
    unsigned char _pad00[0xcc];
    long          BoastsMade;                          // +0xcc
};

struct CPlayer {
    CThing* GetHeroThing();                            // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                          // 0x00449970
};
struct CMainGameComponent {
    unsigned char   _pad0[0x1c];
    CPlayerManager* PlayerManager;                     // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
struct CWorld {
    unsigned char         _pad0[0x60];
    CMessageEventManager* MessageEventManager;         // +0x60
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {                       // STLport list (head pointer, 4 bytes in retail)
    _List_node<T>* _M_node;
    T& back() { return _M_node->_M_prev->_M_data; }
    void push_back(const T& x)
    {
        _List_node<T>* n = _M_node;
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&tmp->_M_data) T(x);
        _List_node<T>* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};

struct CQuestManager {
    unsigned char _pad00[0x6c];
    list<CBoast>  AcceptedBoasts;                      // +0x6c
    unsigned char _pad70[0xc];
    CWorld*       World;                               // +0x7c
    CBoast* GetBoast(long id, CCharString questName);  // 0x004AF7A0
    void RemoveBoast(long id, CCharString questName);  // 0x004AFE20
    void SetBoastAsMade(long id, CCharString questName);
};

void CQuestManager::SetBoastAsMade(long id, CCharString questName)
{
    CBoast* boast = GetBoast(id, questName);
    if (boast == 0)
        return;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero != 0 && !hero->Dead && hero->HasTC(4))
        ++((CTCHeroBoasts*)hero->GetTC(4))->BoastsMade;
    AcceptedBoasts.push_back(*boast);
    World->MessageEventManager->AddMessage(0x29, 0x32, C3DVector(0, 0, 0), 0, 0, 0, 0, (long)&AcceptedBoasts.back());
    RemoveBoast(id, questName);
}
