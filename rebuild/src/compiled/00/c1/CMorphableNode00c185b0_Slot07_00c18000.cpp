// CMorphableNode_00c185b0::Slot07(long a, long b) -- libpk PKCTRMorphable.cpp forwarder: log "Error: NULL implimentation" (line 92) when there is no implementation, then forward to its slot 7
// Leaf: ?Slot07@CMorphableNode_00c185b0@@UAEXJJ@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c18000, 206 bytes, thiscall, `ret 8`, EH-framed (handler 0x01211F85).
// Identity (descriptive): slot 7 of vtable 0x012B2DEC (entry 0x012B2E08), the vtable the landed constructor CMorphableNode_00c185b0 0x00C185B0 stores (PKCTRMorphable.cpp, libpk: no PDB, no RTTI); it logs line 92 of .\PKCTRMorphable.cpp and calls slot 7 (+0x1C) of the +4 implementation with both arguments; class and implementation keep the landed constructor's descriptive names, the method is named by its slot.
// Callees: the log line's chained `__fastcall` appenders 0x00C0DEC0 (text) / 0x00C0DF00 (number), its destructor 0x00C0BA40,
//   the trace temporary 0x00C16750; globals: the shared buffer 0x0139C900, the sink 0x0143BB2C (spellings of the landed
//   twin CAutoDeletePointer::GetChecked 0x00C09920 / PKCAutoFile::GetChecked 0x00C31DB0).
// Shape (family L121): the twin's log block (state 0 -> -1, the trace temporary on a dead frame byte), then the
//   implementation's virtual called with the two arguments.
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct CNodeBase_00c185b0 {                                // the implementation (tree or flat node)
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07(long a, long b);                // slot 7 (+0x1C)
};

struct CMorphableNode_00c185b0 {                           // vtable 0x012B2DEC, 8 bytes
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void Slot04();
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07(long a, long b);                // slot 7 (vtable entry 0x012B2E08)
    CNodeBase_00c185b0* m_pNode;                           // +4
};

void CMorphableNode_00c185b0::Slot07(long a, long b)
{
    if (m_pNode == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCTRMorphable.cpp" << "(" << 92 << ") : " << "Error: NULL implimentation" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    m_pNode->Slot07(a, b);
}