// CInstanceManager_00c2d5e0::Assign(request, theme) -- EH-framed (the manifest's "CEditTransactionSetEngineTheme" is propagated): after the request is registered (@ 0x00C2D210) the two 0x180-byte slots at +0x18 are scanned for a free one (@ 0x00C38840); a free slot takes the theme (@ 0x00C2D260 with the slot index) and its index is kept at +0x318; otherwise a fresh instance is created (@ 0x00C2D520), its absence raising the "Cannot be!" warning (CInstanceManager.cpp line 90: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot; state 0 covers the string), its +0x138 part reset (@ 0x00C3A950 with two zeros), the +0x33c flag set and the 0x1c-byte theme record assigned into +0x320 (the implicit memberwise assignment inlined, its +4 member through its own assignment @ 0x00C16950)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D5E0, oracle length 365 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; mov eax,[esp+14h] ; push ebx ; push esi ; push edi ; push eax ; mov esi,ecx ; call CInstanceManager::Register ; xor edi,edi ; lea ebx,[esi+18h] ; scan: mov ecx,ebx ; call CSlot::IsFree ; test al,al ; jne found ; inc edi ; add ebx,180h ; cmp edi,2 ; jb scan ; mov ecx,esi ; call CInstanceManager::Create ; mov edi,eax ; test edi,edi ; jne have ; mov edx,[g_pchNil] ; mov [esp+0Ch],edx ; mov edx,offset ".\CInstanceManager.cpp" ; lea ecx,[esp+0Ch] ; mov [esp+18h],eax ; call Append ; ... mov edx,5Ah ; ... call AppendNumber ; ... call Append ("Cannot be!") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],-1 ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; have: push 0 ; push 0 ; lea ecx,[edi+138h] ; call CPart::Reset ; mov edi,[esp+24h] ; mov byte ptr [esi+33Ch],1 ; mov eax,[edi] ; lea ecx,[edi+4] ; push ecx ; lea ecx,[esi+324h] ; mov [esi+320h],eax ; call CName::operator= ; mov edx,[edi+0Ch] ; mov [esi+32Ch],edx ; mov eax,[edi+10h] ; mov [esi+330h],eax ; mov ecx,[edi+14h] ; mov [esi+334h],ecx ; mov edx,[edi+18h] ; pop edi ; mov [esi+338h],edx ; ... ret 8 ; found: mov ecx,[esp+24h] ; push ecx ; push edi ; mov ecx,esi ; call CInstanceManager::Use ; mov ecx,[esp+10h] ; mov [esi+318h],edi ; ... ret 8
// The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c2d5e0)(const char* text);
extern PFNLOGSINK_00c2d5e0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c2d5e0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c2d5e0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c2d5e0();                       // 0x00C0BA40
    CString_00c2d5e0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c2d5e0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c2d5e0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c2d5e0();                         // 0x00C16750
};

struct CName_00c2d5e0 {                    // 8 bytes with its own assignment
    unsigned char _body[8];
    CName_00c2d5e0& operator=(const CName_00c2d5e0& other);   // 0x00C16950
};
struct STheme_00c2d5e0 {                   // 0x1c bytes
    long m_0;
    CName_00c2d5e0 m_name;                 // +4
    long m_c;
    long m_10;
    long m_14;
    long m_18;
};

struct CSlot_00c2d5e0 { unsigned char _body[0x180]; bool IsFree(); };   // 0x00C38840
struct CPart_00c2d5e0 { void Reset(long a, long b); };                  // 0x00C3A950
struct CInstance_00c2d5e0 { unsigned char _pad[0x138]; CPart_00c2d5e0 m_part; };

struct CInstanceManager_00c2d5e0 {
    unsigned char _pad[0x18];
    CSlot_00c2d5e0 m_slots[2];             // +0x18
    long m_slot;                           // +0x318
    long m_31c;
    STheme_00c2d5e0 m_theme;               // +0x320
    unsigned char m_bPending;              // +0x33c
    void Register(void* request);          // 0x00C2D210
    CInstance_00c2d5e0* Create();          // 0x00C2D520
    void Use(long slot, const STheme_00c2d5e0* theme);   // 0x00C2D260
    void Assign(void* request, const STheme_00c2d5e0* theme);
};

void CInstanceManager_00c2d5e0::Assign(void* request, const STheme_00c2d5e0* theme)
{
    Register(request);
    for (unsigned long slot = 0; slot < 2; ++slot) {
        if (m_slots[slot].IsFree()) {
            Use(slot, theme);
            m_slot = slot;
            return;
        }
    }
    CInstance_00c2d5e0* instance = Create();
    if (!instance) {
        {
            CString_00c2d5e0 line;
            line.Append(".\\CInstanceManager.cpp")->Append("(")->AppendNumber(90)->Append(") : ")->Append("Cannot be!")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c2d5e0();
    }
    instance->m_part.Reset(0, 0);
    m_bPending = 1;
    m_theme = *theme;
}