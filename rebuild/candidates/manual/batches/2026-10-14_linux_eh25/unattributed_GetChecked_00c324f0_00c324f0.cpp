// CPointerArray_00c324f0::GetChecked(unsigned long) -- EH-framed: 0 for an index at or beyond the count at +0xc; a null slot of the block at +4 logs `.\PKCTRArray.cpp(190) : Error: Null item` through a stack string (in its own block, state 0; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and lands an empty trace temporary in the dead argument slot (@ 0x00C16750); returns the slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C324F0, oracle length 229 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+0Ch] ; push edi ; mov edi,[esp+1Ch] ; cmp edi,eax ; jb inrange ; pop edi ; xor eax,eax ; ... ret 4 ; inrange: mov eax,[esi+4] ; cmp dword ptr [eax+edi*4],0 ; jne get ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call Append ; ... AppendNumber(0BEh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CString ; lea ecx,[esp+1Ch] ; call CTrace::CTrace ; get: mov edx,[esi+4] ; mov eax,[edx+edi*4] ; ... ret 4
// The out-of-range return keeps its own epilogue. The manifest label is a description; the
// class is named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c324f0)(const char* text);
extern PFNLOGSINK_00c324f0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c324f0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c324f0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c324f0();                       // 0x00C0BA40
    CString_00c324f0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c324f0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c324f0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c324f0();                         // 0x00C16750
};

struct CPointerArray_00c324f0 {
    long m_0;
    void** m_pData;                        // +4
    long m_8;
    unsigned long m_count;                 // +0xc
    void* GetChecked(unsigned long index);
};

void* CPointerArray_00c324f0::GetChecked(unsigned long index)
{
    if (index >= m_count)
        return 0;
    if (m_pData[index] == 0) {
        {
            CString_00c324f0 line;
            line.Append(".\\PKCTRArray.cpp")->Append("(")->AppendNumber(190)->Append(") : ")->Append("Error: Null item")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c324f0();
    }
    return m_pData[index];
}
