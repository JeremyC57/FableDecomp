// CCombatActionBase::CStrikeEvent::CStrikeEvent(CThing* weapon, long strikeEventIndex, const CCharString& helperPointName, long startFrame, bool isWeapon, bool legacy, bool primaryLegacy) -- one strike window of a combat animation: flags, helper point, index, start frame (end 0), the weapon held by intelligent pointer, an empty struck-thing list
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085ADB0, 102 bytes; `ret 1Ch`. PDB ??0CStrikeEvent@CCombatActionBase@@QAE@PAVCThing@@JABVCCharString@@J_N22@Z.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer vtable 0x01237F1C.
// Layout: Ego_r.pdb CStrikeEvent (Legacy +0, PrimaryLegacy +1, Weapon +2, HelperPointName +4, StrikeEventIndex +8,
//         StartFrame +0xc, EndFrame +0x10, PWeapon +0x14, StruckThingList +0x1c as a 12-byte STLport vector).
// Shape: members in declaration order; the intelligent pointer's inline ctor (base ctor, vtable, SetPItem); one zero register.
// Leaf: ??0CStrikeEvent
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};
class CThing;
class CThingGameObject;
struct CBaseIntelligentPointer {
    virtual void v000();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(const void* item);               // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual void v000();
};
template <class T> struct _STLP_alloc_proxy {      // STLport: the end-of-storage pointer with its (empty) allocator
    T _M_data;
    _STLP_alloc_proxy(const T& p) : _M_data(p) { }
};
template <class T> struct _Vector_base {
    T*                     _M_start;
    T*                     _M_finish;
    _STLP_alloc_proxy<T*>  _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
template <class T> struct CVector : _Vector_base<T> {   // STLport vector<T>
    CVector() { }
};
class CCombatActionBase {
public:
    class CStrikeEvent {
    public:
        bool                                Legacy;            // +0x00
        bool                                PrimaryLegacy;     // +0x01
        bool                                Weapon;            // +0x02
        CCharString                         HelperPointName;   // +0x04
        long                                StrikeEventIndex;  // +0x08
        long                                StartFrame;        // +0x0c
        long                                EndFrame;          // +0x10
        CIntelligentPointer<const CThing>   PWeapon;           // +0x14
        CVector<CThingGameObject*>          StruckThingList;   // +0x1c
        CStrikeEvent(CThing* weapon, long strikeEventIndex, const CCharString& helperPointName, long startFrame,
                     bool isWeapon, bool legacy, bool primaryLegacy);
    };
};

CCombatActionBase::CStrikeEvent::CStrikeEvent(CThing* weapon, long strikeEventIndex, const CCharString& helperPointName,
                                              long startFrame, bool isWeapon, bool legacy, bool primaryLegacy)
    : Legacy(legacy), PrimaryLegacy(primaryLegacy), Weapon(isWeapon), HelperPointName(helperPointName),
      StrikeEventIndex(strikeEventIndex), StartFrame(startFrame), EndFrame(0), PWeapon(weapon)
{
}