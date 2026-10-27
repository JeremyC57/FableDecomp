// CMorphableNode_00c185b0::Slot00() -- libpk PKCTRMorphable.cpp forwarder: log "Error: NULL implimentation" (line 26) when there is no implementation, then forward to its slot 0
// Leaf: ?Slot00@CMorphableNode_00c185b0@@UAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C186D0, 193 bytes, thiscall, plain `ret`, EH-framed (handler 0x01212033).
// Identity (descriptive): slot 0 of vtable 0x012B2DEC (its only reference), the vtable the landed constructor
//   CMorphableNode_00c185b0 0x00C185B0 stores (PKCTRMorphable.cpp, libpk: no PDB, no RTTI); it logs line 26 of
//   .\PKCTRMorphable.cpp and calls slot 0 of the +4 implementation (no arguments). Same family as the landed Slot01
//   0x00C187A0 (line 32, slot 1); class and method keep that row's descriptive names. Catalog "sub_00c186d0".
// Callees: the log line's chained `__fastcall` appenders 0x00C0DEC0 (text) / 0x00C0DF00 (number), its destructor 0x00C0BA40,
//   the trace temporary 0x00C16750; globals: the shared buffer 0x0139C900, the sink 0x0143BB2C.
extern char* _afxPchNil;                                  // 0x0139C900 (the empty string)
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;                      // 0x0143BB2C

class CString {                                            // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                            // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);      // 0x00C0DEC0
    CString& __fastcall operator<<(long value);            // 0x00C0DF00
};

struct CDebugBreak {                                       // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                                         // 0x00C16750
};

struct CNodeBase_00c185b0 {                                // the implementation (tree or flat node)
    virtual void Slot00();
    virtual void Slot01();                // slot 1 (+0x04)
};

struct CMorphableNode_00c185b0 {                           // vtable 0x012B2DEC, 8 bytes
    virtual void Slot00();
    virtual void Slot01();                // slot 1 (vtable entry 0x012B2DF0)
    CNodeBase_00c185b0* m_pNode;                           // +4
};

void CMorphableNode_00c185b0::Slot00()
{
    if (m_pNode == 0) {
        {
            CString s;
            s << ".\\PKCTRMorphable.cpp" << "(" << 26 << ") : " << "Error: NULL implimentation" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(s.m_pchData);
        }
        CDebugBreak();
    }
    m_pNode->Slot00();
}