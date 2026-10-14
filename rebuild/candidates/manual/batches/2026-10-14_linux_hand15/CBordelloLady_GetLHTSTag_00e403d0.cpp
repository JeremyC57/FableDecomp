// CBordelloLady::GetLHTSTag(const CCharString& suffix) -- build the lady's LHTS tag: prefix + Name + separator + suffix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E403D0, length 79 bytes (+ int3 padding).
// Callees (all __fastcall with the hidden result in ecx): operator+(const char*, const CCharString&)
// @ 0x0099F690, operator+(const CCharString&, const char*) @ 0x0099F600, operator+(const CCharString&,
// const CCharString&) @ 0x0099F570; CCharString::~CCharString @ 0x0099EAE0 (x2).
// Globals: the two string literals @ 0x012E4430 (prefix, passed in edx) and @ 0x01244DB4 (separator).
// Layout: Name is the CCharString at +0x28 (Ego_r.pdb CBordelloLady: Name +40).
// Shape: one frame slot (`push ecx`) holds the first temporary; the second temporary is homed in
// the dead argument slot of `suffix` (its value was loaded into eax and pushed before the first
// call); the final concatenation is built straight into the hidden return slot (no NRVO in VC7.1,
// but a returned temporary is constructed in place), and the two temporaries are destroyed in
// reverse order before `this`'s result pointer is returned.
struct CCharString {
    void* m_pStringData;
    ~CCharString();                             // 0x0099EAE0
};

CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

struct CBordelloLady {
    unsigned char _pad[0x28];                   // +0x00..+0x27 (script base, parent, master data, flags)
    CCharString   Name;                         // +0x28
    CCharString GetLHTSTag(const CCharString& suffix);
};

CCharString CBordelloLady::GetLHTSTag(const CCharString& suffix)
{
    return "LHTS_" + Name + "_" + suffix;
}
