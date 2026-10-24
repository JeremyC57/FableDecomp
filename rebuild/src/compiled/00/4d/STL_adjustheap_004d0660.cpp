// _STL::__adjust_heap(SCameraTagEntry* first, int holeIndex, int len, SCameraTagEntry val, CKeyPairCompareLess comp) -- STLport heap sift-down over the CSubtitledMessage camera-tag vector map (pair<float, CCameraTag>, float key, less): walk the hole down to a leaf through the larger child, then __push_heap the value back up from there
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0660, 190 bytes, `ret 24h`.
// Callers: 0x004D0BBA / 0x004D0CFE (make_heap / pop_heap of the same element) and 0x004D176A (sort_heap).
// Callee: __push_heap 0x004CFEB0 (`__fastcall`: first ecx, hole edx, then top, the 28-byte value and the comparator on
//   the stack, `ret 24h`; its catalog label `_Push_heap<CDegradableInfo*,...,CIsHealth1GTHealth2>` is propagated -- its
//   compare is the same `fld [parent] ; fcomp value ; test ah,5 ; jp` less-than).
// Label correction: the catalog's `_Adjust_heap<CDebugBox*,int,CDebugBox>` is propagated (CDebugBox is 84 bytes in
//   Ego_r.pdb); the element is the 28-byte pair<float, CSubtitledMessage::CCameraTag> of the landed
//   CCameraTagVector::_M_insert_overflow 0x004D1360 (same TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, holeIndex in edx; len, the value and the empty
//   comparator on the stack (4 + 28 + 4 = 0x24). `topIndex` lives in the `push ecx` frame word.
// Shape: the STLport `__adjust_heap` text (`secondChild = 2 * holeIndex + 2`, the child compare
//   `comp(*(first + secondChild), *(first + (secondChild - 1)))`, `2 * (secondChild + 1)`, the single-child tail);
//   element moves are the implicit pair operator= (`rep movsd` x7), the by-value argument to __push_heap is built in the
//   argument area through pair's memberwise copy constructor (key dword, then the CCameraTag words through eax).
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

void __fastcall __push_heap(SCameraTagEntry* first, int holeIndex, int topIndex, SCameraTagEntry val,
                            CKeyPairCompareLess comp);   // 0x004CFEB0

void __fastcall __adjust_heap(SCameraTagEntry* first, int holeIndex, int len, SCameraTagEntry val,
                              CKeyPairCompareLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

}