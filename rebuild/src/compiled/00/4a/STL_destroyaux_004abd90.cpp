// _STL::__destroy_aux(CTestQuest* first, CTestQuest* last, const __false_type&) -- destroy a run of 28-byte test-quest records: each record's six CCharStrings, last member first
// Leaf: ??$__destroy_aux@PAVCTestQuest@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ABD90, 72 bytes (+8 int3), `ret 4` (link-time __fastcall: first in ecx, last in
//   edx, the __false_type tag reference on the stack, never read).
// Identity: the `_M_clear` destroy of CWorld's ActiveTestQuests vector<CTestQuest> (landed _M_insert_overflow 0x004ADB50
//   names this address as `__destroy_aux<CTestQuest*>`); other callers: CWorld::ClearRegisteredQuests 0x004A08D0 (landed),
//   CTCInventoryQuests::GetTestQuestsToDisplay / MoveToNextTestQuest / MoveToPreviousTestQuest (the copied quest lists).
// Callees: CCharString::~CCharString 0x0099EAE0 (six per record).
// Layout: Ego_r.pdb CTestQuest (28 bytes): QuestName +0, HSPName +4, ColourIndex +8 (long), DisplayName +0xC,
//   IniFile +0x10, OutiFile +0x14, QuestCardDefName +0x18 (the model of the landed 0x004ADB50).
// Shape: STLport 4.x `for (; first != last; ++first) _Destroy(&*first);` with CTestQuest's implicit destructor inlined
//   (members in reverse declaration order: +0x18 .. +0xC, +4, +0; the long has no destructor).
// Label correction: catalogued as `vector<std::pair<CCharString...>::_Destroy` (propagated); the body is STLport
//   `__destroy_aux<CTestQuest*>`.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};

class CTestQuest {                                   // Ego_r.pdb class (mangled V)
public:
    CCharString QuestName;                           // +0x00
    CCharString HSPName;                             // +0x04
    long        ColourIndex;                         // +0x08
    CCharString DisplayName;                         // +0x0C
    CCharString IniFile;                             // +0x10
    CCharString OutiFile;                            // +0x14
    CCharString QuestCardDefName;                    // +0x18
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __destroy_aux<CTestQuest*>(CTestQuest*, CTestQuest*, const __false_type&);
}