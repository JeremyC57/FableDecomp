// CMemoryDataOutputStream::ReserveSize(unsigned long size) -- reserve `size` bytes in the stream's Data array
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996BC0, 8 bytes: `add ecx,8 ; jmp 0x00997330` (thiscall, the size stays on the
//   stack for the tail-called vector<char>::reserve).
// Identity: Ego_r.pdb + FableWin.pdb ?ReserveSize@CMemoryDataOutputStream@@QAEXK@Z; Data (CArray<char>, retail the bare
//   12-byte STLport vector<char>) is at +8 (Ego_r.pdb CMemoryDataOutputStream::Data, engine/CMemoryDataOutputStream.h), and
//   0x00997330 is vector<char>::reserve (FINDINGS chunk 408; capacity test then malloc/copy/free of the char block). The
//   callers reserve save buffers on stack streams: CWorld::SaveGameStateInternal (0x400 and 0x8000 bytes, after
//   CPersistContext::Init on the stream), CUserProfileManager::SaveGameState, CThingManager save paths, CDefStringTable::
//   ReadTable. Not the propagated `_Cons_val<allocator<pair<EHeroMorphType, CParticleMorphs::CEntry>>, ...>` label.
// Layout: engine/CMemoryDataOutputStream.h has the fields but declares no member functions, so the class is modelled
//   locally with its retail offsets (Data +0x08, CurrentPosition +0x14).
// Leaf: ?ReserveSize@CMemoryDataOutputStream@@QAEXK@Z
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    void reserve(unsigned int __n);             // 0x00997330 for vector<char>
};
}
template <class T> class CArray : public std::vector<T> { };

class CMemoryDataOutputStream {
public:
    void*         _vftable;                     // +0x00 (CDataOutputStream vptr)
    void*         NewLine;                      // +0x04 CDataOutputStream::NewLine (CCharString)
    CArray<char>  Data;                         // +0x08
    unsigned long CurrentPosition;              // +0x14
    void ReserveSize(unsigned long size);
};

void CMemoryDataOutputStream::ReserveSize(unsigned long size)
{
    Data.reserve(size);
}
