// CProfile::CProfile() -- initialize the leading value and default-construct the class member.
// Leaf: ??0CProfile@@QAE@XZ
// Identity: probable; PDB CProfile72-byte layout uniquely has Manager+0 and CWideString Name+48, matching own store0/call CWideString ctor0x99AED0; exact default-ctor PDB name exists; body has no references.
class CWideStringData;
class CWideString { public: CWideStringData* PStringData; CWideString(); };
class CProfileManager;
class CProfile { public: CProfileManager* Manager; double StartTime,TotalTime,LastTime; bool Running; double Percentage; CWideString Name; long TreeDepth; bool NewNode; long Visible,CallCounter,UpdatedCallCounter; CProfile(); };
CProfile::CProfile() : Manager(0) {}