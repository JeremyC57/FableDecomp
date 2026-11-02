// CShaderDataBank::~CShaderDataBank() -- close the shader entries, then destroy StateBlock and the Shaders array and chain to ~CBankFile
// Leaf: ??1CShaderDataBank@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A620D0, 51 bytes, thiscall, tail `jmp ~CBankFile`.
// Identity (proven): it stores vtable 0x0129D644 (RTTI .?AVCShaderDataBank@@; the landed ctor 0x00A62020 and ??_G 0x00A62860
//   store the same one) and its callers are CShaderDataBank arrays of stride 0x128: CEngineResourceManager's destructor
//   0x00B3BAE4 / 0x00B3BB08 (the array at +0x2898 walked down 0x1B times) and 0x009C309C. FableWin.pdb spells
//   ??1CShaderDataBank@@UAE@XZ (donor 0x0336E950, after ??0 / ??_E and the CShaderBankStateBlock pair). The catalog label
//   "CParticleEffect::~CParticleEffect" is propagated.
// Callees: CShaderDataBank::CloseShaders 0x00A62050 (Ego_r ?CloseShaders@CShaderDataBank@@AAEXXZ: private; the landed
//   CShaderDataBank::Close 0x00A62110 calls it before CBankFile::Close), free 0x00BFEA14 (the Shaders vector's storage),
//   CBankFile::~CBankFile 0x009CE180.
// Shape: a user destructor `{ CloseShaders(); }` (the vtable store, then the call on the untouched ecx); then the members in
//   reverse order: StateBlock's implicit ~CShaderBankStateBlock drops its own store and keeps the inline ~CBankStateBlock's
//   (0x0129C82C, RTTI .?AVCBankStateBlock@@), the Shaders STLport vector of pointers frees its block under the null test;
//   then the base dtor.
// Layout: retail CBankFile is 0x110 bytes (STLport containers); engine/CBankFile.h is on an engine-header hold (flattened,
//   no vtable), so the base is modelled locally as polymorphic with its out-of-line destructor (header debt). Shaders
//   (CArray<CShaderBankEntry*>, 12 bytes) +0x110 and StateBlock (CShaderBankStateBlock, 12 bytes) +0x11C, as the landed
//   ctor 0x00A62020 builds them; the Ego_r-only ShaderPreParser / EntryParserStates / IntermediateFileFolder members are
//   absent from the retail class (sizeof 0x128 = the callers' stride). engine/CShaderDataBank.h carries those Ego_r members.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

class CBankFile {                                           // vtable 0x0129B54C, retail 0x110 bytes (local model: engine/CBankFile.h is on hold)
public:
    virtual ~CBankFile();                                   // 0x009CE180
    unsigned char _body[0x110 - 4];
};
class CBankStateBlock {                                     // vtable 0x0129C82C
public:
    virtual ~CBankStateBlock() { }
};
enum EShaderTarget { };                                     // Ego_r W4EShaderTarget@@; its values do not reach the bytes
class CShaderBankStateBlock : public CBankStateBlock {      // vtable 0x0129D630, 0xC bytes (Ego_r)
public:
    class CStateBlock {                                     // Ego_r CShaderBankStateBlock::CStateBlock, 8 bytes
    public:
        EShaderTarget ShaderTarget;                         // +0x00
        bool          EnableShaderOptimisation;             // +0x04
    };
    CStateBlock State;                                      // +0x04
};
class CShaderBankEntry;

class CShaderDataBank : public CBankFile {                  // vtable 0x0129D644, retail 0x128 bytes
public:
    CShaderDataBank();                                      // 0x00A62020
    virtual ~CShaderDataBank();
    CArray<CShaderBankEntry*> Shaders;                      // +0x110
    CShaderBankStateBlock     StateBlock;                   // +0x11c
private:
    void CloseShaders();                                    // 0x00A62050
};

CShaderDataBank::~CShaderDataBank()
{
    CloseShaders();
}