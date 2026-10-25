// SWaveEntry::SWaveEntry(void* key) -- constructs a 0x20-byte wave-map entry for a key: the key at +0, an empty libpk CString name at +4 (the shared empty data 0x0139C900), the size, offset and the five qualifier fields zeroed
// Leaf: ??0SWaveEntry@@QAE@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B900, 45 bytes, __thiscall `ret 4`, returns `this`:
//   mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; mov edx,[_afxPchNil] ; xor ecx,ecx ; mov [eax+4],edx ;
//   mov [eax+8],ecx ; mov [eax+0Ch],ecx ; mov [eax+10h],cx ; mov [eax+12h],cx ; mov [eax+14h],ecx ;
//   mov [eax+18h],ecx ; mov [eax+1Ch],ecx ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CLandscapeLayerMesh::CLandscapeLayerMesh (a propagated manifest label);
//   FINDINGS chunk 437 ("0xC3B900 = 0x20-byte wave-entry ctor (PKCAutoMap)"). Its only caller is the landed map
//   factory CreateEntry 0x00C312C0 (call 0x00C312FD after `operator new(0x20)`, failure trace
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoMap.h(40) : EMEM"), whose entries the landed CWaveDataReader::OnRecord
//   0x00C31230 fills with this layout (the name through CString::operator= 0x00C0C200, size, offset + base, the
//   qualifiers). Descriptive names (no PDB covers libpk); SWaveEntry and its field names are that row's.
extern char* _afxPchNil;                        // 0x0139C900

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
};

struct SWaveEntry {                             // 0x20 bytes
    void* m_key;                                // +0x00
    CString m_name;                             // +0x04
    unsigned long m_size;                       // +0x08
    unsigned long m_offset;                     // +0x0C
    unsigned short m_field10;                   // +0x10
    unsigned short m_field12;                   // +0x12
    unsigned long m_field14;                    // +0x14
    unsigned long m_field18;                    // +0x18
    unsigned long m_field1C;                    // +0x1C
    SWaveEntry(void* key);
};

SWaveEntry::SWaveEntry(void* key)
    : m_key(key), m_name(), m_size(0), m_offset(0), m_field10(0), m_field12(0), m_field14(0), m_field18(0), m_field1C(0)
{
}
