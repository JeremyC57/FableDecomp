// GFClearBadExitFlag() -- record in the registry that the game shut down cleanly
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401780, 47 bytes. Callees: Obj::Add, Obj::Set, Obj::Apply (registry key helpers).
//
// The front end sets a "bad exit" marker when the game starts and clears it again on an orderly
// shutdown; if the marker is still set on the next launch the game knows it crashed last time (the
// safe-mode / configuration-reset prompt keys off it). This is the clearing half: open the game's
// registry key, write the flag value as 0, and commit. The whole thing works on a stack-local key
// object that is torn down on return.
//
// Matching: the key object is a two-dword local; the two name strings are separate `.rdata`
// relocations pushed as immediates; the three member calls share `this` in ecx.
struct Obj {
    int a;      // registry key handle / state
    int b;      // second word of the key object
    void Add(const char* name, int flag);   // open (create) the key named `name`
    void Set(const char* name, int flag);   // write the value `name` = flag
    void Apply();                           // flush and close
};

// Registry key path and the "bad exit" value name.
extern const char g_nameA[];
extern const char g_nameB[];

void GFClearBadExitFlag()
{
    Obj o;
    o.Add(g_nameA, 0);
    o.Set(g_nameB, 0);
    o.Apply();
}
