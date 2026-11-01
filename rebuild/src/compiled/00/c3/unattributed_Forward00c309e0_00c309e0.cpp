// CClass_00c309e0::~CClass_00c309e0() -- destroy the CString member at +4 (the record's only non-trivial member)
// Leaf: ??1CClass_00c309e0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C309E0, 8 bytes: `add ecx,4 ; jmp 0xc0ba40`.
// Naming (naming wave 24, the 00c0dec0 CString views): 0x00C0BA40 is the libpk CString destructor ??1CString@@QAE@XZ (DECISIONS_W21; the
//   landed own row), and the only caller is the scalar deleting destructor 0x00C30A90, so this is the destructor of a record holding a
//   CString at +4 (its class is not identified: libpk has no PDB / RTTI; CClass_00c309e0 is descriptive). Was a tail-forward
//   Forward_00c309e0 to a stand-in CObject_00c0ba40::Method_00c0ba40.
class CString {
public:
    char* m_pchData;
    ~CString();                     // 0x00C0BA40
};

class CClass_00c309e0 {
public:
    unsigned char _pad[0x4];
    CString m_member;               // +0x04
    ~CClass_00c309e0();
};

CClass_00c309e0::~CClass_00c309e0()
{
}