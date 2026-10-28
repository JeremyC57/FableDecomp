// CFrontendGameComponent::ChooseWorldNumber(long number) -- pick the number-th listed world (1-based, paged) to load
// Retail 0x00494710, 87 bytes (Ego_r.pdb ?ChooseWorldNumber@CFrontendGameComponent@@AAEXJ@Z). The index is
// WorldNamesOffset + number - 1, compared unsigned against WorldNames.size(); a valid pick sets
// WorldToLoad = WorldNames[index] + L".wld" (rdata 0x01238508) and asks the component to quit through the
// virtual SetQuit() (vtable 0x01238768 slot 4, body 0x00494370 `mov byte [ecx+8],1`) so the frontend hands
// over to the game.
// Callees: CWideString operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (`__fastcall`: hidden result in
// ecx -- homed in the dead `number` slot -- left operand in edx, literal pushed), CWideString::operator= @
// 0x0099B7D0, ~CWideString @ 0x0099B510.
// Model: retail offsets. Ego_r.pdb puts WorldNames at +0x1E38, WorldNamesOffset +0x1E48 and WorldToLoad
// +0x1E4C; retail STLport vectors are 12 bytes (PDB 16) and SaveNames precedes WorldNames, so retail sits
// 8 bytes lower: WorldNames +0x1E30 (start/finish/end), WorldNamesOffset +0x1E3C, WorldToLoad +0x1E40.
// Leaf: ?ChooseWorldNumber@CFrontendGameComponent

typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

class CWideString
{
public:
    void* PData;                         // +0x00
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};

CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30

struct CWideStringVector                 // STLport vector<CWideString> (12 bytes)
{
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideString* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CWideString& operator[](unsigned long n) const { return *(begin() + n); }
};

class CFrontendGameComponent
{
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void SetQuit();                         // slot 4
    void ChooseWorldNumber(long number);

    unsigned char _members[0x1e30 - 4];             // +0x04..+0x1E2F
    CWideStringVector WorldNames;                   // +0x1E30
    long WorldNamesOffset;                          // +0x1E3C
    CWideString WorldToLoad;                        // +0x1E40
};

void CFrontendGameComponent::ChooseWorldNumber(long number)
{
    unsigned long index = WorldNamesOffset + number - 1;
    if (index < WorldNames.size())
    {
        WorldToLoad = WorldNames[index] + L".wld";
        SetQuit();
    }
}
