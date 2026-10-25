// pSunnyvaleDataAlloc_func() -- tScriptAllocFunc of Q_SunnyvaleMaster's script data: new + inline ctor of NScript::CQ_SunnyvaleMasterData (0x144 bytes)
// Leaf: ?pSunnyvaleDataAlloc_func@@YIPAVCScriptDataBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDBD20, 79 bytes, `__fastcall` global with no arguments, plain `ret`.
//   It is the pAllocDataFunc of the Q_SunnyvaleMaster CScriptInfo record registered by 0x00CD52D0 (FSE FableAPI.cpp
//   names the address pSunnyvaleDataAlloc_func, type tScriptAllocFunc; the C++ symbol is not recovered).
// Callees: scalar operator new @ 0x00BFEA1A (0x144 bytes), CScriptDataBase::CScriptDataBase() @ 0x0099A2F0 (folded with
//   the identical CBaseClassNonCopyable ctor), CCharString::CCharString() @ 0x0099E4B0 (TeddySolution, +0x54).
// Data: CQ_SunnyvaleMasterData's vtable @ 0x012C2748 (slot 0 its vector deleting dtor @ 0x00CDBD70).
// Layout = Ego_r.pdb NScript::CQ_SunnyvaleMasterData: CScriptDataBase {vptr}, the gameflow flags/counters
//   (PostSavePosition +4 .. CondemnedManMeetsBodyGuardCutSceneFinished +0x50), CCharString TeddySolution +0x54,
//   the flags/counters OrchardFarmRaidLastCompleted +0x58 .. HeroWoreMask +0x133, vector<CCharString> BoastRegions
//   +0x134, bool SingingStonesInSync +0x140. The PDB's 328 bytes carry a 16-byte Dinkumware vector; retail's STLport
//   vector is 12 bytes, so the object is 0x144.
// Shape: speed TU (no pragma). The ctor is inline: out-of-line base ctor, vtable store, TeddySolution's ctor, then the
//   empty BoastRegions vector's three pointers zeroed with imm32 stores; the plain-data members are not initialised.
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

namespace std {
template <class T> class vector {
public:
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

class CScriptDataBase {
protected:
    CScriptDataBase();                                  // 0x0099A2F0
public:
    virtual ~CScriptDataBase();
};

namespace NScript {
class CQ_SunnyvaleMasterData : public CScriptDataBase {
public:
    CQ_SunnyvaleMasterData() {}
    virtual ~CQ_SunnyvaleMasterData();                  // ??_E @ 0x00CDBD70
    unsigned char _gameflowState04[0x50];               // PostSavePosition .. CondemnedManMeetsBodyGuardCutSceneFinished
    CCharString TeddySolution;                          // +0x54
    unsigned char _gameflowState58[0xDC];               // OrchardFarmRaidLastCompleted .. HeroWoreMask
    std::vector<CCharString> BoastRegions;             // +0x134
    bool SingingStonesInSync;                           // +0x140
};
}

CScriptDataBase* __fastcall pSunnyvaleDataAlloc_func()
{
    return new NScript::CQ_SunnyvaleMasterData;
}