// Grade: DIFFER(199v202) -- prologue only: retail pushes ebx/ebp/esi/edi before the empty-list test (the early exit pops all four), ours shrink-wraps ebx/ebp/esi after it; the loop body is identical
// CMessageEventManager::ForEachMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween>,CGameScriptInterface_MsgOnLevelLoaded_Callback>(const Filter& filter, Callback& callback) -- run the callback on every queued message that passes both filters and count them; the callback appends each loaded level's script name
// Leaf: ForEachMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AD050, length 202 bytes (`ret 8`).
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0 (labelled CMessageEventFilter_HasExtraData), CMessageEventFilter_OccuredBetween::
// operator() @ 0x006AF180, CWorld::PeekWorldMap (vtable slot 13), CWorldMap::GetLevelScriptName(long) @ 0x004FCFC0, malloc @ 0x00BFEA0E
// (the 12-byte list node), CCharString copy ctor @ 0x0099EC30.
// Layout: Ego_r.pdb CMessageEventManager World +0, Messages +4 (retail one-pointer list; node next +0, prev +4, message +8);
// CMessageEvent::ExtraData +0x3C (the level number for this message type); CFilter_Bind2 {F1, F2} pointers.
// Shape: the count, the cursor and `this` live in the frame; the callback's level-name argument is pushed before the PeekWorldMap virtual
// call (`World->PeekWorldMap()->GetLevelScriptName(message.ExtraData)`); the name list has no size word (push_back links before the head).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};
struct CMessageEvent {
    unsigned char _pad00[0x3c];
    long          ExtraData;                    // +0x3c
};
struct CMessageEventFilter_IsOfType {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
struct CMessageEventFilter_OccuredBetween {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF180
};
template<class F1, class F2> struct CFilter_Bind2 {
    F1* PF1;
    F2* PF2;
    bool operator()(const CMessageEvent& message) const { return (*PF1)(message) && (*PF2)(message); }
};
struct CWorldMap {
    const CCharString& GetLevelScriptName(long level) const;   // 0x004FCFC0
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* PeekWorldMap();          // slot 13 (+0x34)
};
template<class T> struct CList {                // retail one-pointer list (no size)
    struct node { node* next; node* prev; T value; };
    node* head;
    void push_back(const T& value)
    {
        node* pos = head;
        node* n = (node*)malloc(sizeof(node));
        new (&n->value) T(value);
        node* prev = pos->prev;
        n->prev = prev;
        n->next = pos;
        prev->next = n;
        pos->prev = n;
    }
};
struct CGameScriptInterface_MsgOnLevelLoaded_Callback {
    CWorld*             World;
    CList<CCharString>* LevelNames;
    void operator()(const CMessageEvent& message) { LevelNames->push_back(World->PeekWorldMap()->GetLevelScriptName(message.ExtraData)); }
};
struct CMessageEventManager {
    CWorld*              World;                 // +0x00
    CList<CMessageEvent> Messages;              // +0x04
    template<class Filter, class Callback> long ForEachMessage(const Filter& filter, Callback& callback);
};

template<class Filter, class Callback> long CMessageEventManager::ForEachMessage(const Filter& filter, Callback& callback)
{
    long count = 0;
    for (CList<CMessageEvent>::node* n = Messages.head->next; n != Messages.head; n = n->next) {
        const CMessageEvent* message = &n->value;
        if (filter(*message)) {
            callback(*message);
            ++count;
        }
    }
    return count;
}
template long CMessageEventManager::ForEachMessage<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>,
    CGameScriptInterface_MsgOnLevelLoaded_Callback>(const CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>&,
    CGameScriptInterface_MsgOnLevelLoaded_Callback&);
