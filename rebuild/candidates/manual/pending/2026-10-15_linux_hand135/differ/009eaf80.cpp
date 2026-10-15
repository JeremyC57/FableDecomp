// CConsole::AddCommandLineToHistory(const CConsoleCommandLine& line) -- keeps the console history at most 256 lines (dropping the oldest), inserts the line as the newest entry and refreshes the visible history window
// Grade: DIFFER(ours v retail) 105v107 -- (1) retail re-reads the list head (`mov eax,[edi]`) for pop_back after the size walk, ours reuses the head register (and swaps eax/ecx in the walk); (2) the insert arguments: retail pushes the line reference before building the begin() iterator in its slot, ours interleaves (`mov edx,[edi]` hoisted, `mov [eax],ecx` after the lea). Tried: pointer unlink vs prev/next locals, iterator copy constructor (fixed the in-slot build), erase(--end()) through iterator objects, SGI-distance size().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EAF80, length 107 bytes (112 with padding). The manifest's
// "CEngineManager::Cleanup" label is propagated; the landed caller CConsole::SendMessageToUser @ 0x009EB260 names it.
// Callees: the evicted line's virtual destructor (CConsoleCommandLine vtable 0x0129C400 slot 0, called with 0),
// the node free @ 0x00BFEA14, the history list's out-of-line insert(iterator, const CConsoleCommandLine&) @ 0x009EEB00
// (iterator returned through the hidden pointer, homed in the dead argument slot and ignored),
// CConsole::ResetVisibleHistory @ 0x009EAC80 (manifest "LinkedList_RotateToIndex", on `this`).
// Shape: the history is the retail sentinel-only list at +0x4C (see the landed NextInHistory @ 0x009EA9E0), reached through a
// pointer local (edi); `size()` is the STLport walk (`xor edx,edx ; ... inc edx`) and `>= 256` is unsigned (`jb`);
// pop_back unlinks head->prev (prev->next = next, next->prev = prev), destroys the value in place and frees the node;
// the begin() iterator is a 4-byte class with a user constructor, built in its argument slot (`push ecx ; mov eax,esp`).
struct CCharString {
    void* PStringData;
};

struct CBaseClass {
    virtual ~CBaseClass();                          // vtable 0x01231710
};

struct CConsoleCommandLine : CBaseClass {           // vtable 0x0129C400 (16 bytes), slot 0 = the destructor
    CCharString CurrentEntry;                       // +0x04
    bool        UserEntry;                          // +0x08
    long        CursorPosition;                     // +0x0c
    virtual ~CConsoleCommandLine();
};

void __cdecl FreeListNode(void* node);              // 0x00BFEA14

template<class T> struct CRetailList {              // retail std::list: the sentinel pointer only, node = {next, prev, value}
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        iterator() { }
        explicit iterator(node* q) : p(q) { }
        iterator(const iterator& other) : p(other.p) { }
        iterator& operator--() { p = p->prev; return *this; }
        iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const iterator& other) const { return p != other.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    unsigned long size() const
    {
        iterator first = iterator(head->next);
        iterator last = iterator(head);
        unsigned long n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }
    void erase(iterator where)
    {
        node* victim = where.p;
        node* prev = victim->prev;
        node* next = victim->next;
        prev->next = next;
        next->prev = prev;
        victim->value.~T();
        FreeListNode(victim);
    }
    void pop_back() { erase(--end()); }
    iterator insert(iterator where, const T& value);   // 0x009EEB00
};

struct CConsole_Retail {                            // retail offsets (see CConsole::NextInHistory @ 0x009EA9E0)
    virtual void v00();
    unsigned char                    _pad04[0x48];  // +0x04 .. +0x4c
    CRetailList<CConsoleCommandLine> CommandLineHistory;   // +0x4c
    void ResetVisibleHistory();                     // 0x009EAC80
    void AddCommandLineToHistory(const CConsoleCommandLine& line);
};

void CConsole_Retail::AddCommandLineToHistory(const CConsoleCommandLine& line)
{
    CRetailList<CConsoleCommandLine>* history = &CommandLineHistory;
    if (history->size() >= 256)
        history->pop_back();
    history->insert(history->begin(), line);
    ResetVisibleHistory();
}
