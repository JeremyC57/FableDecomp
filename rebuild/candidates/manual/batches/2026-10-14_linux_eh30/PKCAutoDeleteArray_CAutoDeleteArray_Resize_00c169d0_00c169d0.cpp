// CAutoDeleteArray_00c169d0::Resize(count) -- EH-framed resize of an owned unsigned short array (the manifest's "CDefStringWide_ReserveCapacity" is propagated): unchanged for the same count; a zero count frees the block (`operator delete`) and clears both words; otherwise a new block of count elements is obtained (`operator new`, no cookie), its absence raising the "EMEM" warning (PKCAutoDeleteArray.h line 57: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot; state 0 covers the string), the smaller of the old and new counts (`keep = count; if (m_count < count) keep = m_count;` -- the ternary and the Dinkumware `min` helper both schedule the compare differently) copied element by element when non-zero, the old block freed when present and the words replaced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C169D0, oracle length 316 bytes. Built under /EHsc.
// Retail bytes:  ... mov ebx,[esp+18h] ; push esi ; mov esi,ecx ; mov eax,[esi+4] ; cmp ebx,eax ; je done ; test ebx,ebx ; jne grow ; test eax,eax ; je a ; mov eax,[esi] ; push eax ; call operator delete ; add esp,4 ; mov [esi],ebx ; a: mov dword ptr [esi+4],0 ; ... ret 4 ; grow: lea ecx,[ebx+ebx] ; push edi ; push ecx ; call operator new ; mov edi,eax ; add esp,4 ; test edi,edi ; jne copy ; mov edx,[g_pchNil] ; mov [esp+0Ch],edx ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoDeleteArray.h" ; lea ecx,[esp+0Ch] ; mov [esp+18h],eax ; call Append ; ... mov edx,39h ; ... call AppendNumber ; ... call Append ("EMEM") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],-1 ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; copy: mov eax,[esi+4] ; cmp eax,ebx ; mov ecx,ebx ; jae b ; mov ecx,eax ; b: test ecx,ecx ; je c ; xor eax,eax ; test ecx,ecx ; jbe c ; loop: mov edx,[esi] ; MOVunsigned short ; inc eax ; cmp eax,ecx ; jb loop ; c: mov eax,[esi] ; test eax,eax ; je d ; push eax ; call operator delete ; add esp,4 ; d: mov [esi],edi ; mov [esi+4],ebx ; pop edi ; done: ... ret 4
// The twin at 0x00C31810 resizes a `malloc`/`free` array. The classes are named by address.
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c169d0)(const char* text);
extern PFNLOGSINK_00c169d0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c169d0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c169d0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c169d0();                       // 0x00C0BA40
    CString_00c169d0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c169d0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c169d0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c169d0();                         // 0x00C16750
};



struct CAutoDeleteArray_00c169d0 {
    unsigned short* m_pData;                         // +0
    unsigned long m_count;                 // +4
    void Resize(unsigned long count);
};

void CAutoDeleteArray_00c169d0::Resize(unsigned long count)
{
    if (count == m_count)
        return;
    if (count == 0) {
        if (m_count) {
            delete[] m_pData;
            m_pData = 0;
        }
        m_count = 0;
        return;
    }
    unsigned short* block = new unsigned short[count];
    if (!block) {
        {
            CString_00c169d0 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDeleteArray.h")->Append("(")->AppendNumber(57)->Append(") : ")->Append("EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c169d0();
    }
    unsigned long keep = count; if (m_count < count) keep = m_count;
    if (keep != 0) {
        for (unsigned long i = 0; i < keep; ++i)
            block[i] = m_pData[i];
    }
    if (m_pData)
        delete[] m_pData;
    m_pData = block;
    m_count = count;
}
