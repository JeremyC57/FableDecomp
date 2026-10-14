// CRegisteredTCTable_004e6d8a::Register(const CCharString& name, const CRegisteredTCInfo& info) -- push a (name, info) pair onto the table and mark it changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6D8A, length 56 bytes.
// Callees: std::pair<CCharString, CTCBase::CRegisteredTCInfo>::pair(const CCharString&, const
// CRegisteredTCInfo&) @ 0x004D9D2F (out of line, builds the 16-byte temporary at [ebp-10h]),
// the retail vector push_back for 16-byte elements @ 0x004E40C3 (landed: PushBack_CElement16),
// CCharString::~CCharString @ 0x0099EAE0 (twice: the pair's inlined destructor destroys the info's
// string at +0x0c, then the name at +0).
// Shape: frame pointer, a 16-byte temporary, the arguments pushed straight from their slots
// (`push dword ptr [ebp+0Ch]`) but `this` kept in esi and the body ending in `leave`: the
// 0x004Exxxx def module with size optimisation and frame-pointer omission off
// (`#pragma optimize("y",off)` + `("s",on)`; the `gsy` form spills `this`, see GOTCHAS). The
// vector lives at +0 of `this` (push_back is called with ecx = this) and the "changed" flag is the
// byte at +0x0d. The manifest label (CBBBClientUser::OutputText) is a propagated name; the class
// is named by what the body does.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* m_pStringData;
    ~CCharString();                             // 0x0099EAE0
};

struct CRegisteredTCInfo {                      // 12 bytes
    long        m_type;                         // +0x00
    long        m_index;                        // +0x04
    CCharString m_name;                         // +0x08
};

struct SPair {                                  // std::pair<CCharString, CRegisteredTCInfo>, 16 bytes
    CCharString       first;                    // +0x00
    CRegisteredTCInfo second;                   // +0x04
    SPair(const CCharString& name, const CRegisteredTCInfo& info);   // 0x004D9D2F
};

struct CRetailVector_SPair {                    // VC6-era Dinkumware vector: first / last / end
    SPair* first;
    SPair* last;
    SPair* end;
    void push_back(const SPair& value);         // 0x004E40C3
};

struct CRegisteredTCTable_004e6d8a {
    CRetailVector_SPair m_entries;              // +0x00
    unsigned char       m_locked;               // +0x0c
    unsigned char       m_changed;              // +0x0d
    void Register(const CCharString& name, const CRegisteredTCInfo& info);
};

void CRegisteredTCTable_004e6d8a::Register(const CCharString& name, const CRegisteredTCInfo& info)
{
    m_entries.push_back(SPair(name, info));
    m_changed = 1;
}
