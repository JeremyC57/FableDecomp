// std::swap<pair<float, CSubtitledMessage::CCameraTag> >(pair& a, pair& b) -- STLport: exchange two 28-byte camera-tag entries through a temporary
// Leaf: ??$swap@U?$pair@MVCCameraTag@CSubtitledMessage@@@std@@@std@@YIXAAU?$pair@MVCCameraTag@CSubtitledMessage@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CF450, length 85 bytes, __fastcall (/Gr TU: ecx = a, edx = b), plain `ret`.
// Identity: the 28-byte value is CSubtitledMessage's CameraTags element pair<float, CCameraTag> (Ego_r.pdb: the only
// 28-byte pair<float, ...>; CCameraTag = six enums, 24 bytes) -- the neighbouring rows of this TU are CSubtitledMessage's
// (0x004CDB10 Initialise, 0x004CDB70.., 0x004CE4E0 AddCameraTagFromIndex). No callers (an emitted COMDAT); 0x004CFC50
// (chunk 372) is the byte-identical iter_swap twin. The catalog "swap<CDebugSphere>" is propagated (CDebugSphere is
// 76 bytes).
// Shape: STLport `_Tp __tmp = a; a = b; b = __tmp;` -- the temporary is built by the STLport pair's user copy ctor
// (`first(o.first), second(o.second)`: one dword, then the six CCameraTag dwords through a pointer), the two
// assignments are the implicit bitwise operator= (`rep movsd`, 7 dwords).
enum EConversationProtagonist { ECP_0 };
enum EConversationCameraPosition { ECCP_0 };
enum EConversationCameraFocus { ECCF_0 };
enum EConversationCameraZoomType { ECCZ_0 };
enum EConversationCameraView { ECCV_0 };

class CSubtitledMessage {
public:
    class CCameraTag {                                       // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist    CameraPositionPerson;    // +0x00
        EConversationCameraPosition CameraPosition;          // +0x04
        EConversationProtagonist    CameraFocusPerson;       // +0x08
        EConversationCameraFocus    CameraFocusPosition;     // +0x0c
        EConversationCameraZoomType CameraZoomType;          // +0x10
        EConversationCameraView     CameraView;              // +0x14
    };
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class _Tp> void __fastcall swap(_Tp& a, _Tp& b)
{
    _Tp tmp = a;
    a = b;
    b = tmp;
}

template void __fastcall swap<pair<float, CSubtitledMessage::CCameraTag> >(pair<float, CSubtitledMessage::CCameraTag>&,
                                                                          pair<float, CSubtitledMessage::CCameraTag>&);
}