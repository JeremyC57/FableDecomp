// RegisterThingHandlers() -- static registration: insert four (id, CThing member handler) pairs into the global handler map
// Leaf: RegisterThingHandlers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008162F0, length 147 bytes (+13 int3 padding).
// NOT CScreenEffectPriorityStack::Process (propagated label): a free function called once from the game-component
// registration chain (0x004EE286, next to the other static registrars). Each block builds a pair<const long,
// PMF> temporary at the bottom of the frame and calls the Dinkumware _Tree::insert(const value_type&) @ 0x00818000
// on the map object @ 0x013BAF2C, returning pair<iterator, bool> through a hidden pointer (frame +8), ignored.
// The handlers (0x008F5D00, 0x008F5CF0, 0x008F5D40, 0x008F5E00) are __thiscall members that test CThing flags
// (+0x2C) and look a thing component up in the +0x44 component map, so the value is a 4-byte CThing member pointer.
// Names are descriptive (map and handlers not identified).
// Shape: a direct `value_type(id, &CThing::X)` temporary stores both words AFTER the two pushes
// (GOTCHAS chunk 149: make_pair would store the second word before them).
class CThing {
public:
    void HandlerType1();     // 0x008F5D00
    void HandlerType2();     // 0x008F5CF0
    void HandlerType3();     // 0x008F5D40
    void HandlerType4();     // 0x008F5E00
};
typedef void (CThing::*PThingHandler)();

struct SHandlerEntry {                           // pair<const long, PThingHandler>
    long          first;
    PThingHandler second;
    SHandlerEntry(const long& id, const PThingHandler& handler) : first(id), second(handler) {}
};

struct CHandlerMap {                             // Dinkumware map<long, PThingHandler>
    struct iterator { void* _Ptr; };
    struct insert_result {
        iterator first;
        bool     second;
        insert_result() {}
    };
    insert_result insert(const SHandlerEntry& value);   // 0x00818000
    void* _Myhead;
    unsigned int _Mysize;
};
extern CHandlerMap g_ThingHandlerMap;            // 0x013BAF2C

void RegisterThingHandlers()
{
    g_ThingHandlerMap.insert(SHandlerEntry(1, &CThing::HandlerType1));
    g_ThingHandlerMap.insert(SHandlerEntry(2, &CThing::HandlerType2));
    g_ThingHandlerMap.insert(SHandlerEntry(3, &CThing::HandlerType3));
    g_ThingHandlerMap.insert(SHandlerEntry(4, &CThing::HandlerType4));
}
