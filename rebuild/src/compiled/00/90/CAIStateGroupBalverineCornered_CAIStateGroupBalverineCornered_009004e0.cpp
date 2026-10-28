// CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered() -- construct the state group and its two tracked things.
// Flags: /Os
// Leaf: ??0CAIStateGroup_BalverineCornered@@QAE@XZ
// Identity: proven; own RTTI 0128C0BC and exact PDB default ctor; original CAIStateGroupBase chain; tracked members PTarget/PLastJumpTarget at20/28 and own template vtables.
// Layout: generated state-group headers flatten CBaseClass polymorphism and tracked members, so local original inheritance and typed members retain real virtual construction. Base20 unchanged; Jack base76 after vector contraction, unaccessed base tail modeled as an external-constructor prefix. Stage1 primitives intentionally uninitialized. No CThing/creature object fields or sizes used.
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{SCRIPT_PRIORITY_DEFAULT};
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;CAIStateGroupBase();};template<class T>class CAIStateGroup_WorkBase:public T{};
class CAIStateGroup_JackOfBladesBase:public CAIStateGroupBase{public:unsigned char _unmodeled14[0x4c-0x14];CAIStateGroup_JackOfBladesBase();};
class CBaseObject;class CBaseObjectPointer;class CThing;class CThingCreatureBase;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer():CBaseIntelligentPointer(){}virtual ~CIntelligentPointer(){}};
class CAIStateGroup_BalverineCornered:public CAIStateGroupBase{public: CIntelligentPointer<CThingCreatureBase> PTarget,PLastJumpTarget;CAIStateGroup_BalverineCornered();};
CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered(){}