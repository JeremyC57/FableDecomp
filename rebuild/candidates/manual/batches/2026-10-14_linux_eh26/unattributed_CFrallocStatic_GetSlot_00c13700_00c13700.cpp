// CFrallocStatic_00c13700::GetSlot(index) -- EH-framed static-pool element access (the manifest's "TextureIndex_GetByProperties" is a propagated label): an index at or past the count at +4 raises the "Error: Fralloc overflow" warning (PKCFrallocStatic.h line 31: an MFC-style string appended in a nested block and handed to the log sink at 0x0143BB2C, followed by the empty trace temporary @ 0x00C16750 homed in the dead argument slot); the element of 0x2c bytes at +0x10 is returned regardless
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13700, oracle length 202 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,ecx ; mov eax,[esi+4] ; push edi ; mov edi,[esp+1Ch] ; cmp edi,eax ; jb ok ; mov eax,[g_pchNil] ; mov [esp+8],eax ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h" ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call Append ; mov edx,offset "(" ; mov ecx,eax ; call Append ; mov edx,1Fh ; mov ecx,eax ; call AppendNumber ; mov edx,offset ") : " ; ... call Append ; mov edx,offset "Error: Fralloc overflow" ; ... call Append ; mov edx,offset "\n" ; ... call Append ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CString ; lea ecx,[esp+1Ch] ; call CTrace::CTrace ; ok: mov ecx,[esi+10h] ; mov eax,edi ; imul eax,eax,2Ch ; add eax,ecx ; ... ret 4
// The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c13700)(const char* text);
extern PFNLOGSINK_00c13700 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c13700 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c13700() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c13700();                       // 0x00C0BA40
    CString_00c13700* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c13700* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c13700 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c13700();                         // 0x00C16750
};

struct CSlot_00c13700 {                    // 0x2c-byte pool element
    unsigned char _body[0x2c];
};

struct CFrallocStatic_00c13700 {
    long m_0;
    unsigned long m_count;                 // +4
    long m_8;
    long m_c;
    CSlot_00c13700* m_pSlots;              // +0x10
    CSlot_00c13700* GetSlot(unsigned long index);
};

CSlot_00c13700* CFrallocStatic_00c13700::GetSlot(unsigned long index)
{
    if (index >= m_count) {
        {
            CString_00c13700 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(31)->Append(") : ")->Append("Error: Fralloc overflow")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c13700();
    }
    return &m_pSlots[index];
}
