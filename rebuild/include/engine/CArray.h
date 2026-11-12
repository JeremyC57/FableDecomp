#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// CArray<T> (Codex pass 11 consolidation, one model): Lionhead's array is a public STLport 4.6 vector<T> (12 bytes in retail;
// the donor PDB's later vector base is 16) adding no fields. Ego_r CArray<C3DVector> / CArray<long>: unsigned-int index
// operators returning T& and const T&, the (n) and (n, value) constructors, GetAsCArray over a private zero-size array.
#include "stlport/fable_containers.h"
#include "fable_compiler.h"
template<class T> class CArray : public std::vector<T> {
public:
    CArray() : std::vector<T>() { }
#ifdef FABLE_RETAIL_CARRAY_AT
    // FableWin original At(T) called by02ebbf70; same index access as operator[].
    T& At(unsigned int i){return *(this->begin()+i);}
#endif
#if defined(FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE)
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
#ifdef FABLE_RETAIL_CARRAY
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
