// CThingPlayerCreature::ResetThingReactions() -- collect every leaf thing of the search tools' list, clear the intelligent pointer each one keeps at +0xf4, then free the temporary list
// Grade: DIFFER(182v182) -- register swap only: retail keeps the list head node in edi and the iterator/this in esi, ours the reverse (declaring the iterator before the list changes nothing). The GetAllLeafThings body above (auto_inline off) is what lets VC keep the head in a register across the call.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AC140, length 182 bytes (192 with padding).
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport list nodes), CThing::GetThingSearchTools @ 0x004C7A50
// (`mov ecx,[0x013B8A1C] ; jmp 0x0051E490`; manifest label "_Cons_val<...>" is propagated),
// CThingSearchTools::GetAllLeafThings<CThing, list<CThing*>, CThingFilter_None> @ 0x006AE830 (manifest: the vector
// instantiation, but the body appends 12-byte list nodes), CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem
// @ 0x00A01B90, GetPItem @ 0x00A01B50, dtor @ 0x00A01C10. CIntelligentPointer<CThing> vtable 0x0123E7D8.
// Shape: the STLport list built in a local node then stored; `thing->m_reactionTarget = CIntelligentPointer<CThing>(0)`
// per node (converting constructor = base ctor + vtable + SetPItem, then operator= = SetPItem(GetPItem())); the
// inline STLport clear and node free at the end.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThing
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    virtual void v00();
};

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags;                                  // +0x91 (bit 2 active, 3 in world, 0 dead, 5 limbo)
    unsigned char _pad92[0xf4 - 0x92];
    CIntelligentPointer<CThing> m_reactionTarget;          // +0xf4 (descriptive)
};

struct SThingNode {                                         // STLport list<CThing*> node, 12 bytes
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CThing*     _M_data;
};
struct CThingList {                                         // STLport list<CThing*>: the head node pointer
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

struct CThingFilter_None { CThingFilter_None() { } };
struct CThingSearchTools {
    CThingList* GetThingList(long type);                    // 0x0049C780
    void GetAllLeafThings(const CThingFilter_None& filter, CThingList& out);   // 0x006AE830
};

#pragma auto_inline(off)
void CThingSearchTools::GetAllLeafThings(const CThingFilter_None&, CThingList& out)   // 0x006AE830 (the real body)
{
    CThingList* all = GetThingList(1);
    for (SThingNode* n = all->_M_node->_M_next; n != all->_M_node; n = n->_M_next) {
        CThing* thing = n->_M_data;
        unsigned char flags = thing->m_flags;
        if ((flags & 4) && (flags & 8) && ((unsigned char)~flags & 1) && !(flags & 0x20)) {
            SThingNode* pos = out._M_node;
            SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
            new (&node->_M_data) CThing*(thing);
            SThingNode* prev = pos->_M_prev;
            node->_M_next = pos;
            node->_M_prev = prev;
            prev->_M_next = node;
            pos->_M_prev = node;
        }
    }
}
#pragma auto_inline(on)

struct CThingPlayerCreature : CThing {
    CThingSearchTools* GetThingSearchTools();               // 0x004C7A50
    void ResetThingReactions();
};

void CThingPlayerCreature::ResetThingReactions()
{
    CThingList things;
    GetThingSearchTools()->GetAllLeafThings(CThingFilter_None(), things);
    for (SThingNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data;
        thing->m_reactionTarget = CIntelligentPointer<CThing>(0);
    }
}
