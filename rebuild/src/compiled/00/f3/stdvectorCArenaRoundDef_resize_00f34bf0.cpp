// std::vector<CArenaRoundDef>::resize(size_type n) -- STLport: resize(n, CArenaRoundDef())
// Leaf: ?resize@?$vector@VCArenaRoundDef@@V?$allocator@VCArenaRoundDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f34bf0, 61 bytes, thiscall, `ret 4`.
// Identity (probable): its only caller is the landed 0x00F349F0 (TransferVectorOfSubComponents<CArenaRoundDef>), which persists a vector<CArenaRoundDef>; the default element it builds is a 0x38-byte def record (Ego_r.pdb CArenaRoundDef: CDefClassBase, NumWaves, vector<CArenaWaveDef> Waves) whose member vector sits at +0x2C (destroyed through 0x00F25B70).
// Callees: CArenaRoundDef::CArenaRoundDef() 0x00F34C30, vector<CArenaRoundDef>::resize(size_type, const CArenaRoundDef&) 0x00F34C50, vector<CArenaWaveDef>::~vector 0x00F25B70,
//   CResource::~CResource 0x009FC550 (after the inline CDefPointeeBase vtable store 0x01230BA0).
// Shape (family L148): the temporary's implicit destructor is inline: the member vector's destructor out of line, the
//   dead derived vtable stores dropped, CDefPointeeBase's kept before the out-of-line CResource destructor.
class CResource {                                   // RTTI base of CDefPointeeBase
public:
    virtual ~CResource();                            // 0x009FC550
    unsigned char _res[0x18];
};
class CDefPointeeBase : public CResource {           // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
class CDefClassBase : public CDefPointeeBase {       // Ego_r.pdb 0x25 bytes
public:
    CDefinitionManager* PDefManager;                 // +0x1c
    unsigned long       GlobalIndex;                 // +0x20
    unsigned char       Flags;                       // +0x24 (Setup / Template / DefaultValsApplied bits)
};
class CArenaWaveDef;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    ~vector();                                        // out of line
    void resize(size_type __new_size, const _Tp& __x);   // out of line
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}
class CArenaRoundDef : public CDefClassBase {                 // retail 0x38 bytes
public:
    long  NumWaves;                                   // +0x28
    std::vector<CArenaWaveDef> Waves;                     // +0x2c
    CArenaRoundDef();                                        // 0x00F34C30
};
template void std::vector<CArenaRoundDef>::resize(unsigned int);