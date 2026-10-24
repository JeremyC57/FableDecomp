// _STL::__unguarded_insertion_sort_aux(SCameraTagEntry* first, SCameraTagEntry* last, SCameraTagEntry*, CKeyPairCompareLess comp) -- STLport unguarded insertion sort over the CSubtitledMessage camera-tag vector map (pair<float, CCameraTag>, ascending float key): every element from first to last is lifted out and sifted down with __unguarded_linear_insert
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0460, 132 bytes (+ int3 padding), `ret 8`.
// Callers: 0x004D0A50 (the out-of-line `__unguarded_insertion_sort(first, last, comp)` pushing comp and the null
//   value-type tag) and 0x004D1AE0 (`__final_insertion_sort`, the `first + 16` tail), both reached from the vector map's
//   Sort() at 0x004D2640 (`sort(begin, end, m_compare); m_dirty = false;`). No callees.
// Label correction: the catalog's `_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>` is propagated (CHeroMarriageInfo is 24
//   bytes in Ego_r.pdb, the element here is 28 with a float key). The element is the 28-byte
//   pair<float, CSubtitledMessage::CCameraTag> of the landed CCameraTagVector::_M_insert_overflow 0x004D1360 (same TU):
//   the 0x92492493 / sar 4 divide in the callers, `imul 1Ch`, the 7-dword `rep movsd` element moves.
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx; the null `_Tp*` tag and the empty
//   4-byte comparator on the stack.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the linear insert inlined
//   (`next = last - 1; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`). The value copy is
//   STLport pair's memberwise copy constructor (the float key as a dword, then the CCameraTag by its implicit copy
//   through `lea ecx,[ebx+20h]` = &i->second); the element assignments are the implicit pair operator= (`rep movsd`).
//   The comparator is CKeyPairCompareLess: `fld val.first ; fcomp [next] ; test ah,5 ; jp` = val.first < next->first.
namespace _STL {

enum EConversationProtagonist { PROTAGONIST_NONE };
enum EConversationCameraPosition { CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CAMERA_ZOOM_NONE };
enum EConversationCameraView { CAMERA_VIEW_NONE };

struct CCameraTag {                                    // CSubtitledMessage::CCameraTag (24 bytes, Ego_r.pdb)
    EConversationProtagonist    CameraPositionPerson;  // +0x00
    EConversationCameraPosition CameraPosition;        // +0x04
    EConversationProtagonist    CameraFocusPerson;     // +0x08
    EConversationCameraFocus    CameraFocusPosition;   // +0x0c
    EConversationCameraZoomType CameraZoomType;        // +0x10
    EConversationCameraView     CameraView;            // +0x14
};

struct SCameraTagEntry {                               // pair<float, CCameraTag> (28 bytes)
    float      first;                                  // +0x00
    CCameraTag second;                                 // +0x04
    SCameraTagEntry(const SCameraTagEntry& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<float, CCameraTag>
    bool operator()(const SCameraTagEntry& a, const SCameraTagEntry& b) const { return a.first < b.first; }
};

inline void __unguarded_linear_insert(SCameraTagEntry* last, SCameraTagEntry val, CKeyPairCompareLess comp)
{
    SCameraTagEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(SCameraTagEntry* first, SCameraTagEntry* last, SCameraTagEntry*,
                                               CKeyPairCompareLess comp)
{
    for (SCameraTagEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SCameraTagEntry(*i), comp);
}

}