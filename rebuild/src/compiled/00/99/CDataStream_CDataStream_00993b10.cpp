// CDataStream::~CDataStream() -- puts back the CDataStream vtable and chains to the base destructor
// Leaf: ??1CDataStream@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00993B10, 11 bytes: mov [ecx],0x0129A69C ; jmp 0x0099A300.
// Name: a destructor's identity is the vtable it stores first: 0x0129A69C is the RTTI vtable of .?AVCDataStream@@
//   (slot 0 the scalar deleting destructor 0x00996EA0, slots 1-3 _purecall), and the tail call is
//   ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300; RTTI: CDataStream : CBaseClassNonCopyable : CBaseClass. Ego_r lists
//   ??1CDataStream@@UAE@XZ. The catalog's "CChunkedFileChunk::~CChunkedFileChunk" was wrong (CChunkedFileChunk derives
//   from CBaseClassNonCopyable directly and has its own vtable).
// Shape: an empty out-of-line virtual destructor; VC7.1 stores the class vtable and tail-calls the base destructor
//   (~CBaseClass is inline: its store is dead and dropped).
class CBaseClass {
public:
    virtual ~CBaseClass() { }
};

class CBaseClassNonCopyable : public CBaseClass {
protected:
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
};

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void Slot1() = 0;                             // slots 1-3 pure (names not in the PDB name sets)
    virtual void Slot2() = 0;
    virtual void Slot3() = 0;
};

CDataStream::~CDataStream()
{
}
