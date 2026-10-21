// CTCFollowed::CullNullFollowers() -- drop every follower and AI follower that is gone or dead from the two follower lists (refreshing the player GUI after each dropped follower) and re-show the emote icon of every remaining one
// Leaf: CullNullFollowers
//
// Retail 0x006D5260, 340 bytes (8-aligned ebp frame, plain `ret`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C, CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0), the element's virtual
// destructor (slot 0, flag 0) and CRT `free` @ 0x00BFEA14 in the inlined list erase, CPlayerGui::Refresh(long) @ 0x004378C0 on
// g_pPlayerGui 0x013B8790, CTCFollowed::AddEmoteIcon(CThing*) @ 0x006D4FD0.
// Layout = Ego_r.pdb CTCFollowed: Followers +0x0c, AIFollowers +0x10 (retail STLport lists are one node pointer each; the PDB's
// are 12 bytes); list node {next, prev, CIntelligentPointer data (+8)}; CThing Dead bit 0 of +0x91.
// Shape: each loop copies the element into a local intelligent pointer (default base ctor + SetPItem(GetPItem())), tests it with
// the IS_THING_ALIVE double GetPItem, erases with the inline STLport list erase (destroy through the element pointer = virtual
// destructor call, then free the node).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CThingAICreature : public CThing { };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 (slot 0)
    void* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
template <class P> inline bool IS_THING_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->Dead; }

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct CRetailList {                 // STLport list: the header node pointer
    _List_node<T>* _M_node;
    _List_node<T>* begin() const { return _M_node->_M_next; }
    _List_node<T>* end() const { return _M_node; }
    _List_node<T>* erase(_List_node<T>* pos)
    {
        _List_node<T>* next = pos->_M_next;
        _List_node<T>* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        _Destroy(&pos->_M_data);
        free(pos);
        return next;
    }
};

struct CPlayerGui {
    void Refresh(long mode);                            // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
class CTCFollowed : public CTCBase {
public:
    CRetailList<CIntelligentPointer<CThing> >           Followers;     // +0x0c
    CRetailList<CIntelligentPointer<CThingAICreature> > AIFollowers;   // +0x10
    void AddEmoteIcon(CThing* follower);                // 0x006D4FD0
    void CullNullFollowers();
};

void CTCFollowed::CullNullFollowers()
{
    for (_List_node<CIntelligentPointer<CThing> >* it = Followers.begin(); it != Followers.end(); ) {
        CIntelligentPointer<CThing> follower(it->_M_data);
        if (!IS_THING_ALIVE(follower)) {
            it = Followers.erase(it);
            g_pPlayerGui->Refresh(0);
        } else {
            AddEmoteIcon(follower.GetPItem());
            it = it->_M_next;
        }
    }
    for (_List_node<CIntelligentPointer<CThingAICreature> >* it = AIFollowers.begin(); it != AIFollowers.end(); ) {
        CIntelligentPointer<CThingAICreature> follower(it->_M_data);
        if (!IS_THING_ALIVE(follower)) {
            it = AIFollowers.erase(it);
        } else {
            AddEmoteIcon(follower.GetPItem());
            it = it->_M_next;
        }
    }
}