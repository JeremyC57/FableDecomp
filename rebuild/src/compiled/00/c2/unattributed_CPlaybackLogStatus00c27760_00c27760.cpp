// CPlayback_00c27760::LogStatus() -- EH-framed under the playback section at 0x0143BAFC (@ 0x00C0FFC0 / 0x00C0FFE0, state 1) returning an MFC-style string by value (hidden result on the stack, its destruction guarded by the flag word; the manifest's "Playback_LogStatus"): a playing CODA source (+0xc through the `__stdcall` 0x00C35CA0) yields "CODA reports source IS playing(" position (0x00C35CC0) "/" the +0x20 length ")" built on a frame string (assigned @ 0x00C0C200, unsigned AppendNumber; state 2) and copied out (@ 0x00C0DAB0); otherwise "CODA reports source is NOT playing" is constructed straight into the result (@ 0x00C0BFE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27760, oracle length 298 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,10h ; push ebx ; push esi ; mov esi,ecx ; push offset g_playbackSection ; lea ecx,[esp+14h] ; mov dword ptr [esp+10h],0 ; call CProfileLock::CProfileLock ; mov eax,[esi+0Ch] ; mov ebx,1 ; push eax ; mov [esp+24h],ebx ; call IsPlaying ; test eax,eax ; je idle ; mov ecx,[g_pchNil] ; push edi ; mov [esp+0Ch],ecx ; push offset "CODA reports source IS playing" ; lea ecx,[esp+10h] ; mov byte ptr [esp+28h],2 ; call CString::Assign ; mov edx,[esi+0Ch] ; push edx ; call GetPosition ; mov edx,offset "(" ; lea ecx,[esp+0Ch] ; mov edi,eax ; call Append ; mov edx,edi ; mov ecx,eax ; call AppendNumber(unsigned) ; mov edx,offset "/" ; mov ecx,eax ; call Append ; mov edx,[esi+20h] ; mov ecx,eax ; call AppendNumber(unsigned) ; mov edx,offset ")" ; mov ecx,eax ; call Append ; mov esi,[esp+2Ch] ; lea eax,[esp+0Ch] ; push eax ; mov ecx,esi ; call CString::CString(const CString&) ; lea ecx,[esp+0Ch] ; mov [esp+10h],ebx ; mov byte ptr [esp+24h],bl ; call ~CString ; lea ecx,[esp+14h] ; mov byte ptr [esp+24h],0 ; call ~CProfileLock ; pop edi ; mov eax,esi ; ... ret 4 ; idle: mov esi,[esp+28h] ; push offset "CODA reports source is NOT playing" ; mov ecx,esi ; call CString::CString(const char*) ; lea ecx,[esp+10h] ; mov [esp+0Ch],ebx ; mov byte ptr [esp+20h],0 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 4
// The classes are named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
extern char* g_pchNil_0139c900;            // 0x0139C900
extern "C" long __stdcall CODA_IsSourcePlaying(void* source);             // 0x00C35CA0: +0x10 flags & 0x217
extern "C" unsigned long __stdcall CODA_GetSourcePosition(void* source);   // 0x00C35CC0

struct CString_00c27760 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c27760() : m_pchData(g_pchNil_0139c900) { }
    CString_00c27760(const char* text);    // 0x00C0BFE0
    CString_00c27760(const CString_00c27760& other);   // 0x00C0DAB0
    ~CString_00c27760();                       // 0x00C0BA40
    const CString_00c27760& operator=(const char* text);   // 0x00C0C200
    CString_00c27760* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c27760* __fastcall AppendNumber(unsigned long value);   // 0x00C0DF10
};

class PKCProtectionInstance { public:             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();              // 0x00C0FFE0
};

struct CPlayback_00c27760 {
    long m_0;
    long m_4;
    long m_8;
    void* m_pSource;                       // +0xc
    unsigned char _pad[0x10];
    unsigned long m_length;                // +0x20
    CString_00c27760 LogStatus();
};

CString_00c27760 CPlayback_00c27760::LogStatus()
{
    PKCProtectionInstance lock(&g_soundLock);
    if (CODA_IsSourcePlaying(m_pSource)) {
        CString_00c27760 line;
        line = "CODA reports source IS playing";
        unsigned long position = CODA_GetSourcePosition(m_pSource);
        line.Append("(")->AppendNumber(position)->Append("/")->AppendNumber(m_length)->Append(")");
        return line;
    }
    return CString_00c27760("CODA reports source is NOT playing");
}