// CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id) -- build a spawned script function: a 0x40-word, 0.1-timeslice running process with the given name and ID, no frames run, no region, no predicate
// Leaf: ??0CSpawnedFuncBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD450, 80 bytes, `__fastcall`, `ret 8`; vtable 0x012C2F9C. The manifest label
//   (CSpawnedFunc<NScript::CExpression_FollowScript> ctor) is propagated: the body stores only the base vtable and takes two
//   arguments, and the landed CAT_BanditContact::Init 0x00E6AAC0 names 0x00CDD450 the CSpawnedFuncBase ctor (the CSpawnedFunc<T>
//   ctors such as 0x00E95F10 inline it).
// Callees: the process base ctor (bool running, long stackSize, float timeslice) 0x00A44740 (called with 1, 0x40, 0.1f),
//   CCharString(const CCharString&) 0x0099EC30, CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CSpawnedFuncBase 0x34 (ID +0x14, LastScriptFrame +0x18, CurrentScriptFrame +0x1C, Name +0x20,
//   AssociatedRegionName +0x24, FuncPaused +0x28, Interrupted +0x29, PPredicate +0x2C). Local polymorphic model: the generated
//   engine/CSpawnedFuncBase.h flattens the vptr into a data member, so it cannot carry the vtable store or the base ctor call.
// Shape: member-initialiser order (ID, both frames -1, Name copy, AssociatedRegionName default, the two bools, the counted
//   pointer's inline zeroing ctor); no /EHsc (retail has no EH frame).
class CCharString {                                       // one pointer to the shared string data
public:
    void* PStringData;                                    // +0x00
    CCharString();                                        // 0x0099E4B0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
namespace NScript { class CPredicateBase; }
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};
// The 0x14-byte process base: CASuspendableProcess (vptr, Running +4, StopRunning +5, TerminateUsingExceptions +6,
// TimeProcessSliceStarted +8, TimesliceLength +0xC) plus the dword at +0x10 its (bool, long, float) ctor 0x00A44740 zeroes.
class CScriptProcessBase {
public:
    bool Running;                                         // +0x04
    bool StopRunning;                                     // +0x05
    bool TerminateUsingExceptions;                        // +0x06
    float TimeProcessSliceStarted;                        // +0x08
    float TimesliceLength;                                // +0x0C
    unsigned long ProcessState;                           // +0x10
    CScriptProcessBase(bool running, long stackSize, float timesliceFraction);   // 0x00A44740
    virtual ~CScriptProcessBase();
    virtual void SuspendableProcess() = 0;
};
class CSpawnedFuncBase : public CScriptProcessBase {      // Ego_r.pdb: 0x34 bytes
public:
    unsigned long ID;                                     // +0x14
    long LastScriptFrame;                                 // +0x18
    long CurrentScriptFrame;                              // +0x1C
    CCharString Name;                                     // +0x20
    CCharString AssociatedRegionName;                     // +0x24
    bool FuncPaused;                                      // +0x28
    bool Interrupted;                                     // +0x29
    CCountedPointer<const NScript::CPredicateBase> PPredicate;   // +0x2C
    CSpawnedFuncBase(const CCharString& name, unsigned long id);
    virtual ~CSpawnedFuncBase();
};
CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id)
    : CScriptProcessBase(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
      FuncPaused(false), Interrupted(false)
{
}