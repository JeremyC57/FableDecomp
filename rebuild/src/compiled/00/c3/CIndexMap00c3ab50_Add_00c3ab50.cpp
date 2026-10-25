// CIndexMap_00c3ab50::Add(CMessageLog_00c3ab50* log, void* value, unsigned long index) -- register a value under a numeric index: when the index is already present (slot-17 lookup) append "Index <n> already exists!" to a fresh line of the load log, otherwise insert a new {value, index} node (slot 9; a failed allocation inserts null)
// Leaf: ?Add@CIndexMap_00c3ab50@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3ab50, 122 bytes (+ int3 padding), __fastcall `ret 0Ch`.
// Library: libpk audio bank loader (no PDB/RTTI); descriptive names. The map is a member of the bank reader (caller
//   0x00C2E469 passes the reader's log, a value and the record's index); its twin instantiation is 0x00C3B0E0.
//   The manifest's "Container_AddItem_Checked" is propagated.
// Callees: the map's own virtual slots 17 (+0x44, lookup by index address) and 9 (+0x24, insert); scalar operator new
//   0x00BFEA1A (8-byte node), the node constructor 0x00C3AB30 (value, index); the log's NewMessage 0x00C307E0 (landed as
//   "CScriptHandleRegistry::CreateNewInstance": a 12-byte node whose embedded MFC-style string starts empty, returned
//   by address); the library string's __fastcall stream operators 0x00C0DEC0 (text) and 0x00C0DF10 (unsigned number).
// Strings: "Index " @ 0x012B2B10, " already exists!" @ 0x012B54D4.
// Shape: the index is looked up through its argument slot (`lea ecx,[esp+10h]`); the message chain reuses each
//   returned stream pointer; `new` + constructor is null-checked and the result (or 0) goes straight to Insert.
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A

struct CDebugStream_00c3ab50 {                         // the library's MFC-style string, one word
    char* m_pchData;
    CDebugStream_00c3ab50& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream_00c3ab50& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CMessageLog_00c3ab50 {
    CDebugStream_00c3ab50* NewMessage();                    // 0x00C307E0
};

struct SIndexNode_00c3ab50 {                           // 8 bytes
    void* m_pValue;                                         // +0x00
    unsigned long m_index;                                  // +0x04
    SIndexNode_00c3ab50(void* value, unsigned long index);   // 0x00C3AB30
};

class CIndexMap_00c3ab50 {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Insert(SIndexNode_00c3ab50* node);                      // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual SIndexNode_00c3ab50* Lookup(const unsigned long* index);     // slot 17 (+0x44)
    void Add(CMessageLog_00c3ab50* log, void* value, unsigned long index);
};

void CIndexMap_00c3ab50::Add(CMessageLog_00c3ab50* log, void* value, unsigned long index)
{
    if (Lookup(&index)) {
        *log->NewMessage() << "Index " << index << " already exists!";
        return;
    }
    Insert(new SIndexNode_00c3ab50(value, index));
}