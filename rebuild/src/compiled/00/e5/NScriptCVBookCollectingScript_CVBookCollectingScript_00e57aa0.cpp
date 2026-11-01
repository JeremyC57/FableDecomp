// NScript::CV_BookCollectingScript::~CV_BookCollectingScript() -- implicit quest-script destructor: free BookOwned and BookDonated, destroy and free BookDef, destroy seh_Girl and seh_Boy, destroy and free BookReactions, then ~CScriptBase
// Leaf: ??1CV_BookCollectingScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57AA0, 265 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00E57A83 is the scalar deleting destructor 0x00E57A80, slot 0 of the RTTI-less vtable
//   0x012E5EFC; that vtable is stored only by the ctor 0x00E54310 (landed), whose only caller is the Alloc 0x00E57A50
//   (`push 0B8h ; call new ; ... call 0x00E54310`), stored at 0x00CD849C by the script-registration routine right after the
//   level name "V_BookCollecting" (0x012C58CC) and the save tag "S_VBC" (0x012F774C); refs/script_recovery/conversion_queue.tsv
//   maps S_VBC -> V_BookCollecting -> Alloc 0x00E57A50. Ego_r.pdb ??0CV_BookCollectingScript@NScript@@QAE@PAVCScriptDataBase@@
//   AAVCGameScriptInterfaceBase@@@Z and the class's member list. The body stores no vtable of its own: the destructor is
//   compiler-generated. The catalog label "CParticleEmitter::~CParticleEmitter_MeshFree" is a propagated placeholder and wrong.
// Layout: Ego_r.pdb CV_BookCollectingScript (0xE4) members, 0x14 lower in retail, 4 lower after each Dinkumware vector and 8
//   lower after each Dinkumware vector<bool> (20 bytes there, STLport's 12-byte generic vector here: one byte per element, no
//   element loop): ReadingBook +0x48, BookReactions +0x4C (vector<CConversation>, 0x5C-byte elements with a virtual dtor),
//   seh_Boy +0x58, seh_Girl +0x68, DoneIntro..KeyRewarded +0x78..+0x7A, GossipState +0x7C, HasInformation +0x80,
//   BooksDonated +0x84, GoodBooksDonated +0x88, LastBookRequested +0x8C, BooksInGame +0x90, BookDef +0x94
//   (vector<CCharString>), BookDonated +0xA0, BookOwned +0xAC; retail size 0xB8 (Alloc `push 0B8h`).
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), ~CCharString 0x0099EAE0, CConversation's virtual destructor
//   (vtable slot 0, flags 0), CScriptGameResourceObjectScriptedThingBase implicit dtor inline (PImp release, operator delete
//   0x00BFE9BC, then ~CScriptGameResourceObjectBase: vtable 0x0126008C + ~CBaseObject 0x0099A430), ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h or engine/CConversation.h exists: CCharString is the one-pointer local model of the landed quest rows,
//   CConversation the 92-byte virtual-dtor model of the landed vector<CConversation> rows (0x00F337A0).
// TU: the PDB ctor is defined with an empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor and
//   this implicit dtor; only the dtor is graded.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h (the members this row needs; the malloc-backed allocator of this
// build: deallocate = free 0x00BFEA14)
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CCharStringData;
class CCharString { public:
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
class CConversation { public:                               // retail 92 bytes: vptr + the conversation data (not touched here)
    virtual ~CConversation();                               // slot 0 (the scalar deleting destructor)
    unsigned char Data[92 - 4];
};

class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase { public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                         // +0x40
    T*                        MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CV_BookCollectingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool ReadingBook;                                                    // +0x48
    std::vector<CConversation> BookReactions;                            // +0x4C
    CScriptGameResourceObjectScriptedThingBase seh_Boy;                  // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl;                 // +0x68
    bool DoneIntro;                                                      // +0x78
    bool HatRewarded;                                                    // +0x79
    bool KeyRewarded;                                                    // +0x7A
    long GossipState;                                                    // +0x7C
    bool HasInformation;                                                 // +0x80
    long BooksDonated;                                                   // +0x84
    long GoodBooksDonated;                                               // +0x88
    long LastBookRequested;                                              // +0x8C
    long BooksInGame;                                                    // +0x90
    std::vector<CCharString> BookDef;                                    // +0x94
    std::vector<bool> BookDonated;                                       // +0xA0
    std::vector<bool> BookOwned;                                         // +0xAC
};
CV_BookCollectingScript::CV_BookCollectingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E54310
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BookCollectingScript) == 0xB8);