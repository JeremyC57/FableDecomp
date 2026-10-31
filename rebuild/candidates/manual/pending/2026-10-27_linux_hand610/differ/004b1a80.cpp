// CQuestManager::SetBoastAsMade(long id, CCharString questName) -- accept a boast: bump the hero's boasts-made counter, append a copy of the boast to the accepted list, post a "boast made" message about it and remove it from the offered boasts
// Grade: DIFFER(277v277) -- wave 25: callee spellings now PDB-exact (CQuestManager::CBoast nested, CPlayerManager::GetMainPlayer returning CPlayer&, CPlayer::GetPControlledCreature() const, AddMessage(EMessageEventType, ...)); residue unchanged: `this` in edi / list head in ebx where retail has ebx / edi, and the zero C3DVector temporary stores plus the MessageEventManager load sit before the AddMessage pushes instead of after `push 32h`. Tried this wave: STLport 4.6 _List_iterator/_List_node_base model with a user copy ctor and back() = *--end() (279v277, worse), SGI-form insert through position._M_node with only __prev local (inert), name fixes (inert). Lane gap: equal-use register colouring + store sinking past pushes.
// Leaf: ?SetBoastAsMade@CQuestManager@@QAEXJVCCharString@@@Z
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

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
};

class C3DVector {
public:
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
class CThing {
public:
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

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                          // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad0[0x1c];
    CPlayerManager* PlayerManager;                     // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

enum EMessageEventType { MESSAGE_EVENT_BOAST_MADE = 0x29 };
class CMessageEvent;
class CMessageEventManager {
public:
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CWorld {
public:
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
    struct iterator { _List_node<T>* _M_node; iterator(_List_node<T>* n) : _M_node(n) {} };
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&tmp->_M_data) T(x);
        return tmp;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node<T>* n = position._M_node;
        _List_node<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};

class CQuestManager {
public:
    class CBoast {                                     // Ego_r.pdb CQuestManager::CBoast (32 bytes)
    public:
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
    unsigned char _pad00[0x6c];
    list<CBoast>  AcceptedBoasts;                      // +0x6c
    unsigned char _pad70[0xc];
    CWorld*       World;                               // +0x7c
    CBoast* GetBoast(long id, CCharString questName);  // 0x004AF7A0 (CQuestManager::CBoast)
    void RemoveBoast(long id, CCharString questName);  // 0x004AFE20
    void SetBoastAsMade(long id, CCharString questName);
};

void CQuestManager::SetBoastAsMade(long id, CCharString questName)
{
    CBoast* boast = GetBoast(id, questName);
    if (boast == 0)
        return;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero != 0 && !hero->Dead && hero->HasTC(4))
        ++((CTCHeroBoasts*)hero->GetTC(4))->BoastsMade;
    AcceptedBoasts.push_back(*boast);
    World->MessageEventManager->AddMessage(MESSAGE_EVENT_BOAST_MADE, 0x32, C3DVector(0, 0, 0), 0, 0, 0, 0, (long)&AcceptedBoasts.back());
    RemoveBoast(id, questName);
}
