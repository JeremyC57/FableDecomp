// std::vector<CLevelFiles>::push_back() -- (CLevelLoader::CObject::CLevelFiles) append a default-constructed 28-byte level-file record
// Leaf: PushBackEmpty
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051B620, length 50 bytes (+ int3 padding).
// Callees: vector<CLevelLoader::CObject::CLevelFiles>::push_back(const CLevelFiles&) @ 0x0051AE60 (landed; it copies
// the record into `_M_finish` when there is room: LevelID, then the three counted pointers with their add-refs),
// CLevelLoader::CObject::CLevelFiles::~CLevelFiles @ 0x0050F980 (landed).
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong. `this` is handed to
// push_back untouched (ecx is never reloaded), so the function is a member of the vector of CLevelFiles itself
// (the CLevelLoader::CObject::RegionLevelFiles element type, nested per naming wave 20 / chunk 688); it has no
// direct caller in the image. STLport 4.6 _vector.h's `void push_back() { push_back(_Tp()); }` is the matching text
// (the no-argument overload under _STLP_DONT_SUP_DFLT_PARAM); FableWin.pdb does not carry the QAEXXZ spelling, so the
// member keeps the descriptive name PushBackEmpty (DEFERRED: the vector::push_back() identity).
//
// Layout = Ego_r.pdb CLevelFiles (0x1c bytes): LevelID +0x00, then three CCountedPointer<CAFile>
// {object, count block} at +0x04 / +0x0c / +0x14 (PTopologyFile, PNavigationFile, PThingFile).
// Shape: the temporary is built in the 0x1c-byte frame: the implicit constructor leaves LevelID
// uninitialised and the three counted pointers' inline constructors zero their six words from one
// register; it is passed by reference and destroyed through the out-of-line destructor.
class CAFile;

#include "engine/CCountedPointer.h"

class CLevelLoader { public: class CObject { public:
class CLevelFiles { public:
    long                    LevelID;            // +0x00
    CCountedPointer<CAFile> PTopologyFile;      // +0x04
    CCountedPointer<CAFile> PNavigationFile;    // +0x0c
    CCountedPointer<CAFile> PThingFile;         // +0x14
    ~CLevelFiles();                             // 0x0050F980
};
}; };

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector<CLevelFiles> (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);             // 0x0051AE60
    void PushBackEmpty();                       // STLport `push_back()` (see above)
};
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::PushBackEmpty()
{
    push_back(_Tp());
}
template void vector<CLevelLoader::CObject::CLevelFiles>::PushBackEmpty();
}
