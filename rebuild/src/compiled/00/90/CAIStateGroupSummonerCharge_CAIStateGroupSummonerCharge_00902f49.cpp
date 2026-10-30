// CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge() -- the compiler-generated (implicit) destructor, kept out of line: release the def pointer PDef (+0x1C), destroy PTarget (+0x14), then the base destructor CAIStateGroupBase::~CAIStateGroupBase
// Leaf: ??1CAIStateGroup_SummonerCharge@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902F49, 40 bytes (+ int3 padding), thiscall, no return value; size-optimised TU (odd entry,
//   `and dword ptr [esi+1Ch],0`); reached only from its scalar deleting destructor 0x00902F2D.
// Identity (proven): CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(); evidence: its only caller is the scalar deleting destructor 0x00902F2D in slot 0 of vtable 0x0128D2DC, whose RTTI is .?AVCAIStateGroup_SummonerCharge@@ : CAIStateGroupBase : CBaseClass; the PDB name sets hold ??1CAIStateGroup_SummonerCharge@@UAE@XZ; the released and destroyed offsets +0x1C / +0x14 are Ego_r.pdb CAIStateGroup_SummonerCharge::PDef / PTarget, and the tail jump is the base's destructor.
// Label: the manifest's "~CCreatureAction_FireMissileWeapon" is propagated across byte-identical bodies (family J081: five implicit destructors of five
//   classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CIVCountedPointeeBase::Delete() const (the def's vtable slot 1, on the last release); CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the implicit ~CIntelligentPointer<CThing>); CAIStateGroupBase::~CAIStateGroupBase 0x008FD288 (tail jmp; PDB-spelled ??1CAIStateGroupBase@@UAE@XZ).
// Layout: Ego_r.pdb CAIStateGroup_SummonerCharge members at their PDB offsets.
// Shape: the J072 model (no vtable store -- VC7.1 resets the vptr only in user-written destructors; members destroyed in reverse
//   declaration order, the last one the tail `jmp`) with a CDefPointer member whose inline destructor is the IV release
//   (`p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; }`, agent952 H021 / J075), `__forceinline` because
//   under /Os VC7.1 keeps the class-body destructor out of line (grade 1: a call, 27 bytes) and without /Os the clear is a 7-byte
//   `mov` (grade 2, 43 bytes); retail has both the size-TU `and` and the inlined release. KillOne's explicit
//   destructor call is a modelling device that makes VC7.1 emit the implicit destructor out of line.
#pragma optimize("s", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CIVCountedPointeeBase {                            // Ego_r.pdb 8 bytes: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1 (FableWin.pdb ?Delete@CIVCountedPointeeBase@@UBEXXZ)
    unsigned long IVRefCount;                            // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {         // Ego_r.pdb 0x1C
public:
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource { };            // Ego_r.pdb 0x1C, no members of its own
template <class T> class CDefPointer {                   // Ego_r.pdb 4 bytes
public:
    __forceinline ~CDefPointer()                         // the IV release, inline (FableWin.pdb has it out of line, debug)
    {
        CDefPointeeBase* p = Object;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                             // +0x0
};
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CThing;
class CAIBrain;
enum EScriptAIPriority { };                              // Ego_r.pdb (enumerators not needed)
class CAIStateGroupBase : public CBaseClass {            // Ego_r.pdb 20 bytes
public:
    virtual ~CAIStateGroupBase();                        // 0x008FD288 (user-written: stores its vtable)
    CAIBrain*         PBrain;                            // +0x04
    EScriptAIPriority ScriptAIPriority;                  // +0x08
    CCharString       Name;                              // +0x0C
    bool              Terminated;                        // +0x10
    unsigned char     Priority;                          // +0x11
    bool              Enabled;                           // +0x12
};
class CSummonerDef;
class CAIStateGroup_SummonerCharge : public CAIStateGroupBase {   // vtable 0x0128D2DC, Ego_r.pdb 0x24 bytes (no containers: retail = Ego_r)
public:
    CIntelligentPointer<CThing>      PTarget;            // +0x14
    CDefPointer<const CSummonerDef>  PDef;               // +0x1C
    long                             NextFrameToCharge;   // +0x20
};
void KillOne(CAIStateGroup_SummonerCharge* p) { p->CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(); }