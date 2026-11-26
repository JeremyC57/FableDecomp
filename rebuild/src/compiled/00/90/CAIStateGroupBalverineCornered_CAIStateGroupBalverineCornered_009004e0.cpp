// CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered() -- construct the state group and its two tracked things.
// Flags: /Os
// Leaf: ??0CAIStateGroup_BalverineCornered@@QAE@XZ
// NF472 (0x00900533 self respell, the r469a declaration-only form): retail's ~CAIStateGroup_BalverineCornered is the implicit one, kept once at 0x00900533
//   (both PDBs flag it compiler-generated; owner 0x00900517 on engine/CAIStateGroup_BalverineCornered.h). This local model's implicit copy was not retail's
//   (38 bytes: its local CIntelligentPointer<T> destructor is expanded with a vptr store), so the destructor is declared here and the object references
//   0x00900533's ??1CAIStateGroup_BalverineCornered@@UAE@XZ; the row's own bytes are unchanged.
// Identity: proven; own RTTI 0128C0BC and exact PDB default ctor; original CAIStateGroupBase chain; tracked members PTarget/PLastJumpTarget at20/28 and own template vtables.
// Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.
// NF469 (r467a, ruling r469a): retail's ~CAIStateGroupBase is the implicit one, kept once at 0x008FD288 (Ego_r / FableWin flag it
//   compiler-generated). This local model's implicit copy was not retail's (a lone jmp ~CBaseClass: the model gives Name (+0x0C) no destructor), so the destructor is declared here and the
//   object references 0x008FD288's ??1CAIStateGroupBase@@UAE@XZ like the 640 landed local models; the row's own bytes are unchanged.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{SCRIPT_PRIORITY_DEFAULT};
class CAIStateGroupBase:public CBaseClass{public: virtual ~CAIStateGroupBase();CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;CAIStateGroupBase();};template<class T>class CAIStateGroup_WorkBase:public T{};
class CAIStateGroup_JackOfBladesBase:public CAIStateGroupBase{public:unsigned char _unmodeled14[0x4c-0x14];CAIStateGroup_JackOfBladesBase();};
class CBaseObject;class CBaseObjectPointer;class CThing;class CThingCreatureBase;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer():CBaseIntelligentPointer(){}virtual ~CIntelligentPointer(){}};
class CAIStateGroup_BalverineCornered:public CAIStateGroupBase{public: virtual ~CAIStateGroup_BalverineCornered();CIntelligentPointer<CThingCreatureBase> PTarget,PLastJumpTarget;CAIStateGroup_BalverineCornered();};
CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered(){}