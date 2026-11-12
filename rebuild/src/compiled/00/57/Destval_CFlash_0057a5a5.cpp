// Leaf: ?Dtor@S@@QAEXXZ
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013B8B0C is the anonymous-namespace CCharString DEFAULT_DEATH_RECOVERY_MARKER_NAME
//   ("HERO_GUILD_RECOVERY_MARKER"; Ego_r.pdb + FableWin.pdb `dynamic initializer for '`anonymous namespace'::DEFAULT_DEATH_RECOVERY_MARKER_NAME'',
//   initializer 0x0121A7AD landed by agentFAM09); was g_glob (an int read as a CCharString). The stand-in S::Dtor is untouched.
class CCharString { public: void* Data; const CCharString& operator=(const CCharString& other); };   // 0x0099EFB0
namespace { extern CCharString DEFAULT_DEATH_RECOVERY_MARKER_NAME; }   // 0x013B8B0C
struct S { char pad[0x170]; void Dtor(); };
void S::Dtor(){ *(CCharString*)((char*)this+0x170) = DEFAULT_DEATH_RECOVERY_MARKER_NAME; }