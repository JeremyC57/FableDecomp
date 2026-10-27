// std::vector<CHeroSoulsRoundDef>::resize(size_type n) -- STLport: resize(n, CHeroSoulsRoundDef())
// Leaf: ?resize@?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f353f0, 61 bytes, thiscall, `ret 4`.
// Identity (probable): its only caller is the landed 0x00F351F0 (TransferVectorOfSubComponents<CHeroSoulsRoundDef>), which persists a vector<CHeroSoulsRoundDef>; the default element it builds is a 0x38-byte def record (Ego_r.pdb CHeroSoulsRoundDef: CDefClassBase, NumWaves, vector<CHeroSoulsWaveDef> Waves) whose member vector sits at +0x2C (destroyed through 0x00F26E20).
// Callees: CHeroSoulsRoundDef::CHeroSoulsRoundDef() 0x00F35430, vector<CHeroSoulsRoundDef>::resize(size_type, const CHeroSoulsRoundDef&) 0x00F35450, vector<CHeroSoulsWaveDef>::~vector 0x00F26E20,
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
class CHeroSoulsWaveDef;
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
class CHeroSoulsRoundDef : public CDefClassBase {                 // retail 0x38 bytes
public:
    long  NumWaves;                                   // +0x28
    std::vector<CHeroSoulsWaveDef> Waves;                     // +0x2c
    CHeroSoulsRoundDef();                                        // 0x00F35430
};
template void std::vector<CHeroSoulsRoundDef>::resize(unsigned int);