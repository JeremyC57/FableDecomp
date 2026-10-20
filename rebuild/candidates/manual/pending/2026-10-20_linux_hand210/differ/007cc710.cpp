// CTCMultiArrow::ReleaseShot() -- one shot fired: when a whole volley has gone, remove (and kill) the newest ammo effect, or count down the pending ammo effects when none is showing; clean up after the last shot
// Grade: DIFFER(185v180) -- only the empty test: retail compares the two vector pointers (`mov eax,[esi+30h] ; cmp eax,[esi+34h]`), this size()==0 form subtracts and masks; the pointer-compare forms tried let the lane CSE the end pointer into edi (wrong registers below)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC710, 180 bytes (plain `ret`; PDB ?ReleaseShot@CTCMultiArrow@@QAEXXZ).
// Callees: CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (the copy of the back element,
//   vtable 0x01238C6C), CThing::Kill(bool) @ 0x004C9B80, the element's virtual destructor (slot 0, `push 0`) in pop_back,
//   ~CBaseIntelligentPointer @ 0x00A01C10, FinalCleanup() @ 0x007CC410.
// Layout: Ego_r.pdb CTCMultiArrow: NumberOfShots +0x10, NumberInVolley +0x14, NumOfVolleys +0x18, CreateAmmoEffectCount +0x24,
//   AmmoEffects +0x30 (12-byte vector of 8-byte CIntelligentPointer<CThing>).
// Tried: `begin() == end()` on pointers, an iterator-object back(), iterator objects for begin/end/back, `empty()` as
//   `size() == 0`. Next: keep `size() == 0` out and write the test as `AmmoEffects._M_start == AmmoEffects._M_finish` with
//   back() through the iterator object only (the size form already stops the CSE).
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediately);                        // 0x004C9B80
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    T& back() const { return *(end() - 1); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    bool empty() const { return size() == 0; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
struct CRGBColour {
    unsigned char B;                                    // +0
    unsigned char G;                                    // +1
    unsigned char R;                                    // +2
    unsigned char A;                                    // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_1 = 1 };
    struct CTCGraphicAppearance {
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
        void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
    };
}
struct CSpecialAbilitiesMultiArrowDef {                 // retail = PDB - 4
    unsigned char _pad00[0x64];
    float         BowHighlightWidth;                    // +0x64
    vector<long>  BowHighlightColor;                    // +0x68
};
struct CTCMultiArrow {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    const CSpecialAbilitiesMultiArrowDef* PDef;         // +0x0c CDefPointer
    long          NumberOfShots;                        // +0x10
    long          NumberInVolley;                       // +0x14
    long          NumOfVolleys;                         // +0x18
    long          SpellLevel;                           // +0x1c
    bool          RemovedEffect;                        // +0x20
    long          CreateAmmoEffectCount;                // +0x24
    long          CreateAmmoEffectDelay;                // +0x28
    long          CreateAmmoEffectTimer;                // +0x2c
    vector< CIntelligentPointer<CThing> > AmmoEffects;  // +0x30
    void FinalCleanup();                                // 0x007CC410
    void ApplyEffectOnWeapon(CThing* weapon);
    void ReleaseShot();
};

void CTCMultiArrow::ReleaseShot()
{
    long volley = --NumberOfShots / NumberInVolley;
    if (volley < NumOfVolleys) {
        if (AmmoEffects.empty())
            --CreateAmmoEffectCount;
        else {
            CIntelligentPointer<CThing> effect = AmmoEffects.back();
            if (IsThingAlive(effect))
                effect.GetPItem()->Kill(true);
            AmmoEffects.pop_back();
        }
        NumOfVolleys = volley;
    }
    if (NumberOfShots <= 0)
        FinalCleanup();
}
