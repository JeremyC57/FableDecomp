// _global::UninitCopy_CTestQuest_004aa060(const CTestQuest* first, const CTestQuest* last, CTestQuest* result, const __false_type&) -- STLport __uninitialized_copy over 28-byte CTestQuest records: placement-copy each record (six strings through the out-of-line CCharString copy, the colour index inline), return the advanced result
// Leaf: UninitCopy_CTestQuest_004aa060
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA060, length 112 bytes, `ret 8`.
// Callers: the vector<CTestQuest> reserve/assign helper @ 0x004ADB50 (calls 0x004ADBAB / 0x004ADBF8).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (six times per element).
// Layout (Ego_r.pdb CTestQuest, 28 bytes -- the only PDB record with strings at 0/4/12/16/20/24 and a scalar at 8):
//   QuestName +0x00, HSPName +0x04, long ColourIndex +0x08, DisplayName +0x0C, IniFile +0x10, OutiFile +0x14,
//   QuestCardDefName +0x18.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack); first/last/cur in esi/ebx/edi across the calls,
//   placement new (null test) of the implicit memberwise copy constructor in member order (the ColourIndex word is
//   copied between the second and third string copies).
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, unsigned int*, ..>` is propagated.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);     // 0x0099EC30
    void* PData;                               // +0x0
};

struct CTestQuest {                            // Ego_r.pdb, 28 bytes
    CCharString QuestName;                     // +0x00
    CCharString HSPName;                       // +0x04
    long        ColourIndex;                   // +0x08
    CCharString DisplayName;                   // +0x0C
    CCharString IniFile;                       // +0x10
    CCharString OutiFile;                      // +0x14
    CCharString QuestCardDefName;              // +0x18
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CTestQuest* __fastcall UninitCopy_CTestQuest_004aa060(const CTestQuest* first, const CTestQuest* last, CTestQuest* result, const __false_type&)
{
    CTestQuest* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}