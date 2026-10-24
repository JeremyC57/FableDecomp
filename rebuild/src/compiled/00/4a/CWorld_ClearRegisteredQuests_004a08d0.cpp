// CWorld::ClearRegisteredQuests() -- empty the world's registered-quest, initial-active-quest and active-test-quest lists (STLport clear() = erase(begin(), end()) on each)
// Leaf: ClearRegisteredQuests
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A08D0, 112 bytes, plain `ret` (__fastcall, this -> esi; the `push ecx`
//   frame word holds the two empty tag temporaries in its top byte). PDB name ?ClearRegisteredQuests@CWorld@@QAEXXZ.
// Callees: 0x0043336A vector<CCharString>::erase(iterator, iterator) (out of line, __fastcall; twice), 0x004AA580
//   __copy<CTestQuest*> (__fastcall first/last, then result, the random_access_iterator_tag reference and the null
//   distance-type pointer; ret 0Ch), 0x004ABD90 __destroy_aux<CTestQuest*> (__fastcall first/last + the __false_type
//   reference).
// Caller: CWorld::LoadQuestsFromFile 0x004A0D90 (`if (clear) ClearRegisteredQuests();`).
// Layout (retail, STLport 12-byte vectors): InitialActiveQuests CArray<CCharString> +0xAC, RegisteredQuests
//   vector<CCharString> +0xB8, ActiveTestQuests vector<CTestQuest> +0xC4. The generated engine/CWorld.h carries the
//   Ego_r.pdb offsets of the debug build's 16-byte Dinkumware vectors (+0xAC / +0xBC / +0xCC), so the three lists are
//   modelled locally at their retail offsets. CTestQuest = Ego_r.pdb 28-byte record (QuestName, HSPName, ColourIndex,
//   DisplayName, IniFile, OutiFile, QuestCardDefName); the 0x004AA580 copy's operator= / dword pattern matches it (its
//   propagated `pair<CCharString, CPersistInfo>` label is a byte-identical twin).
// Shape: `RegisteredQuests.clear(); InitialActiveQuests.clear(); ActiveTestQuests.clear();` -- the CCharString
//   vectors call their out-of-line erase, the CTestQuest vector's STLport erase is inlined (`pointer i =
//   __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0); _Destroy(i, _M_finish); _M_finish = i;`
//   with `_Destroy` = `__destroy_aux(first, last, __false_type())`); the empty tags need user constructors.
struct CCharString {
    void* PStringData;
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct CTestQuest {
    CCharString QuestName;                          // +0x00
    CCharString HSPName;                            // +0x04
    long        ColourIndex;                        // +0x08
    CCharString DisplayName;                        // +0x0C
    CCharString IniFile;                            // +0x10
    CCharString OutiFile;                           // +0x14
    CCharString QuestCardDefName;                   // +0x18
};

CTestQuest* __fastcall __copy(CTestQuest* first, CTestQuest* last, CTestQuest* result,
                              const random_access_iterator_tag&, int*);             // 0x004AA580
void __fastcall __destroy_aux(CTestQuest* first, CTestQuest* last, const __false_type&);   // 0x004ABD90

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* first, CCharString* last);                       // 0x0043336A
    void clear() { erase(_M_start, _M_finish); }
};

template <class T> struct CArray : CCharStringVector { };

struct CTestQuestVector {
    CTestQuest* _M_start;
    CTestQuest* _M_finish;
    CTestQuest* _M_end_of_storage;
    CTestQuest* erase(CTestQuest* __first, CTestQuest* __last)
    {
        CTestQuest* __i = __copy(__last, _M_finish, __first, random_access_iterator_tag(), (int*)0);
        __destroy_aux(__i, _M_finish, __false_type());
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(_M_start, _M_finish); }
};

struct CWorld_RetailLayout {
    unsigned char           _pad_0x00[0xAC];
    CArray<CCharString>     InitialActiveQuests;    // +0xAC
    CCharStringVector       RegisteredQuests;       // +0xB8
    CTestQuestVector        ActiveTestQuests;       // +0xC4
    void ClearRegisteredQuests();
};

void CWorld_RetailLayout::ClearRegisteredQuests()
{
    RegisteredQuests.clear();
    InitialActiveQuests.clear();
    ActiveTestQuests.clear();
}