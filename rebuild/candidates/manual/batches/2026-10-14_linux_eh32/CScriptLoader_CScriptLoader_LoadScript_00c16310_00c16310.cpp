// CScriptLoader_00c16310::LoadScript(script, params, record) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0; the manifest's "Script_LoadOakValeRevisited" is propagated): a 0x14-byte range on the frame takes the parameter block's +4 limit, +0 kind (16-bit) and +0xc tag, its start and end converted from the +0x10 offset and the +0x10 + +0x14 span (@ 0x00C2D820) and both cleared unless start <= end < limit; the +8 engine opens the script over the range with the parameter block's +8 (@ 0x00C0E770); a record builder on the frame (@ 0x00C1BD10) takes the fourteen fields of the caller's record through its setters (@ 0x00C1BB20..0x00C1BC70) and the engine applies it to the opened script (@ 0x00C0E870), whose result is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16310, oracle length 408 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,30h ; push ebx ; push esi ; push edi ; mov edi,ecx ; push offset g_section ; lea ecx,[esp+10h] ; call CProfileLock::CProfileLock ; mov esi,[esp+50h] ; mov eax,[esi+4] ; mov cx,[esi] ; mov edx,[esi+0Ch] ; xor ebx,ebx ; mov [esp+14h],eax ; mov eax,[esi+10h] ; mov [esp+20h],cx ; push eax ; lea ecx,[esp+18h] ; mov [esp+48h],ebx ; mov [esp+1Ch],ebx ; mov [esp+20h],ebx ; mov [esp+26h],bx ; mov [esp+28h],edx ; call SRange::Convert ; mov ecx,[esi+14h] ; add ecx,[esi+10h] ; push ecx ; lea ecx,[esp+18h] ; mov [esp+1Ch],eax ; call SRange::Convert ; mov edx,[esp+18h] ; mov ecx,[esp+14h] ; cmp edx,ecx ; mov [esp+1Ch],eax ; jae zero ; cmp eax,ecx ; jae zero ; cmp edx,eax ; jbe keep ; zero: mov [esp+1Ch],ebx ; mov [esp+18h],ebx ; keep: mov edx,[esi+8] ; mov eax,[esp+4Ch] ; push edx ; push eax ; lea ecx,[esp+1Ch] ; push ecx ; mov ecx,[edi+8] ; call CEngine::Open ; lea ecx,[esp+28h] ; mov ebx,eax ; call CRecordBuilder::CRecordBuilder ; mov esi,[esp+54h] ; (fourteen setters) ; mov ecx,[edi+8] ; lea edx,[esp+28h] ; push edx ; push ebx ; call CEngine::Apply ; lea ecx,[esp+0Ch] ; mov esi,eax ; mov dword ptr [esp+44h],-1 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 0Ch
// The classes are named by address.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c16310 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c16310(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c16310();              // 0x00C0FFE0
};

struct SParams_00c16310 {                  // the caller's parameter block
    unsigned short m_kind;                 // +0
    unsigned long m_limit;                 // +4
    void* m_pContext;                      // +8
    long m_tag;                            // +0xc
    unsigned long m_offset;                // +0x10
    unsigned long m_length;                // +0x14
};
struct SRange_00c16310 {                   // 0x14 bytes on the frame
    unsigned long m_limit;
    unsigned long m_start;
    unsigned long m_end;
    unsigned short m_kind;
    unsigned short m_e;
    long m_tag;
    unsigned long Convert(unsigned long offset);   // 0x00C2D820
};
struct SRecord_00c16310 {                  // the caller's record
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18;
    unsigned short m_1c, m_1e;
    long m_20;
    unsigned char m_24;
    long m_28;
};
struct CRecordBuilder_00c16310 {           // 0x14 bytes on the frame
    unsigned char _body[0x14];
    CRecordBuilder_00c16310();             // 0x00C1BD10
    void Set0(long v);                     // 0x00C1BB20
    void Set4(long v);                     // 0x00C1BB40
    void Set8(long v);                     // 0x00C1BB60
    void SetC(long v);                     // 0x00C1BB80
    void Set10(long v);                    // 0x00C1BBA0
    void Set14(long v);                    // 0x00C1BBC0
    void Set18(long v);                    // 0x00C1BBD0
    void Set1C(unsigned short v);          // 0x00C1BBE0
    void Set1E(unsigned short v);          // 0x00C1BBF0
    void Set20(long v);                    // 0x00C1BC00
    void Set24(unsigned char v);           // 0x00C1BC10
    void Set28(long v);                    // 0x00C1BC70
};
struct CEngine_00c16310 {
    void* Open(const SRange_00c16310* range, void* script, void* context);   // 0x00C0E770
    long Apply(void* handle, const CRecordBuilder_00c16310* record);      // 0x00C0E870
};

struct CScriptLoader_00c16310 {
    long m_0;
    long m_4;
    CEngine_00c16310* m_pEngine;           // +8
    long LoadScript(void* script, const SParams_00c16310* params, const SRecord_00c16310* record);
};

long CScriptLoader_00c16310::LoadScript(void* script, const SParams_00c16310* params, const SRecord_00c16310* record)
{
    CProfileLock_00c16310 lock(g_section_0143bb14);
    SRange_00c16310 range;
    range.m_limit = params->m_limit;
    range.m_start = 0;
    range.m_end = 0;
    range.m_kind = params->m_kind;
    range.m_e = 0;
    range.m_tag = params->m_tag;
    range.m_start = range.Convert(params->m_offset);
    range.m_end = range.Convert(params->m_length + params->m_offset);
    if (!(range.m_start < range.m_limit && range.m_end < range.m_limit && range.m_start <= range.m_end)) {
        range.m_end = 0;
        range.m_start = 0;
    }
    void* handle = m_pEngine->Open(&range, script, params->m_pContext);
    CRecordBuilder_00c16310 builder;
    builder.Set0(record->m_0);
    builder.Set4(record->m_4);
    builder.Set8(record->m_8);
    builder.SetC(record->m_c);
    builder.Set10(record->m_10);
    builder.Set14(record->m_14);
    builder.Set18(record->m_18);
    builder.Set1C(record->m_1c);
    builder.Set1E(record->m_1e);
    builder.Set20(record->m_20);
    builder.Set24(record->m_24);
    builder.Set28(record->m_28);
    return m_pEngine->Apply(handle, &builder);
}
