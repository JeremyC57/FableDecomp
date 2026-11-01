// CFileDataOutputStream::CFileDataOutputStream(CAFile& file) -- the CDataOutputStream base (CBase, its vtable, the "\r\n" line ending at +4), the file at +8, this class's vtable
// Leaf: ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z
// Naming wave 26 (c693 queue): Ego_r.pdb + FableWin.pdb ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z -- the file by reference (the same
//   pointer push); the member is the PDB's File (engine/CFileDataOutputStream.h +0x08). Was CFileDataOutputStream(void*) on m_pFile.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994400, length 45 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the literal @ 0x012397F8, -1). Vtables 0x0129A6E0 = CDataOutputStream,
// 0x0129A6F8 = CFileDataOutputStream (rtti-vtables.tsv). Shape: the base's vtable store is scheduled between the two pushes and the string constructor, the
// derived vtable after the file store; `this` is returned, `ret 4`.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
};

class CDataOutputStream : public CBaseClassNonCopyable { public:                 // vtable 0x0129A6E0
    CCharString m_lineEnding;                      // +0x04
    CDataOutputStream() : CBaseClassNonCopyable(), m_lineEnding("\r\n", -1) { }
};

class CAFile;
class CFileDataOutputStream : public CDataOutputStream { public:   // vtable 0x0129A6F8
    CAFile* File;                                  // +0x08
    CFileDataOutputStream(CAFile& file);
};

CFileDataOutputStream::CFileDataOutputStream(CAFile& file)
    : CDataOutputStream()
    , File(&file)
{
}