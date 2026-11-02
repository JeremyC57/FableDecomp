# h32_aisx.py: naming wave 32, family aisg (special rows, after h32_aisg.py): the own row CAIStateGroupBase::CAIStateGroupBase 0x008FD255
#  (generated over CBase_0099a2d0 / CMember_0099e4b0, truncated Leaf) defined on the header's retail branch (Name is the CCharString
#  built by ??0CCharString@@QAE@XZ 0x0099E4B0); CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat 0x008FF1F8 is protected (IAE in Ego_r.pdb
#  and FableWin.pdb; its three derived ctors call ??0CAIStateGroup_UsesSeat@@IAE@XZ) and moves onto the header; 0x00900241
#  (CAIStateGroup_MinionHawkingFromStall, base ??0CAIStateGroupBase_Calls@@QAE@XZ) moves onto the header through its RTTI base
#  CAIStateGroup_WorkBase<CAIStateGroupBase>.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
INC = ('#define FABLE_RETAIL_CAISTATEGROUPBASE\n'
       '#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255\n')
he.put('008fd255', 'src', '''// CAIStateGroupBase::CAIStateGroupBase() -- CBaseClass(), own vtable, then the Name string built out of line
// Leaf: ??0CAIStateGroupBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD255, 26 bytes, thiscall, `ret`.
// Identity: stores vtable 0x01288C60 (RTTI .?AVCAIStateGroupBase@@ : CBaseClass); Ego_r.pdb / FableWin.pdb ??0CAIStateGroupBase@@QAE@XZ;
//   the base ctor of every CAIStateGroup_* ctor (chunk 703 NAME_FIXES).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (engine/CBaseClass.h), CCharString::CCharString 0x0099E4B0 (??0CCharString@@QAE@XZ).
// Layout: engine/CAIStateGroupBase.h's retail branch (Ego_r.pdb members: PBrain +0x04, ScriptAIPriority +0x08, Name +0x0C,
//   Terminated / Priority / Enabled +0x10..+0x12; 0x14 bytes).
// Shape: user default ctor, empty: CBaseClass(), the vtable store, then Name's default ctor; the other members stay uninitialised.
// History: generated in a chunk batch ("One of 16 bodies of this shape") over CBase_0099a2d0 / CMember_0099e4b0 with the truncated Leaf
//   ??0CAIStateGroupBase@; re-authored on the header in naming wave 32 (family aisg).
''' + INC + '''
CAIStateGroupBase::CAIStateGroupBase()
{
}
''')
a = '008ff1f8'
he.edit(a, 'src', '''class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {   // vtable 0x0128B53C
''', INC + '''
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {   // vtable 0x0128B53C
public:
''')
he.edit(a, 'src', '''    bool SittingOnBench;                        // +0x30
    CAIStateGroup_UsesSeat();
};''', '''    bool SittingOnBench;                        // +0x30
protected:
    CAIStateGroup_UsesSeat();                   // ??0CAIStateGroup_UsesSeat@@IAE@XZ (Ego_r.pdb / FableWin.pdb: protected)
};''')
note(a, 'aisg', 'the ctor is protected (IAE, both PDBs) and the local CAIStateGroupBase (a declared virtual dtor neither PDB has) is engine/CAIStateGroupBase.h\'s retail branch', leaf='??0CAIStateGroup_UsesSeat@@IAE@XZ')
a = '00900241'
he.edit(a, 'src', '''#pragma optimize("s", on)
struct CAIStateGroupBase_Calls {
    virtual void v00();
    unsigned char _body[0x10];                              // PBrain, ScriptAIPriority, Name, flags
    CAIStateGroupBase_Calls();                              // 0x008FD255
};
''', '''#pragma optimize("s", on)
''' + INC + '''template <class T> class CAIStateGroup_WorkBase : public T { };     // RTTI CAIStateGroup_WorkBase<CAIStateGroupBase> (no ctor of its own)
''')
he.edit(a, 'src', '''struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase_Calls {
    long                NextHawkFrame;                      // +0x14
    CIntelligentPointer<CThingCreatureBase> Customer;                           // +0x18
    virtual void v00();                                     // own vtable 0x0128BF48
    CAIStateGroup_MinionHawkingFromStall();
};

CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall()
    : CAIStateGroupBase_Calls()
    , NextHawkFrame(0)''', '''class CAIStateGroup_MinionHawkingFromStall : public CAIStateGroup_WorkBase<CAIStateGroupBase> {   // own vtable 0x0128BF48
public:
    long                NextHawkFrame;                      // +0x14
    CIntelligentPointer<CThingCreatureBase> Customer;                           // +0x18
    CAIStateGroup_MinionHawkingFromStall();
};

CAIStateGroup_MinionHawkingFromStall::CAIStateGroup_MinionHawkingFromStall()
    : NextHawkFrame(0)''')
he.edit(a, 'src', '''// Layout = Ego_r.pdb: CAIStateGroupBase is 0x14 bytes (engine/CAIStateGroupBase.h, a flat POD ->
// `_Calls` model), NextHawkFrame''', '''// Layout = Ego_r.pdb: CAIStateGroupBase is 0x14 bytes (engine/CAIStateGroupBase.h's retail branch; RTTI base
// CAIStateGroup_WorkBase<CAIStateGroupBase>), NextHawkFrame''')
note(a, 'aisg', 'the base is engine/CAIStateGroupBase.h\'s retail branch through the RTTI base CAIStateGroup_WorkBase<CAIStateGroupBase> (was ??0CAIStateGroupBase_Calls@@QAE@XZ)', leaf='??0CAIStateGroup_MinionHawkingFromStall@@QAE@XZ')
