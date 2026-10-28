// CMemoryDataOutputStream::SetPosition(unsigned long position) -- move the write position within the written data; a position past the end throws CBBBFileException(SeekOutOfRange)
// Grade: DIFFER(46v45) -- int3-after-noreturn: byte-exact through the _CxxThrowException call; VC7.1 emits the int3 after the noreturn call that the cutter stripped from the 45-byte row (the 2026-10-14 hand57 park, same residue). Land against the 46-byte span.
// Leaf: ?SetPosition@CMemoryDataOutputStream@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994B10, 45 bytes, thiscall, `ret 4`; the body ends in the noreturn
//   _CxxThrowException call (the int3 VC7.1 emits after it is the row's padding: int3-after-noreturn).
// Identity (proven): CMemoryDataOutputStream vtable slot 1 (RTTI .?AVCMemoryDataOutputStream@@; CDataOutputStream slots 0 dtor,
//   1 SetPosition, 2 GetPosition, 3 Close, 4 Write), Ego_r.pdb ?SetPosition@CMemoryDataOutputStream@@UAEXK@Z.
// Callees: _CxxThrowException 0x00BFEB84 with the CBBBFileException ThrowInfo 0x01369380 (catchable types .?AVCBBBFileException@@,
//   .?AVCExceptionBase@@; 4-byte object: the type code only).
// Shape: `if (position <= Data.size()) { CurrentPosition = position; return; } throw CBBBFileException(SeekOutOfRange);` (the
//   guard-throw-first order puts the throw on the fall-through) -- the 4-byte throw
//   object is built in the dead `position` slot; FableWin.pdb's ??0CBBBFileException@@QAE@W4EFileExceptionType@0@@Z is the
//   type-only constructor (inline). No EH frame: the module is built without /EHsc.
// Layout: CDataOutputStream (vptr +0, NewLine +4), Data CArray<char> +0x08 (12 bytes in retail), CurrentPosition +0x14
//   (Ego_r +0x18 minus 4 for the STLport CArray). Local model: engine/CMemoryDataOutputStream.h is the flat Ego_r layout.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
class CExceptionBase {
public:
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullParameter = 1, NotOpenForWriting = 2, ReadPastEnd = 3, FileNotFound = 10, SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(EFileExceptionType type) : Type(type) { }
};

template <class T> class CArray {                        // STLport vector<T>: start, finish, end of storage (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};

class CDataOutputStream {
public:
    virtual ~CDataOutputStream();                       // slot 0
    virtual void SetPosition(unsigned long position) = 0;   // slot 1
    CCharString NewLine;                                // +0x04
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    CArray<char>  Data;                                 // +0x08
    unsigned long CurrentPosition;                      // +0x14
    virtual void SetPosition(unsigned long position);
};

void CMemoryDataOutputStream::SetPosition(unsigned long position)
{
    if (position <= Data.size()) {
        CurrentPosition = position;
        return;
    }
    throw CBBBFileException(CBBBFileException::SeekOutOfRange);
}
