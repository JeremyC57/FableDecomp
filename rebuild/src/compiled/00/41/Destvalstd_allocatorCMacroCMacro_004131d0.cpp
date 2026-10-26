// Retail: Fable.exe (TLC, VC7.1) @ 0x004131D0, 35 bytes. Catalog name: std::_Dest_val<std::allocator<CMacro>,CMacro>
// Leaf: allocator<CMacro>,CMacro>
// (propagated: the PDB CMacro is {CCharString, CCharString, CArray<CCharString>, ...}, which this body is not).
//
// Body: the reverse-order destruction of a 16-byte record of four one-word string members:
//   +0xc CWideString (~CWideString @ 0x0099B510), +0x8 CCharString (~CCharString @ 0x0099EAE0),
//   +0x4 CWideString, then +0x0 CWideString as the tail jump (mov ecx,esi ; pop esi ; jmp 0x0099B510).
// 0x0099B510 is the wide-string release (NoRefs at data+0xc, free(), instance count 0x013BCA20; the
// CDiskFileWin32 Pathname is destroyed with it in GFGetBuildNumber2); 0x0099EAE0 is ~CCharString
// (release 0x0099E9B0 with NoRefs at data+0xd as in the PDB CCharStringData, instance count 0x013BD800).
// __fastcall, this in ecx.

class CWideString { public:
    void* PStringData;
    ~CWideString();                 // 0x0099B510
};
class CCharString { public:
    void* PStringData;
    ~CCharString();                 // 0x0099EAE0
};

struct CStringRecord_004131d0 {
    CWideString m_0;                // +0x00  (destroyed last, as the tail jump)
    CWideString m_4;                // +0x04
    CCharString m_8;                // +0x08
    CWideString m_c;                // +0x0c  (destroyed first)
};

void __fastcall Dest_val(CStringRecord_004131d0* self)
{
    self->m_c.~CWideString();
    self->m_8.~CCharString();
    self->m_4.~CWideString();
    self->m_0.~CWideString();
}
