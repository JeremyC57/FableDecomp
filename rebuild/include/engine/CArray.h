#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// CArray<T> (Codex pass 11 consolidation, one model): Lionhead's array is a public STLport 4.6 vector<T> (12 bytes in retail;
// the donor PDB's later vector base is 16) adding no fields. Ego_r CArray<C3DVector> / CArray<long>: unsigned-int index
// operators returning T& and const T&, the (n) and (n, value) constructors, GetAsCArray over a private zero-size array.
#include "stlport/fable_containers.h"
#include "fable_compiler.h"
#ifdef FABLE_RETAIL_CARRAY_FOLD_ORIGINAL_AT_PASS27
#include "fable_assert.h"
#endif
#ifdef FABLE_RETAIL_CARRAY_INDEX_ASSERT
#include "fable_assert.h"
#endif
#if defined(FABLE_RETAIL_CARRAY_DEBUGGER_BOUNDS_PASS27) && defined(_MSC_VER) && !defined(FABLE_PORTABLE)
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#endif
#if defined(FABLE_RETAIL_CARRAY_RELEASE_INDEX_PASS29) && defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#include "fable_string.h"
#include "engine/CDebugManager.h"
extern CDebugManager* GPLibDebugManager;
CCharString FABLE_FASTCALL GFIntToCharString(long);
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#endif
#ifdef FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
// Both original TPI records:23 real methods, generated assignment omitted.
// Inverse LF_METHOD overload groups: measured VC71 reversal supports this
// reconstruction hypothesis; original textual source order is not proven.
template<class T> class CArray : public std::vector<T> {
public:
 CArray() : std::vector<T>() { }
 explicit CArray(unsigned int n) : std::vector<T>(n) { }
 CArray(unsigned int n,const T& t) : std::vector<T>(n,t) { }
 CArray(const CArray& a) : std::vector<T>(a) { }
 ~CArray() { }
#ifdef FABLE_RETAIL_CARRAY_FOLD_ORIGINAL_AT_PASS27
 typename std::vector<T>::reference At(unsigned int index) { 
#ifdef _DEBUG
        FABLE_RETAIL_ASSERT(index < this->size());
#endif
 return std::vector<T>::operator[](index); }
#else
 typename std::vector<T>::reference At(unsigned int index);
#endif
 typename std::vector<T>::const_reference At(unsigned int index) const;
#if defined(FABLE_RETAIL_CARRAY_DEBUGGER_BOUNDS_PASS27) && defined(_MSC_VER) && !defined(FABLE_PORTABLE)
 // Fidelity: FableWin 0322E450 and 0322C360 check index < size() and break only with a debugger.
 // The disabled release form is inferred from code generation in 00A4EFC0.
 typename std::vector<T>::reference operator[](unsigned int index) {
  if (0) { if (!(index < this->size())) { if (IsDebuggerPresent()) __debugbreak(); } }
  return std::vector<T>::operator[](index);
 }
 typename std::vector<T>::const_reference operator[](unsigned int index) const {
  if (0) { if (!(index < this->size())) { if (IsDebuggerPresent()) __debugbreak(); } }
  return const_cast<std::vector<T>*>(static_cast<const std::vector<T>*>(this))->operator[](index);
 }
#else
 typename std::vector<T>::reference operator[](unsigned int index) { return std::vector<T>::operator[](index); }
#ifdef FABLE_RETAIL_CARRAY_RELEASE_INDEX_PASS29
 // FableWin's const overload checks index<size() and forwards to the mutable vector subscript.
 // Inferred: retail disables the diagnostic and indexes through a signed iterator displacement.
 typename std::vector<T>::const_reference operator[](unsigned int index) const {
#if defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_CARRAY_INDEX_DIAGNOSTIC \
        (!GPLibDebugManager || GPLibDebugManager->DoErrorMessage( \
            CCharString("index<size()"), \
            CCharString("Illegal index ") + GFIntToCharString(index) + " in array of size " + GFIntToCharString(this->size()), \
            CWideString(L"e:\\fabletlc\\fable tlc build repository\\bbbprojects\\fable1_5mainpc\\bbblibrary\\lib_array.hpp"), 29, NDebugManager::ERROR_TYPE_CRITICAL) == true)
#ifdef GAME_COMP_DEBUG
    if (!(index < this->size()) && FABLE_CARRAY_INDEX_DIAGNOSTIC) {
        if (IsDebuggerPresent())
            __debugbreak();
    }
#else
    (void)(false && (!(index < this->size()) &&
        (FABLE_CARRAY_INDEX_DIAGNOSTIC && IsDebuggerPresent() && (__debugbreak(), true))));
#endif
#undef FABLE_CARRAY_INDEX_DIAGNOSTIC
#endif
#ifdef GAME_COMP_DEBUG
    return const_cast<std::vector<T>*>(static_cast<const std::vector<T>*>(this))->operator[](index);
#else
    return this->begin()[static_cast<ptrdiff_t>(index)];
#endif
 }
#else
 typename std::vector<T>::const_reference operator[](unsigned int index) const {
  return const_cast<std::vector<T>*>(static_cast<const std::vector<T>*>(this))->operator[](index);
 }
#endif
#endif
 typename std::vector<T>::iterator Find(const T& value);
 typename std::vector<T>::const_iterator Find(const T& value) const;
 void push_back(const T& _Val) { std::vector<T>::push_back(_Val); }
 void push_back() { T element; push_back(element); }
 void Append(const CArray& other);
 T* GetAsCArray();
 const T* GetAsCArray() const;
 const T* PeekAsCArray() const;
 unsigned int GetByteSize() const;
 void DeallocateMemory();
 void DeallocateUnusedMemory();
 unsigned long Sizeof() const;
private:
 T* GetZeroSizeArray();
 const T* GetZeroSizeArray() const;
};
#else
template<class T> class CArray : public std::vector<T> {
public:
#ifdef FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
 unsigned int GetByteSize()const;
#endif
#ifdef FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
    // Both original PDBs: user-written copy ctor/dtor attr0x3; assignment generated0x103.
    // FableWin033264B0 forwards copy to vector;02C89C80/01CA07F0 only call base dtor.
    CArray(const CArray& other) : std::vector<T>(other) { }
    ~CArray() { }
#endif
    CArray() : std::vector<T>() { }
#ifdef FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22
    // Both original types expose these At overloads; original02CFC49A calls vector::operator[].
    T& At(unsigned int index) { return std::vector<T>::operator[](index); }
    const T& At(unsigned int index) const { return std::vector<T>::operator[](index); }
#elif defined(FABLE_RETAIL_CARRAY_AT)
    // FableWin original At(T) called by02ebbf70; same index access as operator[].
    T& At(unsigned int i){return *(this->begin()+i);}
#endif
#if defined(FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE)
    // Per-TU inline opt-in (wfE10, row 0x00507C30 CWorldMap::LoadFromFile): retail calls the CArray<char> one-argument constructor out of
    // line (0x00410F30, Ego_r.pdb ??0?$CArray@D@@QAE@I@Z: three zero stores, malloc(n), memset-zero fill, `ret 4`) while its body stays
    // visible to the caller (the buffer's _M_start is kept in edi across the whole parse loop, 0x00507DE3..0x00509546). Default body.
#if defined(_MSC_VER)
    __declspec(noinline)
#endif
#if defined(FABLE_RETAIL_CARRAY)
    explicit CArray(unsigned int n) : std::vector<T>(n) { }
#else
    explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
#endif
#elif defined(FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE)
    // Opt-in (chunk Q16, row 0x00A90260 C3DPrimitive2::CreateDegenerateTriangleIndexBuffer): the one-argument constructor called out
    // of line (0x00A96100 for CArray<unsigned long>: three zero stores, malloc(n*4), `rep stosd` zero fill, `ret 4`; FableWin.pdb
    // ??0?$CArray@K@@QAE@I@Z). Declaration only.
    explicit CArray(unsigned int n);
#elif defined(FABLE_RETAIL_CARRAY)
    // retry11a (row 0x00520670 CArray<unsigned char>(size)): the one-argument ctor forwards to STLport's vector(n) -- the
    // vector(n, T()) form changes LoadHeroStateBinary's register allocation (1 instruction).
    explicit CArray(unsigned int n) : std::vector<T>(n) { }
#elif defined(FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE)
    // Per-TU inline control (agentFAM10, 2026-11-12, row 0x004538EB COpinionSet::COpinionSet; owner decision 2026-11-08, inline
    // budget): that size-optimised TU expands this wrapper (the 0.0f and allocator temporaries sit in the caller's frame, [ebp-8] /
    // [ebp-1]) and calls only the vector fill constructor 0x004516E7; VC7.1's /Os heuristic otherwise calls the wrapper. Same body.
    FABLE_FORCEINLINE explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
#else
    explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
#endif
    CArray(unsigned int n,const T& x) : std::vector<T>(n,x) { }
#if defined(FABLE_RETAIL_CARRAY_INDEX_ASSERT)
    // Opt-in (w24A, rows 0x0047042A CSoundMap::TransferBinaryIn, 0x00681E60). INFERRED: the debug build has out-of-line indexers
    // (FableWin.pdb ??A?$CArray@VCGraphicInfo@@@@QBEABVCGraphicInfo@@I@Z 0x01D2F450; the body, a bounds ASSERT, is not readable here:
    // FableWin.exe is not available); the release build keeps such an ASSERT as a dead arm (fable_assert.h; define FABLE_RETAIL_ASSERT_DEAD_ARM
    // in the row). It emits no bytes. Graded on 0x0047042A: with the arm, the second access to one element (`v[j].first = ...;
    // v[j].second.F()`) gives retail's `lea ecx,[eax+ecx+4]` (index * size first, 0x004706C9); without it `[ecx+eax+4]`.
    T& operator[](unsigned int index) { FABLE_RETAIL_ASSERT(index < this->size()); return std::vector<T>::operator[](index); }
    const T& operator[](unsigned int index) const { FABLE_RETAIL_ASSERT(index < this->size()); return std::vector<T>::operator[](index); }
#elif defined(FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22)
    // Original CArray<CGraphicInfo>::operator[]01D2F450 release tail calls vector::operator[].
    T& operator[](unsigned int index) { return std::vector<T>::operator[](index); }
    const T& operator[](unsigned int index) const { return std::vector<T>::operator[](index); }
#elif defined(FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22)
    // The actual original const CArray indexer calls the mutable vector indexer
    // (FableWin02e54e33,02f1a9f3,01a71595); release omits its bounds assertion.
    typename std::vector<T>::reference operator[](unsigned int index) { return std::vector<T>::operator[](index); }
    typename std::vector<T>::const_reference operator[](unsigned int index) const {
        return const_cast<std::vector<T>*>(static_cast<const std::vector<T>*>(this))->operator[](index);
    }
#elif defined(FABLE_RETAIL_CARRAY)
    // Retail opt-in (chunk 723, row 0x00BC14E0): the index operators go through begin() like STLport's vector::operator[]; the
    // element offset then folds into the addressing mode of the inlined CVertexShader::operator= (`[esi+0Ch]`), where the
    // `_M_start[i]` form materialises the element address first (`add esi,8`).
    T& operator[](unsigned int i) { return *(this->begin() + i); }
    const T& operator[](unsigned int i) const { return *(this->begin() + i); }
#else
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
#endif
#ifdef FABLE_RETAIL_CARRAY_GETASCARRAY_START
    // Nested opt-in (chunk 762, row 0x00B847D0 RadixSort): the non-const form reads _M_start like the const one (the landed
    // 0x00BB1EF0 CEngineClothRenderer::FrameStart model); `&front()` makes VC7.1 select into a register instead of
    // storing both arms of the conditional to the spill slot (`mov [esp+10h],eax ; ja ; mov [esp+10h],edi`).
    T* GetAsCArray() { return this->size()>0 ? this->_M_start : GetZeroSizeArray(); }
#else
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
#endif
    const T* GetAsCArray() const { return this->size()>0 ? this->_M_start : (const T*)this; }
#ifdef FABLE_RETAIL_CARRAY_GETBYTESIZE
    // Nested opt-in (2026-11-16, rows 0x00AE1730 / 0x00AE18C0 CParticleSimulation::ReadVectorArray / WriteVectorArray): FableWin.pdb
    // ?GetByteSize@?$CArray@..@@QBEIXZ (52 instances, e.g. CArray<float> 0x0179E49C), inline in retail: size() * sizeof(T). Body only.
    unsigned int GetByteSize() const { return this->size() * sizeof(T); }
#endif
#ifdef FABLE_RETAIL_CARRAY_SIZEOF
    // Nested opt-in (NF80, q10931: row 0x009C8060 NGameText::CGroupEntry::CGroupEntry): FableWin.pdb ?Sizeof@?$CArray@J@@QBEKXZ
    // (0x02F77E50; no Ego_r.pdb or Fable.exe copy), inline in retail: the object plus its reserved storage (0x009C8096..0x009C80A6
    // `mov edi,[esi+24h] ; sub edi,[esi+1Ch] ; sar edi,2` then `lea edi,[edi*4+..]`). Body only.
    // (the reserved element count written as STLport's capacity(), which fable_containers.h defines only under FABLE_RETAIL_STLP_VECTOR_C)
    unsigned long Sizeof() const { return sizeof(*this) + size_t(this->_M_end_of_storage._M_data - this->_M_start) * sizeof(T); }
#endif
#ifdef FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
    // Chunk 767 (row 0x009E5350 CSymbolMap::DeallocateUnusedMemory): Ego_r.pdb ?DeallocateUnusedMemory@?$CArray@..@@QAEXXZ (CMapInfo,
    // pair<K,CAnimationEntry>, unsigned short, unsigned char ... instances), out of line in retail (the pair<CCharString,
    // CSymbolMap::CSymbolInfo> instance 0x009E7C70). A declaration only.
    void DeallocateUnusedMemory();
#endif
#ifdef FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
    // Opt-in (chunk 1017, row 0x00AE3920 CParticleProgram::RemapBoneIndices): Ego_r.pdb ?push_back@?$CArray@VC3DGroup2@@@@QAEXABVC3DGroup2@@@Z,
    // out of line in retail (0x00AE83D0, bound by the landed AddGroup 0x00AE37D0). The body (the FABLE_RETAIL_CARRAY_PUSHBACK one) is
    // visible but kept out of line, as in the retail TU: VC7.1 then knows push_back never captures the array, so a local array's
    // start stays in a callee-saved register across its inline destructor's free calls (0x00AE39EC `mov ebx,[esp+1Ch]` ...
    // 0x00AE3A1D `push ebx`); declaration-only reloads it after every free.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    void push_back(const T& x) { std::vector<T>::push_back(x); }
#endif
#ifdef FABLE_RETAIL_CARRAY_PUSHBACK_VALUE_DECL_PASS27
    void push_back(const T& x);
#endif
#ifdef FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL
    // Opt-in (NF62, row 0x007E7C90 CTCFireheartMinigame::DrawPreparePrimitivesForRendering): Lionhead's argument-less push_back
    // (Ego_r.pdb / FableWin.pdb ?push_back@?$CArray@..@@QAEXXZ, 18 instances; only CArray has one in either PDB), called out of line in
    // that TU (0x005ED940 for CArray<CEnginePrimitiveHandle>: a default handle local, the inline vector push_back, its destructor).
    // Declaration only.
    void push_back();
#endif
#ifdef FABLE_RETAIL_CARRAY_PUSHBACK
    // Retail opt-in (chunk 785, rows 0x00BE4660 / 0x00BE4FE0 water tesselators): Lionhead's own push_back pair (Ego_r.pdb
    // ?push_back@?$CArray@VCTVertexWaterBackground@@@@QAEXABVCTVertexWaterBackground@@@Z and ...QAEXXZ, the Sea twins likewise).
    // push_back(const T&) is out of line in retail (0x00BE4F90 water, 0x00BE5950 sea, both landed under this spelling); the
    // argument-less push_back appends an uninitialised element: retail passes the address of a fresh, never-written local at each
    // expansion (0x00BE48E4 [esp+4Ch], 0x00BE497E [esp+0BCh], ...), the POD vertex having no constructor.
    // The CArray<unsigned short> instance expands inline (straight to vector<unsigned short>::_M_insert_overflow 0x005135D0).
    void push_back(const T& x) { std::vector<T>::push_back(x); }
    void push_back() { T element; push_back(element); }
#endif
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
#ifdef FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
// Original FableWin02e4c2a0/03065c20: vector element count times original element sizeof.
template<class T> inline unsigned int CArray<T>::GetByteSize()const{return this->size()*sizeof(T);}
#endif
#endif
