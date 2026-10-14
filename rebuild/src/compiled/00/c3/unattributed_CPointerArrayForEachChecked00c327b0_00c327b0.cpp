// CPointerArray_00c327b0::ForEachChecked(visitor) -- EH-framed walk over the pointer array at +4 (unsigned count at +0xc): a null slot raises the "Error: Null item" warning (PKCTRArray.cpp line 220: an MFC-style string appended in a nested block and handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 in the dead argument slot); every slot's address is then handed to the visitor's slot-0 virtual; the slot offset (i * 4) is kept in edi across the warning
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C327B0, oracle length 247 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,ecx ; mov eax,[esi+0Ch] ; xor ebx,ebx ; test eax,eax ; jbe done ; push ebp ; mov ebp,[esp+20h] ; push edi ; loop: mov eax,[esi+4] ; lea edi,[ebx*4] ; cmp dword ptr [edi+eax],0 ; jne visit ; mov ecx,[g_pchNil] ; mov [esp+10h],ecx ; mov edx,offset ".\PKCTRArray.cpp" ; lea ecx,[esp+10h] ; mov dword ptr [esp+1Ch],0 ; call Append ; ... mov edx,0DCh ; ... call AppendNumber ; ... call Append ("Error: Null item") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+10h] ; call eax ; skip: lea ecx,[esp+10h] ; mov dword ptr [esp+1Ch],-1 ; call ~CString ; lea ecx,[esp+24h] ; call CTrace::CTrace ; visit: mov eax,[esi+4] ; mov edx,[ebp] ; add eax,edi ; push eax ; mov ecx,ebp ; call [edx] ; mov eax,[esi+0Ch] ; inc ebx ; cmp ebx,eax ; jb loop ; pop edi ; pop ebp ; done: ... ret 4
// The sibling checked getter landed at 0x00C324F0 (same class, same warning text at line 190). The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c327b0)(const char* text);
extern PFNLOGSINK_00c327b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c327b0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c327b0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c327b0();                       // 0x00C0BA40
    CString_00c327b0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c327b0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c327b0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c327b0();                         // 0x00C16750
};

struct IVisitor_00c327b0 {
    virtual void Visit(void** slot);       // slot 0
};

struct CPointerArray_00c327b0 {
    long m_0;
    void** m_pData;                        // +4
    long m_8;
    unsigned long m_count;                 // +0xc
    void ForEachChecked(IVisitor_00c327b0* visitor);
};

void CPointerArray_00c327b0::ForEachChecked(IVisitor_00c327b0* visitor)
{
    for (unsigned long i = 0; i < m_count; ++i) {
        if (m_pData[i] == 0) {
            {
                CString_00c327b0 line;
                line.Append(".\\PKCTRArray.cpp")->Append("(")->AppendNumber(220)->Append(") : ")->Append("Error: Null item")->Append("\n");
                if (g_pfnLogSink_0143bb2c)
                    g_pfnLogSink_0143bb2c(line.m_pchData);
            }
            CTrace_00c327b0();
        }
        visitor->Visit(&m_pData[i]);
    }
}