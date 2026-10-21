// CScreenEffectPriorityStack::Process() -- (re)register the per-effect-type handler tables: nine radial-blur handlers, two displacement handlers and one screen-filter handler keyed by entry type
// Leaf: Process
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B08B0, length 425 bytes (`ret`, aligned ebp frame `and esp,-8`); its
// only reference is a tail `jmp` at 0x008B13BC (ecx = this+4, unused here). Callees: the three maps' insert
// @ 0x008B2DD0 / 0x008B2E90 / 0x008B2F50 (manifest "RBTree_Insert" / "std::map::insert"), handler functions
// 0x00905500 (`xor al,al ; ret`), 0x00905690, 0x009056F0, 0x00905510, 0x00905530, 0x009055E0, 0x009059D0, 0x00905A20,
// 0x00905C30. Globals: the handler maps 0x013BAFA8 / 0x013BAFB4 / 0x013BAFC0 (0x013BAFA8 is also searched by
// 0x008B07E0 with a CStackEntry type).
// Label: the catalog name Process is kept, but the body only fills three global int -> handler maps (radial blur
// types 0..8, displacement 0..1, screen filter 0); the real name is not known.
// Shape: each insert takes a CHandlerPair temporary (key, function pointer) and returns an 8-byte pair<iterator,bool>
// by hidden pointer that is discarded: VC7.1 RTM shares ONE 8-aligned result slot and ONE pair slot for all twelve
// calls here (hence `and esp,-8`); the key-0 entries store a null handler through esi.
struct CStackEntry;
typedef bool (*THandler)(CStackEntry* entry);
bool HandlerR1(CStackEntry*);   // 0x00905500
bool HandlerR2(CStackEntry*);      // 0x00905690
bool HandlerR3(CStackEntry*);      // 0x009056F0
bool HandlerR4(CStackEntry*);      // 0x00905510
bool HandlerR5(CStackEntry*);      // 0x00905530
bool HandlerR6(CStackEntry*);      // 0x009055E0
bool HandlerR7(CStackEntry*);      // 0x009059D0
bool HandlerR8(CStackEntry*);      // 0x00905A20
bool HandlerD1(CStackEntry*);      // 0x00905C30
struct CHandlerPair {
    long     first;
    THandler second;
    CHandlerPair(long k, THandler f) : first(k), second(f) {}
};
struct CInsertResult { void* Position; bool Inserted; CInsertResult() {} };
struct CHandlerMapA { void* Head; unsigned long Count; unsigned long Compare; CInsertResult insert(const CHandlerPair& v); };   // 0x008B2DD0
struct CHandlerMapB { void* Head; unsigned long Count; unsigned long Compare; CInsertResult insert(const CHandlerPair& v); };   // 0x008B2E90
struct CHandlerMapC { void* Head; unsigned long Count; unsigned long Compare; CInsertResult insert(const CHandlerPair& v); };   // 0x008B2F50
extern CHandlerMapA g_RadialBlurHandlers;        // 0x013BAFA8
extern CHandlerMapB g_DisplacementHandlers;      // 0x013BAFB4
extern CHandlerMapC g_ScreenFilterHandlers;      // 0x013BAFC0

struct CScreenEffectPriorityStack {
    void Process();
};

void CScreenEffectPriorityStack::Process()
{
    g_RadialBlurHandlers.insert(CHandlerPair(0, 0));
    g_RadialBlurHandlers.insert(CHandlerPair(1, HandlerR1));
    g_RadialBlurHandlers.insert(CHandlerPair(2, HandlerR2));
    g_RadialBlurHandlers.insert(CHandlerPair(3, HandlerR3));
    g_RadialBlurHandlers.insert(CHandlerPair(4, HandlerR4));
    g_RadialBlurHandlers.insert(CHandlerPair(5, HandlerR5));
    g_RadialBlurHandlers.insert(CHandlerPair(6, HandlerR6));
    g_RadialBlurHandlers.insert(CHandlerPair(7, HandlerR7));
    g_RadialBlurHandlers.insert(CHandlerPair(8, HandlerR8));
    g_DisplacementHandlers.insert(CHandlerPair(0, 0));
    g_DisplacementHandlers.insert(CHandlerPair(1, HandlerD1));
    g_ScreenFilterHandlers.insert(CHandlerPair(0, 0));
}
