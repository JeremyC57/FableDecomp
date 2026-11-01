// CUserProfileManager::~CUserProfileManager() -- probe
// Leaf: ??1CUserProfileManager@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
namespace std {
struct __false_type { __false_type() {} };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Deque_iterator_base {
    typedef _Tp value_type;
    _Tp* _M_cur; _Tp* _M_first; _Tp* _M_last; _Tp** _M_node;
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_iterator(const iterator& __x) : _Deque_iterator_base<_Tp>(__x) {}
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00410D80
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp, class _Traits>
inline void _Destroy(_Deque_iterator<_Tp, _Traits> __first, _Deque_iterator<_Tp, _Traits> __last)
{
    __destroy(__first, __last, (_Tp*)0);
}
template <class _Tp, class _Alloc>
class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;
    iterator _M_finish;
    _Tp** _M_map;
    unsigned int _M_map_size;
protected:
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);   // 0x0040E130
    void _M_deallocate_map(_Tp** __p, unsigned int) { if (__p) free(__p); }
public:
    ~_Deque_base()
    {
        if (_M_map) {
            _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
            _M_deallocate_map(_M_map, _M_map_size);
        }
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    ~deque() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp, class _Sequence = deque<_Tp> >
class queue {
protected:
    _Sequence c;
};
} // namespace std

class CWideStringData;
class CWideString { public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};
class CDisplayManager;
class CDeviceResetCallback { public:
    CDeviceResetCallback();
protected:
    virtual void OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CDisplayManager { public:
    void RemoveDeviceResetCallback(CDeviceResetCallback* callback);   // 0x009BF5A0
};
class CInitBaseClass : public CBaseClass { public:
    bool Valid;                                             // +0x04
};
class CSystemManager : public CInitBaseClass { public:
    unsigned char                    _pad_0x08[0x60 - 0x08];
    CCountedPointer<CDisplayManager> PDisplayManager;       // +0x60
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0

enum EGameAction { };
enum EControllerType { };
enum EInputKey { };
enum EXboxControllerButton { };
enum EMouseButtonControl { };
enum ETutorialCategory { };
class C2DVector { public: float X, Y; };
class C2DExtentsI { public: long X, Y; };
class CActionInputControl { public:
    EGameAction           GameAction;                       // +0x00
    EControllerType       ControllerType;                   // +0x04
    EInputKey             KeyboardKey;                      // +0x08
    EXboxControllerButton XboxButton;                       // +0x0C
    EMouseButtonControl   MouseButton;                      // +0x10
    C2DVector             ControlDirection;                 // +0x14
};
class CDisplayModeDesc { public:
    C2DExtentsI Dimensions;                                 // +0x00
    long        BitsPerPixel;                               // +0x08
    long        RefreshRate;                                // +0x0C
};
class CTexture;
class CUserProfileManager : public CBaseClassNonCopyable, public CDeviceResetCallback {   // vtables 0x0122E23C / 0x0122E230
public:
    CUserProfileManager();
    virtual ~CUserProfileManager();
protected:
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
public:
    CWideString   ProfileName;                              // +0x08
    bool          UseNewProfileValues;                      // +0x0C
    bool          SubtitlingEnabled;                        // +0x0D
    bool          VibrationEnabled;                         // +0x0E
    bool          CameraXYInverted;                         // +0x0F
    bool          CameraYZInverted;                         // +0x10
    bool          CameraFPBowXYInverted;                    // +0x11
    bool          CameraFPBowYZInverted;                    // +0x12
    bool          CameraPCBowXYInverted;                    // +0x13
    bool          CameraPCBowYZInverted;                    // +0x14
    bool          CameraPCViewHeroXYInverted;               // +0x15
    bool          CameraPCViewHeroYZInverted;               // +0x16
    bool          TutorialSystemEnabled;                    // +0x17
    bool          GuildMasterEnabled;                       // +0x18
    bool          HUDTooltipEnabled;                        // +0x19
    float         Gamma;                                    // +0x1C
    float         HUDAlpha;                                 // +0x20
    float         SoundVolume;                              // +0x24
    float         MusicVolume;                              // +0x28
    float         DialogueVolume;                           // +0x2C
    bool          UsingHeadPhones;                          // +0x30
    bool          SoundsMuted;                              // +0x31
    bool          ShowingBuddyNames;                        // +0x32
    bool          ApplyVideoValuesOnLoad;                   // +0x33
    bool          DisplayDialogOnOptionsChanged;            // +0x34
    unsigned long Antialiasing;                             // +0x38
    float         TextureDetail;                            // +0x3C
    float         ShadowDetail;                             // +0x40
    float         MeshDetail;                               // +0x44
    unsigned long Resolution;                               // +0x48
    unsigned long RefreshRate;                              // +0x4C
    bool          VerticalSyncEnabled;                      // +0x50
    bool          StereoReversed;                           // +0x51
    std::vector<CActionInputControl> AssignedInputsPassive;      // +0x54
    std::vector<CActionInputControl> AssignedInputsAggressive;   // +0x60
    bool          ShowTargetingStatus;                      // +0x6C
    float         EffectsDetail;                            // +0x70
    unsigned long ScreenAspectRatio;                        // +0x74
    bool          NewProfileToUseWASD;                      // +0x78
    bool          CameraResetting;                          // +0x79
    float         CameraSensitivity;                        // +0x7C
    std::vector<CDisplayModeDesc>    DisplayModes;          // +0x80
    std::vector<ETutorialCategory>   GivenTutorialCategories;     // +0x8C
    std::queue<ETutorialCategory>    PendingTutorialCategories;   // +0x98
    std::vector<CWideString>         SaveGameNames;         // +0xC0
    bool          DisplayTutorialsDuringCutscenes;          // +0xCC
    bool          SubtitlingEnabledNewProfile;              // +0xCD
    bool          VibrationEnabledNewProfile;               // +0xCE
    bool          CameraXYInvertedNewProfile;               // +0xCF
    bool          CameraYZInvertedNewProfile;               // +0xD0
    bool          CameraFPBowXYInvertedNewProfile;          // +0xD1
    bool          CameraFPBowYZInvertedNewProfile;          // +0xD2
    bool          CameraPCBowXYInvertedNewProfile;          // +0xD3
    bool          CameraPCBowYZInvertedNewProfile;          // +0xD4
    bool          CameraPCViewHeroXYInvertedNewProfile;     // +0xD5
    bool          CameraPCViewHeroYZInvertedNewProfile;     // +0xD6
    bool          TutorialSystemEnabledNewProfile;          // +0xD7
    bool          GuildMasterEnabledNewProfile;             // +0xD8
    bool          HUDTooltipEnabledNewProfile;              // +0xD9
    float         GammaNewProfile;                          // +0xDC
    float         HUDAlphaNewProfile;                       // +0xE0
    float         SoundVolumeNewProfile;                    // +0xE4
    float         MusicVolumeNewProfile;                    // +0xE8
    float         DialogueVolumeNewProfile;                 // +0xEC
    bool          UsingHeadPhonesNewProfile;                // +0xF0
    bool          ShowingBuddyNamesNewProfile;              // +0xF1
    unsigned long AntialiasingNewProfile;                   // +0xF4
    float         TextureDetailNewProfile;                  // +0xF8
    float         ShadowDetailNewProfile;                   // +0xFC
    float         MeshDetailNewProfile;                     // +0x100
    unsigned long ResolutionNewProfile;                     // +0x104
    unsigned long RefreshRateNewProfile;                    // +0x108
    bool          VerticalSyncEnabledNewProfile;            // +0x10C
    bool          StereoReversedNewProfile;                 // +0x10D
    CWideString   ScoreBoardUserName;                       // +0x110
    CWideString   ScoreBoardPassword;                       // +0x114
    CWideString   ScoreBoardProxyIp;                        // +0x118
    long          ScoreBoardProxyPort;                      // +0x11C
    long          ScoreBoardServerPort;                     // +0x120
    float         EffectsDetailNewProfile;                  // +0x124
    unsigned long ScreenAspectRatioNewProfile;              // +0x128
    bool          CameraResettingNewProfile;                // +0x12C
    float         CameraSensitivityNewProfile;              // +0x130
    bool          ShowTargetingStatusNewProfile;            // +0x134
    CTexture*     PBackBufferTexture;                       // +0x138
    bool          VideoChangesAccepted;                     // +0x13C
};
FABLE_STATIC_ASSERT(sizeof(CUserProfileManager) == 0x140);

CUserProfileManager::~CUserProfileManager()
{
    GFGetSystemManager()->GetDisplayManager()->RemoveDeviceResetCallback(this);
}
