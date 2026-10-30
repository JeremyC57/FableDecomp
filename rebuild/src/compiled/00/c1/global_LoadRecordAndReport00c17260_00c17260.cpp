// LoadRecordAndReport_00c17260(CInventoryOwner* owner, CLoader* loader, void* argument) -- copy a 0x2000-byte
// resource into a 0x30-byte record and, when the record's quick-assign flag is set but the copy did not succeed,
// append a text to the owner's message chunk.
// Shape (retry1): parked as an "LTCG custom convention" gap (`this` in esi); as a file-static __stdcall with a same-TU caller
//   VC7.1 /O2 takes the owner in esi itself (chunk 585 lever). The loader 0x00C30F60 also takes the owner: __fastcall
//   (loader ecx, record edx, size and owner on the stack, `ret 8`), so `push esi` before `push 2000h` is that argument.
// Name: descriptive; the manifest's CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction is propagated from the AI
//   body at 0x0093D140 (wrong for this address).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17260, length 138 bytes (+ int3 padding), `ret 8`, EH frame
// (`push -1 ; push offset ; mov eax,fs:[0]`, state -1/0 around the 0x30-byte local).
// LABEL NOTE: this is an engine function of
// the 0x00C1xxxx module (callers: 0x00C17364). ITS `this` ARRIVES IN ESI (the caller does `lea esi,[esp+14h] ; call`
// and the body reads esi without ever loading it): the private convention VC7.1 gives the file-static below.
// Callees: the record constructor @ 0x00C2EED0 / destructor @ 0x00C2EF30 (labelled "WaypointData"), the loader
// @ 0x00C30F60 (__fastcall: argument 1 in ecx, the record in edx, 0x2000 on the stack), the quick-assign flag
// getter @ 0x00C307C0 (`mov al,[ecx+30h]`), the record's test @ 0x00C17820 (__fastcall(record, argument 2)),
// the message-chunk accessor @ 0x00C30840, the text appender @ 0x00C0DEC0 (__fastcall(chunk, text @ 0x012B2CD0)).
// Leaf: ?LoadRecordAndReport_00c17260@@YGXPAUCInventoryOwner@@PAUCLoader@@PAX@Z
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
    void __fastcall Load(CRecord0x30* record, long size, struct CInventoryOwner* owner);   // 0x00C30F60 (ecx = this, edx = record; size and owner on the stack, `ret 8`)
};

struct CInventoryOwner {
    unsigned char _pad[0x30];
    bool m_canQuickAssign;                      // +0x30
    bool GetCanItemBeQuickAssigned();           // 0x00C307C0
    CMessageChunk* GetMessageChunk();           // 0x00C30840
};

static __declspec(noinline) void __stdcall LoadRecordAndReport_00c17260(CInventoryOwner* owner, CLoader* loader, void* argument)
{
    CRecord0x30 record;
    loader->Load(&record, 0x2000, owner);
    if (owner->GetCanItemBeQuickAssigned()) {
        if (!record.Test(argument))
            AppendText(owner->GetMessageChunk(), g_text012B2CD0);
    }
}
// Same-TU caller (0x00C17364 passes a frame object in esi), present so the static gets the private convention. Not graded.
void LoadRecordAndReport_00c17260_caller(CLoader* loader, void* argument)
{
    CInventoryOwner owner;
    LoadRecordAndReport_00c17260(&owner, loader, argument);
}