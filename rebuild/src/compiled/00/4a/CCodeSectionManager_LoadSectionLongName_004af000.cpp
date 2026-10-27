// CCodeSectionManager::LoadSectionLongName(CCharString longName) -- loads the code section registered under a long name (LoadSection(GetHandleFromLongName(name)))
// Leaf: ?LoadSectionLongName@CCodeSectionManager@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF000, 44 bytes, thiscall `ret 4` (the CCharString by value; the callee
//   destroys it).
// Identity: the callees are the landed CCodeSectionManager::GetHandleFromLongName(CCharString) 0x009F0500 and
//   CCodeSectionManager::LoadSection(unsigned long) 0x009F02C0 on `this`; Ego_r.pdb / FableWin.pdb name the matching
//   member ?LoadSectionLongName@CCodeSectionManager@@QAEXVCCharString@@@Z (UnloadSectionLongName pairs with UnloadSection).
//   The row is an out-of-line COMDAT with no direct caller (the xref.py hit 0x004EFADC decodes to no instruction).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, GetHandleFromLongName 0x009F0500, LoadSection
//   0x009F02C0, CCharString::~CCharString 0x0099EAE0.
// Shape: the by-value argument is copy-constructed in its own stack slot (`push ecx ; mov ecx,esp ; push &name ; call`);
//   the handle is pushed straight from eax; the function's own by-value parameter is destroyed last.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CCodeSectionManager {
public:
    unsigned long GetHandleFromLongName(CCharString longName);   // 0x009F0500
    void LoadSection(unsigned long handle);                      // 0x009F02C0
    void LoadSectionLongName(CCharString longName);
};

void CCodeSectionManager::LoadSectionLongName(CCharString longName)
{
    LoadSection(GetHandleFromLongName(longName));
}