// CUserProfileManager::CUserProfileManager() -- build the profile manager: CBase + CDeviceResetCallback bases, empty containers, 50 empty save-game names, then collect the distinct display modes (size + depth) at or above the configured minimum resolution, set the default and new-profile values, name the profile "Default" and register for device resets
// Grade: DIFFER(981v977) -- frame slots and register naming only: the 50-name fill homes its CWideString() temporary at [esp+20h] and the reference at [esp+18h] (retail 18h/1Ch, the ref slot later shared with the defaults pointer); the defaults release compiles to `mov edi,[eax+4] ; dec edi ; cmp edx,ebx` instead of `dec dword ptr [ecx+4] ; jne`; GFGetSystemManager()->PDisplayManager loads straight into ecx before the modes push (retail `mov eax,[eax+60h]` ... `mov ecx,eax` after it, i.e. an inline operator-> call); the inline CDisplayModeDesc copies load their fields in another register order. The real STLport _Vector_base/_STLP_alloc_proxy/_Deque_base structure, the C2DExtentsI user copy ctor, the field-wise compares and the in-loop-inlined / post-loop-out-of-line _M_insert_overflow split all agree.
// Leaf: ??0CUserProfileManager@@QAE@XZ
// Tried: (1) plain vector model + C2DExtentsI without copy ctor (994v977); (2) + user copy ctor, `ProfileName = L"Default"` implicit temp, `if (--IVRefCount == 0)` (996v977); (3) + real STLport bases (alloc proxies, deque base), explicit field compares, raw display pointer (981v977, this file); (4) + CCountedPointer operator-> and a named `system` local for AddDeviceResetCallback (fixes that call), height-first defaults (swaps ecx/edx: worse), non-const def pointer (release unchanged) (989v977).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040CEC0, 977 bytes (row 992 incl. int3 padding), thiscall, no EH frame.
// Callees: CBase::CBase() 0x0099A2F0, CWideString() 0x0099AED0 / (const wchar_t*) 0x0099B6B0 / copy 0x0099B720 / operator=
//   0x0099B7D0 / dtor 0x0099B510, deque<ETutorialCategory>::_M_initialize_map 0x0040F4A0, malloc 0x00BFEA0E, free 0x00BFEA14,
//   CUserProfileManager::GetConfigOptionsDefaultsDef() const 0x004083E0 (manifest "GetIconsDef" is propagated), GFGetSystemManager()
//   0x009A4EC0, CDisplayManager::EnumerateDisplayModes 0x009C02D0, __uninitialized_copy<CDisplayModeDesc*> 0x0040DDC0 (fastcall,
//   manifest "_Copy_impl<C2DLineF*>"), vector<CDisplayModeDesc>::_M_insert_overflow 0x004124C0, SetDefaultValues 0x0040C790,
//   SetDefaultValuesForNewProfile 0x00409B70, CDisplayManager::AddDeviceResetCallback 0x009C0190.
// Layout: Ego_r.pdb CUserProfileManager with 12-byte STLport vectors (PDB 16) and a 0x28-byte STLport deque under the queue (PDB 24):
//   ProfileName +0x08, UseNewProfileValues +0x0C, SoundsMuted +0x31, ApplyVideoValuesOnLoad +0x33, DisplayDialogOnOptionsChanged +0x34,
//   AssignedInputsPassive +0x54 / Aggressive +0x60, DisplayModes +0x80, GivenTutorialCategories +0x8C, PendingTutorialCategories +0x98,
//   SaveGameNames +0xC0, DisplayTutorialsDuringCutscenes +0xCC, ScoreBoardUserName/Password/ProxyIp +0x110/+0x114/+0x118,
//   PBackBufferTexture +0x138, VideoChangesAccepted +0x13C (size 0x140). CConfigOptionsDefaultsDef MinResolutionWidth/Height +0x4C/+0x50
//   (PDB +0x50/+0x54). CSystemManager PDisplayManager +0x60.
typedef unsigned short wchar16;
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CBase {
public:
    virtual ~CBase();                                      // 0x0099A300
    CBase();                                               // 0x0099A2F0
};

class CDeviceResetCallback {
public:
    virtual void OnPreDeviceReset() = 0;                   // slot 0
    virtual void OnPostDeviceReset() = 0;                  // slot 1
};

class CWideString {
public:
    void* PStringData;
    CWideString();                                         // 0x0099AED0
    CWideString(const wchar16* text);                      // 0x0099B6B0
    CWideString(const CWideString& other);                 // 0x0099B720
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
    ~CWideString();                                        // 0x0099B510
};

class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI() { }
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};

class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;                                // +0x00
    long        BitsPerPixel;                              // +0x08
    long        RefreshRate;                               // +0x0C
    CDisplayModeDesc() { }
};

class CActionInputControl {                                // Ego_r.pdb, 0x1C bytes
public:
    long  GameAction;                                      // +0x00 EGameAction
    long  ControllerType;                                  // +0x04 EControllerType
    long  KeyboardKey;                                     // +0x08 EInputKey
    long  XboxButton;                                      // +0x0C EXboxControllerButton
    long  MouseButton;                                     // +0x10 EMouseButtonControl
    float ControlDirection[2];                             // +0x14 C2DVector
};
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp1, class _Tp2> inline allocator<_Tp2> __stl_alloc_create(const allocator<_Tp1>&, const _Tp2*) { return allocator<_Tp2>(); }

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x0040DDC0

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _AllocProxy;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp*        _M_start;
    _Tp*        _M_finish;
    _AllocProxy _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef _Alloc allocator_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;

    explicit vector(const allocator_type& __a = allocator_type()) : _Base(__a) { }
    explicit vector(size_type __n) : _Base(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference back() { return *(end() - 1); }
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);

        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Tp> struct _Deque_iterator_base {
    _Tp*  _M_cur;
    _Tp*  _M_first;
    _Tp*  _M_last;
    _Tp** _M_node;
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
};
template <class _Tp> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    _Deque_iterator() { }
};

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp> iterator;
    typedef allocator<_Tp*> _Map_alloc_type;
    _Deque_base(const _Alloc& __a, unsigned int __num_elements)
        : _M_start(), _M_finish(), _M_map(__stl_alloc_create(__a, (_Tp**)0), 0), _M_map_size(__a, (unsigned int)0)
    {
        _M_initialize_map(__num_elements);
    }
    void _M_initialize_map(unsigned int __num_elements);   // 0x0040F4A0
    iterator _M_start;
    iterator _M_finish;
    _STLP_alloc_proxy<_Tp**, _Tp*, _Map_alloc_type> _M_map;
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class deque : public _Deque_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit deque(const allocator_type& __a = allocator_type()) : _Deque_base<_Tp, _Alloc>(__a, 0) { }
};

template <class _Tp, class _Sequence = deque<_Tp> > class queue {
public:
    _Sequence c;
    queue() : c() { }
};

}   // namespace std

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                           // slot 1
    mutable unsigned long IVRefCount;                      // +0x04
    void RemoveReference() const { if (--IVRefCount == 0) Delete(); }
};

class CConfigOptionsDefaultsDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x4C - 0x08];
    long          MinResolutionWidth;                      // +0x4C (PDB +0x50)
    long          MinResolutionHeight;                     // +0x50 (PDB +0x54)
};

template <class T> class CDefPointer {
public:
    T* PObject;
    CDefPointer() : PObject(0) { }
    CDefPointer(const CDefPointer& p) : PObject(p.PObject) { if (PObject) ++PObject->IVRefCount; }
    ~CDefPointer() { if (PObject) PObject->RemoveReference(); }
    T* operator->() const { return PObject; }
};

class CDisplayManager {
public:
    void EnumerateDisplayModes(std::vector<CDisplayModeDesc>& modes, C2DExtentsI minSize, C2DExtentsI maxSize);   // 0x009C02D0
    void AddDeviceResetCallback(CDeviceResetCallback* callback);                                                    // 0x009C0190
};

class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;                      // +0x60 (CCountedPointer<CDisplayManager>::Data)
    void*            PDisplayManagerInfo;                  // +0x64
};
CSystemManager* __fastcall GFGetSystemManager();           // 0x009A4EC0

class CTexture;

class CUserProfileManager : public CBase, public CDeviceResetCallback {
public:
    CWideString   ProfileName;                             // +0x08
    bool          UseNewProfileValues;                     // +0x0C
    bool          SubtitlingEnabled;
    bool          VibrationEnabled;
    bool          CameraXYInverted;
    bool          CameraYZInverted;
    bool          CameraFPBowXYInverted;
    bool          CameraFPBowYZInverted;
    bool          CameraPCBowXYInverted;
    bool          CameraPCBowYZInverted;
    bool          CameraPCViewHeroXYInverted;
    bool          CameraPCViewHeroYZInverted;
    bool          TutorialSystemEnabled;
    bool          GuildMasterEnabled;
    bool          HUDTooltipEnabled;                       // +0x19
    float         Gamma;                                   // +0x1C
    float         HUDAlpha;
    float         SoundVolume;
    float         MusicVolume;
    float         DialogueVolume;                          // +0x2C
    bool          UsingHeadPhones;                         // +0x30
    bool          SoundsMuted;                             // +0x31
    bool          ShowingBuddyNames;                       // +0x32
    bool          ApplyVideoValuesOnLoad;                  // +0x33
    bool          DisplayDialogOnOptionsChanged;           // +0x34
    unsigned long Antialiasing;                            // +0x38
    float         TextureDetail;
    float         ShadowDetail;
    float         MeshDetail;
    unsigned long Resolution;                              // +0x48
    unsigned long RefreshRate;                             // +0x4C
    bool          VerticalSyncEnabled;                     // +0x50
    bool          StereoReversed;                          // +0x51
    std::vector<CActionInputControl> AssignedInputsPassive;     // +0x54
    std::vector<CActionInputControl> AssignedInputsAggressive;  // +0x60
    bool          ShowTargetingStatus;                     // +0x6C
    float         EffectsDetail;                           // +0x70
    unsigned long ScreenAspectRatio;                       // +0x74
    bool          NewProfileToUseWASD;                     // +0x78
    bool          CameraResetting;                         // +0x79
    float         CameraSensitivity;                       // +0x7C
    std::vector<CDisplayModeDesc> DisplayModes;           // +0x80
    std::vector<ETutorialCategory> GivenTutorialCategories;   // +0x8C
    std::queue<ETutorialCategory> PendingTutorialCategories;  // +0x98
    std::vector<CWideString> SaveGameNames;               // +0xC0
    bool          DisplayTutorialsDuringCutscenes;         // +0xCC
    bool          NewProfileBools_cd[13];                  // +0xCD SubtitlingEnabledNewProfile .. HUDTooltipEnabledNewProfile
    float         GammaNewProfile;                         // +0xDC
    float         HUDAlphaNewProfile;
    float         SoundVolumeNewProfile;
    float         MusicVolumeNewProfile;
    float         DialogueVolumeNewProfile;                // +0xEC
    bool          UsingHeadPhonesNewProfile;               // +0xF0
    bool          ShowingBuddyNamesNewProfile;             // +0xF1
    unsigned long AntialiasingNewProfile;                  // +0xF4
    float         TextureDetailNewProfile;
    float         ShadowDetailNewProfile;
    float         MeshDetailNewProfile;
    unsigned long ResolutionNewProfile;                    // +0x104
    unsigned long RefreshRateNewProfile;                   // +0x108
    bool          VerticalSyncEnabledNewProfile;           // +0x10C
    bool          StereoReversedNewProfile;                // +0x10D
    CWideString   ScoreBoardUserName;                      // +0x110
    CWideString   ScoreBoardPassword;                      // +0x114
    CWideString   ScoreBoardProxyIp;                       // +0x118
    long          ScoreBoardProxyPort;                     // +0x11C
    long          ScoreBoardServerPort;                    // +0x120
    float         EffectsDetailNewProfile;                 // +0x124
    unsigned long ScreenAspectRatioNewProfile;             // +0x128
    bool          CameraResettingNewProfile;               // +0x12C
    float         CameraSensitivityNewProfile;             // +0x130
    bool          ShowTargetingStatusNewProfile;           // +0x134
    CTexture*     PBackBufferTexture;                      // +0x138
    bool          VideoChangesAccepted;                    // +0x13C

    CUserProfileManager();
    virtual ~CUserProfileManager();                        // out of line (??_G 0x00412B00)
    virtual void OnPreDeviceReset();                       // 0x004067F0
    virtual void OnPostDeviceReset();                      // 0x00406800
    CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef() const;   // 0x004083E0
    void SetDefaultValues();                               // 0x0040C790
    void SetDefaultValuesForNewProfile();                  // 0x00409B70
};

CUserProfileManager::CUserProfileManager()
    : UseNewProfileValues(false),
      SoundsMuted(false),
      ApplyVideoValuesOnLoad(true),
      DisplayDialogOnOptionsChanged(true),
      SaveGameNames(50),
      DisplayTutorialsDuringCutscenes(false),
      VideoChangesAccepted(true)
{
    std::vector<CDisplayModeDesc> modes;
    CDefPointer<const CConfigOptionsDefaultsDef> defaults = GetConfigOptionsDefaultsDef();
    long minWidth = 1024;
    long minHeight = 768;
    if (defaults.PObject) {
        minWidth = defaults->MinResolutionWidth;
        minHeight = defaults->MinResolutionHeight;
    }
    GFGetSystemManager()->PDisplayManager->EnumerateDisplayModes(modes, C2DExtentsI(minWidth, minHeight), C2DExtentsI(0x7FFF, 0x7FFF));

    CDisplayModeDesc prev = *modes.begin();
    for (std::vector<CDisplayModeDesc>::iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X != prev.Dimensions.X || it->Dimensions.Y != prev.Dimensions.Y || it->BitsPerPixel != prev.BitsPerPixel)
            DisplayModes.push_back(prev);
        prev = *it;
    }
    if (!DisplayModes.empty()) {
        if (DisplayModes.back().Dimensions.X != prev.Dimensions.X || DisplayModes.back().Dimensions.Y != prev.Dimensions.Y
            || DisplayModes.back().BitsPerPixel != prev.BitsPerPixel)
            DisplayModes.push_back(prev);
    }

    SetDefaultValues();
    SetDefaultValuesForNewProfile();
    ProfileName = (const wchar16*)L"Default";
    PBackBufferTexture = 0;
    GFGetSystemManager()->PDisplayManager->AddDeviceResetCallback(this);
}
