// CScriptGameResourceObjectMovieBase::Release() -- drop the movie handle's implementation (reset its counted pointer to null)
// Leaf: ?Release@CScriptGameResourceObjectMovieBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF8850, 11 bytes, thiscall, plain `ret`.
// Identity (probable): the manifest label _Dest_val<allocator<NCreatureHitNotification::CFlash>, CFlash> is propagated (CFlash
//   holds no counted pointer at +8). The body resets the CCountedPointer at +8 through 0x008AB2B0, whose only other caller is
//   0x008AB970 (`add ecx,8 ; jmp 0x008AB2B0`), the movie handle's SetImplementation that the landed
//   CGameScriptInterface::StartMovieSequence 0x0089B110 calls on its CScriptGameResourceObjectMovieBase& argument. Ego_r.pdb
//   CScriptGameResourceObjectMovieBase is {vptr, +4, CCountedPointer<CScriptGameResourceObjectMovieBase> PImp +8} with the
//   non-virtual pair SetImplementation(CScriptGameResourceObjectMovieBase*) and Release(): this is Release (PImp.Reset(0)).
//   Its two callers (0x00CF70BC, 0x00CF7EFA in a quest Main) release a local movie handle after Pause(1.0f).
// Callees: CCountedPointer<CScriptGameResourceObjectMovieBase>::Reset(CScriptGameResourceObjectMovieBase*) 0x008AB2B0 (Ego_r.pdb
//   spelling; landed as the descriptive CCountedPointer_008aa7f0::Reset, deleter 0x008AA7F0).
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    void Reset(T* data);                                // 0x008AB2B0 for T = CScriptGameResourceObjectMovieBase
};
class CScriptGameResourceObjectMovieBase {              // Ego_r.pdb: vptr, +4, PImp +8 (0x10)
public:
    virtual ~CScriptGameResourceObjectMovieBase();
    void Release();
    long                                                _4;
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};

void CScriptGameResourceObjectMovieBase::Release()
{
    PImp.Reset(0);
}