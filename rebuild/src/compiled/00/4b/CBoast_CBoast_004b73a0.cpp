// CBoast::CBoast(const CBoast& other) -- memberwise copy: id/cost/reward, the script name, the two wide strings, three flags, the group index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B73A0, length 91 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CWideString::CWideString(const CWideString&) @ 0x0099B720 (x2).
// Layout from Ego_r.pdb (CBoast, 32 bytes): ID +0, Cost +4, Reward +8, ScriptName +0xc, Text +0x10,
// Title +0x14, Evil +0x18, Failed +0x19, Completed +0x1a, GroupIndex +0x1c. The copy constructor is
// written out of line (an implicit one would be inlined away and the TU would carry no symbol).
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);      // 0x0099B720
};

struct CBoast {
    long        ID;                             // +0x00
    long        Cost;                           // +0x04
    long        Reward;                         // +0x08
    CCharString ScriptName;                     // +0x0c
    CWideString Text;                           // +0x10
    CWideString Title;                          // +0x14
    bool        Evil;                           // +0x18
    bool        Failed;                         // +0x19
    bool        Completed;                      // +0x1a
    long        GroupIndex;                     // +0x1c
    CBoast(const CBoast& other);
};

CBoast::CBoast(const CBoast& other)
    : ID(other.ID), Cost(other.Cost), Reward(other.Reward), ScriptName(other.ScriptName)
    , Text(other.Text), Title(other.Title), Evil(other.Evil), Failed(other.Failed)
    , Completed(other.Completed), GroupIndex(other.GroupIndex)
{
}