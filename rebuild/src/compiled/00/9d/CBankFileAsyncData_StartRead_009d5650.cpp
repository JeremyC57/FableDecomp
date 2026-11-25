// Leaf: ?StartRead@CBankFileAsyncData@@AAEXXZ
// CBankFileAsyncData::StartRead  retail 0x009d5650  __fastcall void(this)
// this->f18 = this->f28->m110->Read(&this->f2c);

struct Arg2c { int a, b, c, d; };

struct Reader {
    // __fastcall member; the call target 0x98f350
};

struct Inner {
    char pad[0x110];
    Reader* m110;        // +0x110 : object whose method is called
};

class CBankFileAsyncData { public:
    char    pad0[0x18];
    int     f18;         // +0x18  result store
    char    pad1c[0x28 - 0x1c];
    Inner*  f28;         // +0x28  pointer member
    Arg2c   f2c;         // +0x2c  address of this member is passed

    private:
    void StartRead();
    public:
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x0098F350 ?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z (was ?Read@Reader@@QAEHPAUArg2c@@@Z)
class CThreadedFileObjectDesc;
class CThreadedFile { public: unsigned long AddObject(CThreadedFileObjectDesc const&); };
void CBankFileAsyncData::StartRead()
{
    Reader* rd = this->f28->m110;
    this->f18 = reinterpret_cast<CThreadedFile*>(rd)->AddObject(*reinterpret_cast<CThreadedFileObjectDesc const*>(&this->f2c));
}