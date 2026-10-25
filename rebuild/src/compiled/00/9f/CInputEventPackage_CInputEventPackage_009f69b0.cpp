// CInputEventPackage::CInputEventPackage(const CInputEventPackage&) -- memberwise copy construction of a recorded input
//   package: the Events vector (its out-of-line copy constructor), TimeStamp, the 16-byte Checksum, MousePos, JoystickPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F69B0, length 79 bytes, `ret 4`, returns this.
// Identity: Ego_r.pdb CInputEventPackage = {vector<CInputEvent> Events, long TimeStamp, CChecksum Checksum (16 bytes),
//   C2DVector MousePos, C2DVector JoystickPos} -- with the retail 12-byte vector exactly the +0 / +0xC / +0x10 / +0x20 /
//   +0x28 copies here (0x30 bytes; the PDB's 52 has the 16-byte Dinkumware vector). The callee 0x009F66A0 is the
//   vector<CInputEvent> COPY CONSTRUCTOR (zeroes the three words, allocates n * 0x34, copy-constructs each CInputEvent
//   through 0x009E4850), not an operator=, and every caller is a placement construction: vector<CInputEventPackage>::
//   push_back 0x009F6FE0 (`if (finish) T(finish, x) ; finish += 0x30`), the insert paths 0x009F6AC0 / 0x009F6ED0; Ego_r.pdb
//   instantiates _Construct<CInputEventPackage, const CInputEventPackage&>. So this is the copy constructor, not the
//   catalog's "CDefWithRecords_009f69b0::operator=" or the manifest's CAugmentationDef::operator=. FINDINGS chunk 414.
// Callees: vector<CInputEvent>::vector(const vector&) 0x009F66A0.
// Shape: the 16-byte Checksum is a plain four-dword POD member: its compiler-generated copy is the `lea ecx,[edi+10h] ;
//   lea edx,[esi+10h]` pair with four dword moves; the two C2DVectors (user memberwise copy constructor) copy as straight
//   dword moves.
// Leaf: ??0CInputEventPackage@@QAE@ABV0@@Z
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    vector(const vector& __x);                  // 0x009F66A0 for vector<CInputEvent>
};
}

class CInputEvent { public: unsigned char _body[0x34]; };   // retail 0x34-byte element

struct CChecksum {                              // 16-byte POD
    unsigned long Word0, Word1, Word2, Word3;
};

class C2DVector {
public:
    float X;
    float Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

class CInputEventPackage {
public:
    std::vector<CInputEvent> Events;            // +0x00
    long                     TimeStamp;         // +0x0C
    CChecksum                Checksum;          // +0x10
    C2DVector                MousePos;          // +0x20
    C2DVector                JoystickPos;       // +0x28
    CInputEventPackage(const CInputEventPackage& other);
};

CInputEventPackage::CInputEventPackage(const CInputEventPackage& other)
    : Events(other.Events), TimeStamp(other.TimeStamp), Checksum(other.Checksum), MousePos(other.MousePos),
      JoystickPos(other.JoystickPos)
{
}
