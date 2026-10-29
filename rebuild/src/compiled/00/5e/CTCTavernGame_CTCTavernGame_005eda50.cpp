// CTCTavernGame::~CTCTavernGame() -- tavern mini-game TC dtor (compiler-generated): the member dtors in reverse order (the game master pointer, three font-bank counted pointers, five engine primitive handle vectors, the UI component vector) and ~CTCBase
// Leaf: ??1CTCTavernGame@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EDA50, 417 bytes, tail jump to the base destructor.
// Identity: the only call is from 0x005EDA30, slot 0 (the scalar deleting destructor) of vtable 0x012560BC, RTTI
//   `.?AVCTCTavernGame@@` (rtti_bases: CTCTavernGame : CTCBase); the derived tavern-game dtors (0x007D1ADC, 0x007D5EE2,
//   0x007D9F09: CTCTavernGameCardBase / CoinBase family) tail-jump to it. Ego_r.pdb has `??1CTCTavernGame@@UAE@XZ` and
//   `??_GCTCTavernGame@@UAEPAXI@Z`. No vtable store: the destructor is implicit (VC7.1 resets the vptr only in user-written
//   destructors). The catalog label `CChunkedFileChunk::DestructorCleanup` is wrong (0x00686830 is ~CTCBase, not
//   ~CChunkedFileChunk).
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (PGameMaster: the implicit ~CIntelligentPointer<CThingAICreature>), operator delete
//   0x00BFE9BC (the counted pointers' info blocks), free 0x00BFEA14 (vector blocks), CTCBase::~CTCBase 0x00686830 (tail jump);
//   virtual calls: each count block's DeleteFunc, each CEnginePrimitiveHandle's slot-0 destructor (flag 0: STLport _Destroy).
// Layout: Ego_r.pdb CTCTavernGame (0x108) with retail STLport vectors (0xC): UIComponents +0x0C, LastRenderTime +0x18, ...,
//   Handles +0x44, BoxHandles +0x50, BetHandles +0x5C, MoneyHandles +0x68, WinningsHandles +0x74, PScoreFont +0x80, PTargetFont
//   +0x88, PStatsFont +0x90, ..., PGameMaster +0xD0, GamePlayed +0xD8 ... +0xF0. No engine header for CTCTavernGame.
// Shape: the implicit destructor, emitted out of line through a qualified explicit destructor call in a one-line helper
//   (EmitImplicitDestructor, not a retail function); CCountedPointer's inline release (`ReleaseData(); Data = 0; Info = 0;`),
//   STLport ~vector (_Destroy the range, then ~_Vector_base frees the block).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};


class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    CBaseObjectPointer* PData;                              // +0x04
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};
class CThingAICreature;
class CFontBank;
namespace NUISystem { class CComponent; }

class CTCTavernGame : public CTCBase { public:
    enum ETavernGameStates { };                             // values not needed here
    std::vector<NUISystem::CComponent*>     UIComponents;              // +0x0C
    double                                  LastRenderTime;            // +0x18
    bool                                    IsBeingPlayed;             // +0x20
    bool                                    GameWon;                   // +0x21
    long                                    NumRounds;                 // +0x24
    bool                                    GameFinished;              // +0x28
    bool                                    GameInSession;             // +0x29
    float                                   Score;                     // +0x2C
    float                                   Target;                    // +0x30
    long                                    BaseBet;                   // +0x34
    long                                    LastBet;                   // +0x38
    long                                    TotalWinnings;             // +0x3C
    long                                    CurrentMoney;              // +0x40
    std::vector<CEnginePrimitiveHandle>     Handles;                   // +0x44
    std::vector<CEnginePrimitiveHandle>     BoxHandles;                // +0x50
    std::vector<CEnginePrimitiveHandle>     BetHandles;                // +0x5C
    std::vector<CEnginePrimitiveHandle>     MoneyHandles;              // +0x68
    std::vector<CEnginePrimitiveHandle>     WinningsHandles;           // +0x74
    CCountedPointer<CFontBank>              PScoreFont;                // +0x80
    CCountedPointer<CFontBank>              PTargetFont;               // +0x88
    CCountedPointer<CFontBank>              PStatsFont;                // +0x90
    float                                   TargetTime;                // +0x98
    long                                    TargetRound;               // +0x9C
    bool                                    NewBestScore;              // +0xA0
    bool                                    Draw;                      // +0xA1
    bool                                    GameStarted;               // +0xA2
    long                                    NumConsecutiveRoundsWon;   // +0xA4
    ETavernGameStates                       YesState;                  // +0xA8
    ETavernGameStates                       NoState;                   // +0xAC
    ETavernGameStates                       AltState;                  // +0xB0
    void (__cdecl* YesFunc)();                                         // +0xB4
    void (__cdecl* NoFunc)();                                          // +0xB8
    void (__cdecl* AltFunc)();                                         // +0xBC
    ETavernGameStates                       TavernGameState;           // +0xC0
    ETavernGameStates                       PreviousTavernGameState;   // +0xC4
    ETavernGameStates                       NextTavernGameState;       // +0xC8
    float                                   StateTimer;                // +0xCC
    CIntelligentPointer<CThingAICreature>   PGameMaster;               // +0xD0
    bool                                    GamePlayed;                // +0xD8
    long                                    FrameBetWasLastChanged;    // +0xDC
    long                                    FrameBetChangeStarted;     // +0xE0
    long                                    Mult;                      // +0xE4
    bool                                    BetIncreasing;             // +0xE8
    bool                                    HasPrizeBeenGiven;         // +0xE9
};

void EmitImplicitDestructor(CTCTavernGame* game)            // emission device only (not a retail function)
{
    game->CTCTavernGame::~CTCTavernGame();
}