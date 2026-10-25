// std::swap(CTestQuest& a, CTestQuest& b) -- exchange two test-quest records (six names + colour index) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006232F0, 99 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> esi), plain
//   `ret`; 0x1c-byte frame temporary.
// Identity: the element copy 0x004A89D0 and assignment 0x004A8F50 are the landed CTestQuest copy constructor and
//   operator= (Ego_r.pdb CTestQuest, 0x1c bytes: QuestName +0, HSPName +4, long ColourIndex +8, DisplayName +0xc, IniFile
//   +0x10, OutiFile +0x14, QuestCardDefName +0x18); the TU sorts CTestQuest with CTestQuestNameCompare (landed
//   operator() 0x006238A0). Caller: the out-of-line iter_swap 0x00623860 (a bare `jmp`). The catalog's
//   "swap<CTCBase::CRegisteredTCInfo>" is propagated.
// Callees: CTestQuest copy ctor 0x004A89D0, CTestQuest::operator= 0x004A8F50 (both out of line), ~CCharString 0x0099EAE0
//   (six times: the temporary's implicit destructor, last member first; ColourIndex at +8 has none).
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }`; CTestQuest's copy and assignment are out-of-line
//   functions (as in the landed rows), its destructor the implicit inline one.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CTestQuest {                                      // Ego_r.pdb, 0x1c bytes, no vtable
public:
    CCharString QuestName;                              // +0x00
    CCharString HSPName;                                // +0x04
    long        ColourIndex;                            // +0x08
    CCharString DisplayName;                            // +0x0c
    CCharString IniFile;                                // +0x10
    CCharString OutiFile;                               // +0x14
    CCharString QuestCardDefName;                       // +0x18
    CTestQuest(const CTestQuest& other);                // 0x004A89D0
    CTestQuest& operator=(const CTestQuest& other);     // 0x004A8F50
};

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<CTestQuest>(CTestQuest&, CTestQuest&);
}