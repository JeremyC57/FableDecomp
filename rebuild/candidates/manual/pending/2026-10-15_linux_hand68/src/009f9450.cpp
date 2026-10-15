// CProfile::CProfile(const CProfile&) -- memberwise copy of a profiler node: manager, timings, flags, the wide-string name through its copy constructor and the counters
// Leaf: ??0CProfile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9450, length 91 bytes.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (landed). Ego_r.pdb ??0CProfile@@QAE@ABV0@@Z.
// Layout (Ego_r.pdb CProfile, 0x48 bytes = retail): Manager +0, StartTime +8, TotalTime +0x10, LastTime +0x18,
// Running +0x20, Percentage +0x28, Name +0x30, TreeDepth +0x34, NewNode +0x38, Visible +0x3c,
// CallCounter +0x40, UpdatedCallCounter +0x44.
// Shape: the copy in declaration order; doubles through fld/fstp, the name's address pushed early.
struct CWideStringData;
struct CWideString {
    CWideStringData* m_pStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
};

struct CProfileManager;

struct CProfile {
    CProfileManager* Manager;                               // +0x00
    double           StartTime;                             // +0x08
    double           TotalTime;                             // +0x10
    double           LastTime;                              // +0x18
    bool             Running;                               // +0x20
    double           Percentage;                            // +0x28
    CWideString      Name;                                  // +0x30
    long             TreeDepth;                             // +0x34
    bool             NewNode;                               // +0x38
    long             Visible;                               // +0x3c
    long             CallCounter;                           // +0x40
    long             UpdatedCallCounter;                    // +0x44
    CProfile(const CProfile& other);
};

CProfile::CProfile(const CProfile& other)
    : Manager(other.Manager), StartTime(other.StartTime), TotalTime(other.TotalTime),
      LastTime(other.LastTime), Running(other.Running), Percentage(other.Percentage),
      Name(other.Name), TreeDepth(other.TreeDepth), NewNode(other.NewNode),
      Visible(other.Visible), CallCounter(other.CallCounter),
      UpdatedCallCounter(other.UpdatedCallCounter)
{
}
