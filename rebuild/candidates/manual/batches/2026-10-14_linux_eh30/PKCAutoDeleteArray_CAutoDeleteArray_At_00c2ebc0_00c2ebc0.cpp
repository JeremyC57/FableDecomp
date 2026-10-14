// CAutoDeleteArray_00c2ebc0::At(index) -- EH-framed checked element accessor over an owned 4-byte element array (the manifest's "PKCAutoDeleteArray_RangeCheck_Abort"): an index not below the count at +4 raises the "Index <i> is out of range (<n>)" warning (PKCAutoDeleteArray.h line 77: an MFC-style string in a nested block, both numbers through the unsigned AppendNumber @ 0x00C0DF10, handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot; state 0 covers the string); the address of the element is returned either way
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EBC0, oracle length 241 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx+4] ; push edi ; mov edi,[esp+20h] ; cmp edi,esi ; jb done ; mov eax,[g_pchNil] ; mov [esp+0Ch],eax ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoDeleteArray.h" ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],0 ; call Append ; ... "(" ... mov edx,4Dh ; ... call AppendNumber ; ... ") : " ... "Index " ... mov edx,edi ; ... call AppendNumber(unsigned) ; ... " is out of range (" ... mov edx,esi ; ... call AppendNumber(unsigned) ; ... ")" ... "\n" ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov dword ptr [esp+18h],-1 ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; done: mov ecx,[ebx] ; lea eax,[ecx+edi*4] ; ... ret 4
// The twin at 0x00C16B10 indexes an unsigned short array. The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c2ebc0)(const char* text);
extern PFNLOGSINK_00c2ebc0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c2ebc0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c2ebc0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c2ebc0();                       // 0x00C0BA40
    CString_00c2ebc0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c2ebc0* __fastcall AppendNumber(long value);   // 0x00C0DF00
    CString_00c2ebc0* __fastcall AppendNumber(unsigned long value);   // 0x00C0DF10
};

struct CTrace_00c2ebc0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c2ebc0();                         // 0x00C16750
};

struct CAutoDeleteArray_00c2ebc0 {
    long* m_pData;               // +0
    unsigned long m_count;                 // +4
    long* At(unsigned long index);
};

long* CAutoDeleteArray_00c2ebc0::At(unsigned long index)
{
    if (!(index < m_count)) {
        {
            CString_00c2ebc0 line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDeleteArray.h")->Append("(")->AppendNumber(77L)->Append(") : ")->Append("Index ")->AppendNumber(index)->Append(" is out of range (")->AppendNumber(m_count)->Append(")")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c2ebc0();
    }
    return &m_pData[index];
}
