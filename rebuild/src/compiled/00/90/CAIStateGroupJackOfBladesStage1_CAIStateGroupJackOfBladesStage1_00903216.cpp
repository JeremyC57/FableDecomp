// CAIStateGroup_JackOfBladesStage1::CAIStateGroup_JackOfBladesStage1() -- construct the state group and its two tracked things.
// Flags: /Os
// Leaf: ??0CAIStateGroup_JackOfBladesStage1@@QAE@XZ
// Identity: proven; own RTTI 0128D6B4 and exact PDB default ctor; original CAIStateGroup_JackOfBladesBase chain; tracked members PObstruction/PShieldThing and own template vtables; Stage1 member donor92/100->retail84/92 after two base vectors shrink8.
// Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{SCRIPT_PRIORITY_DEFAULT};
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;CAIStateGroupBase();};template<class T>class CAIStateGroup_WorkBase:public T{};
class CAIStateGroup_JackOfBladesBase:public CAIStateGroupBase{public:unsigned char _unmodeled14[0x4c-0x14];CAIStateGroup_JackOfBladesBase();};
class CBaseObject;class CBaseObjectPointer;class CThing;class CThingCreatureBase;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer():CBaseIntelligentPointer(){}virtual ~CIntelligentPointer(){}};
class CAIStateGroup_JackOfBladesStage1:public CAIStateGroup_JackOfBladesBase{public:bool HasAttacked;long ZigZagCounter; CIntelligentPointer<CThing> PObstruction,PShieldThing;CAIStateGroup_JackOfBladesStage1();};
CAIStateGroup_JackOfBladesStage1::CAIStateGroup_JackOfBladesStage1(){}