// CInstanceManager::Assign(float fadeTime, const SThemeRequest* request) -- EH-framed: fade every busy instance out over fadeTime (FadeOutAll @ 0x00C2D210), then start the request on a free instance of the two at +0x18 (IsIdle @ 0x00C38840; StartInstance @ 0x00C2D260, index kept at +0x318); with no free instance take over the quietest one (FindQuietestInstance @ 0x00C2D520, "Cannot be!" reported when there is none: CInstanceManager.cpp line 90), ramp its volume to 0 at once (@ 0x00C3A950), set the pending flag +0x33c and queue a copy of the 0x1c-byte request at +0x320 (its +4 member through its own assignment @ 0x00C16950)
// Leaf: ?Assign@CInstanceManager@@QAEXMPBUSThemeRequest@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D5E0, oracle length 365 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; mov eax,[esp+14h] ; push ebx ; push esi ; push edi ; push eax ; mov esi,ecx ; call CInstanceManager::FadeOutAll ; xor edi,edi ; lea ebx,[esi+18h] ; scan: mov ecx,ebx ; call CInstance::IsIdle ; test al,al ; jne found ; inc edi ; add ebx,180h ; cmp edi,2 ; jb scan ; mov ecx,esi ; call CInstanceManager::FindQuietestInstance ; mov edi,eax ; test edi,edi ; jne have ; mov edx,[_afxPchNil] ; mov [esp+0Ch],edx ; mov edx,offset ".\CInstanceManager.cpp" ; lea ecx,[esp+0Ch] ; mov [esp+18h],eax ; call operator<< ; ... mov edx,5Ah ; ... call operator<<(long) ; ... ("Cannot be!") ; ... ("\n") ; mov eax,[g_pfnAssertHook] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],-1 ; call ~CString ; lea ecx,[esp+20h] ; call CDebugBreak::CDebugBreak ; have: push 0 ; push 0 ; lea ecx,[edi+138h] ; call CRampedParam::RampTo ; mov edi,[esp+24h] ; mov byte ptr [esi+33Ch],1 ; mov eax,[edi] ; lea ecx,[edi+4] ; push ecx ; lea ecx,[esi+324h] ; mov [esi+320h],eax ; call CName::operator= ; ... ret 8 ; found: mov ecx,[esp+24h] ; push ecx ; push edi ; mov ecx,esi ; call CInstanceManager::StartInstance ; mov ecx,[esp+10h] ; mov [esi+318h],edi ; ... ret 8
// Naming (sweep1, 2026-10-25): the catalog row was still `CEditTransactionSetEngineTheme::Apply` (propagated) and the
//   landed source used address-named classes (`CInstanceManager_00c2d5e0::Assign`); the class is libpk's
//   CInstanceManager, from the file string `.\CInstanceManager.cpp` this body reports with, and the landed chunk-436
//   rows of the same class. FINDINGS chunk 436 callee fixes: 0x00C2D210 (landed source "Register(request)") is
//   CInstanceManager::FadeOutAll(float time), so the first argument is the fade time; 0x00C2D520 ("Create") is
//   CInstanceManager::FindQuietestInstance; 0x00C38840 is CInstance::IsIdle; 0x00C3A950 is CRampedParam::RampTo
//   (SetTarget(target, 0, time)) on the instance's +0x138 volume, as the landed FindQuietestInstance models them.
//   Descriptive (no PDB covers libpk); StartInstance 0x00C2D260 (not landed: starts instance[index] from the request's
//   fields through 0x00C38CD0) is a descriptive name.
// Shape: the error report is libpk's macro: a CString over the nil buffer in a nested block (EH state 0 covers it)
//   streamed with the `__fastcall` CString operator<< family, handed to the assert hook, destroyed, then the empty
//   CDebugBreak temporary in the dead argument slot.
extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct CName {                                  // 8 bytes with its own assignment
    unsigned char _body[8];
    CName& operator=(const CName& other);       // 0x00C16950
};

struct SThemeRequest {                          // 0x1c bytes
    long m_0;
    CName m_name;                               // +4
    long m_c;
    long m_10;
    long m_14;
    long m_18;
};

struct CRampedParam { void RampTo(float target, float time); };   // 0x00C3A950
struct CInstance {                              // 0x180 bytes
    unsigned char _pad[0x138];
    CRampedParam  m_volume;                     // +0x138
    unsigned char _pad_0x139[0x47];
    bool IsIdle();                              // 0x00C38840
};

struct CInstanceManager {
    unsigned char _pad[0x18];
    CInstance m_instances[2];                   // +0x18
    long m_current;                             // +0x318
    long m_31c;
    SThemeRequest m_pending;                    // +0x320
    unsigned char m_bPending;                   // +0x33c
    void FadeOutAll(float time);                // 0x00C2D210
    CInstance* FindQuietestInstance();          // 0x00C2D520
    void StartInstance(long index, const SThemeRequest* request);   // 0x00C2D260
    void Assign(float fadeTime, const SThemeRequest* request);
};

void CInstanceManager::Assign(float fadeTime, const SThemeRequest* request)
{
    FadeOutAll(fadeTime);
    for (unsigned long index = 0; index < 2; ++index) {
        if (m_instances[index].IsIdle()) {
            StartInstance(index, request);
            m_current = index;
            return;
        }
    }
    CInstance* instance = FindQuietestInstance();
    if (!instance) {
        {
            CString line;
            line << ".\\CInstanceManager.cpp" << "(" << (long)90 << ") : " << "Cannot be!" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    instance->m_volume.RampTo(0, 0);
    m_bPending = 1;
    m_pending = *request;
}
