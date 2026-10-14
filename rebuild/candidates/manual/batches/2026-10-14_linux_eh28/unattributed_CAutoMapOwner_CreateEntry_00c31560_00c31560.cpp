// CAutoMapOwner_00c31560::CreateEntry(key) -- EH-framed factory (the manifest's "CEngineWaterBackgroundSubPatch_Constructor" is propagated): a 48-byte entry is allocated (null-checked `new`, state 0 frees it if the constructor @ 0x00C3BAA0 throws) from the key read through its pointer; a failed allocation raises the "EMEM" warning (PKCAutoMap.h line 40: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot; state 1); the entry is registered through the owner's slot-9 virtual and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31560, oracle length 249 bytes. Built under /EHsc.
// Retail bytes:  ... push 30h ; mov edi,ecx ; call operator new ; add esp,4 ; mov [esp+8],eax ; test eax,eax ; mov dword ptr [esp+14h],0 ; je null ; mov ecx,[esp+1Ch] ; mov edx,[ecx] ; push edx ; mov ecx,eax ; call CEntry::CEntry ; mov esi,eax ; jmp got ; null: xor esi,esi ; got: test esi,esi ; mov dword ptr [esp+14h],-1 ; jne reg ; mov edx,[g_pchNil] ; mov [esp+8],edx ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoMap.h" ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],1 ; call Append ; ... mov edx,28h ; ... call AppendNumber ; ... call Append ("EMEM") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CString ; lea ecx,[esp+1Ch] ; call CTrace::CTrace ; reg: mov eax,[edi] ; push esi ; mov ecx,edi ; call [eax+24h] ; ... mov eax,esi ; ... ret 4
// The classes are named by address.
void* __cdecl operator new(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c31560)(const char* text);
extern PFNLOGSINK_00c31560 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c31560 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c31560() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c31560();                       // 0x00C0BA40
    CString_00c31560* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c31560* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c31560 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c31560();                         // 0x00C16750
};

struct CEntry_00c31560 {                   // 0x30 bytes
    unsigned char _body[0x30];
    CEntry_00c31560(void* key);            // 0x00C3BAA0
};

struct CAutoMapOwner_00c31560 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Register(CEntry_00c31560* entry);   // slot 9
    CEntry_00c31560* CreateEntry(void* const* key);
};

CEntry_00c31560* CAutoMapOwner_00c31560::CreateEntry(void* const* key)
{
    CEntry_00c31560* entry = new CEntry_00c31560(*key);
    if (!entry) {
        {
            CString_00c31560 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoMap.h")->Append("(")->AppendNumber(40)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c31560();
    }
    Register(entry);
    return entry;
}
