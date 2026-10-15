// CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(a, b) -- (propagated label; see the note) copy a 0x2000-byte
// Grade: DIFFER(144v138) -- lane gap: `this` arrives in ESI (custom link-time convention; the body never loads esi, the caller 0x00C17364 sets it with `lea esi,[esp+14h]`), so a __thiscall model adds `mov esi,ecx` and shifts every frame offset by 4; the call sequence itself (ctor, Load, flag getter, Test, chunk + AppendText, dtor) lines up.
// resource into a 0x30-byte record and, when the record's quick-assign flag is set but the copy did not succeed,
// append a text to the owner's message chunk.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17260, length 138 bytes (+ int3 padding), `ret 8`, EH frame
// (`push -1 ; push offset ; mov eax,fs:[0]`, state -1/0 around the 0x30-byte local).
// LABEL NOTE: the manifest module/name is propagated from the AI body @ 0x0093D140; this is an engine function of
// the 0x00C1xxxx module (callers: 0x00C17364). ITS `this` ARRIVES IN ESI (the caller does `lea esi,[esp+14h] ; call`
// and the body reads esi without ever loading it): a link-time custom calling convention, not reproducible from
// plain C++ -- the lane can only model it as a __thiscall with `this` in ecx.
// Callees: the record constructor @ 0x00C2EED0 / destructor @ 0x00C2EF30 (labelled "WaypointData"), the loader
// @ 0x00C30F60 (__fastcall: argument 1 in ecx, the record in edx, 0x2000 on the stack), the quick-assign flag
// getter @ 0x00C307C0 (`mov al,[ecx+30h]`), the record's test @ 0x00C17820 (__fastcall(record, argument 2)),
// the message-chunk accessor @ 0x00C30840, the text appender @ 0x00C0DEC0 (__fastcall(chunk, text @ 0x012B2CD0)).
// Flags: /EHsc
struct CRecord0x30 {
    unsigned char _data[0x30];
    CRecord0x30();                              // 0x00C2EED0
    ~CRecord0x30();                             // 0x00C2EF30
    bool __fastcall Test(void* argument);       // 0x00C17820
};

struct CMessageChunk;
CMessageChunk* __fastcall AppendText(CMessageChunk* chunk, const char* text);   // 0x00C0DEC0
extern const char g_text012B2CD0[];             // 0x012B2CD0

struct CLoader {
    void __fastcall Load(CRecord0x30* record, long size);   // 0x00C30F60 (ecx = this, edx = record, stack = size)
};

struct CInventoryOwner {
    unsigned char _pad[0x30];
    bool m_canQuickAssign;                      // +0x30
    bool GetCanItemBeQuickAssigned();           // 0x00C307C0
    CMessageChunk* GetMessageChunk();           // 0x00C30840
    void UpdateCurrentAction(CLoader* loader, void* argument);
};

void CInventoryOwner::UpdateCurrentAction(CLoader* loader, void* argument)
{
    CRecord0x30 record;
    loader->Load(&record, 0x2000);
    if (GetCanItemBeQuickAssigned()) {
        if (!record.Test(argument))
            AppendText(GetMessageChunk(), g_text012B2CD0);
    }
}
