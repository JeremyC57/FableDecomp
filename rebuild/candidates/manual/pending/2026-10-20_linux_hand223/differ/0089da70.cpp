// Grade: DIFFER(311v314) -- the alpha byte: retail stores it into the dead `alpha` slot and re-reads that slot (`mov eax,[esp+18h] ; push eax`) for each carried thing; ours pushes ebp, stores the byte, then keeps the slot's dword in ebp for the loop
// CGameScriptInterface::EntitySetAlpha(const CScriptThing& entity, float alpha, bool includeCarried) const -- set the live entity's graphic alpha (alpha clamped to [0, 1], scaled to a byte) and, when asked, the alpha of every live thing it carries
// Leaf: ?EntitySetAlpha@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089DA70, length 312 bytes (+ int3 padding), `ret 0Ch`.
// Signature from Ego_r.pdb: ?EntitySetAlpha@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z.
// Callees: CScriptThing slots 75 IsAlive and 11 GetThing, __ftol2 @ 0x00BFEA70, CTCGraphicAppearance::SetAlpha(unsigned
//   char) @ 0x004BFAC0, the TC map lower bound @ 0x0040F020, CTCCarrying::GetAllCarriedThings(vector<CThing*>&) const
//   @ 0x006A2220 (manifest "COptimisedPrimitive::RemapGroups" is propagated), free @ 0x00BFEA14 (the vector's storage).
//   Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x01230014 = 255.0f.
// Layout: retail CThing interface bitset +0x20 (TC 0x46 CTCCarrying = +0x28 bit 6), map +0x44, graphic appearance +0x64,
//   dead bit +0x91; the carried-thing vector is a 12-byte STLport vector local.
// Shape: the clamp stays on the x87 stack (if / else-if, as SetThemeWeight 0x00886D70); the alpha byte is a local homed in
//   the dead `alpha` slot and re-read for each carried thing; HasTC + one-ternary GetTC puts the carrying component in ecx;
//   the loop re-derives the size and re-reads the vector's start after each SetAlpha.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);                 // 0x004BFAC0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicAppearance;            // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
template <class T> struct vector {                      // STLport vector<T*>: start, finish, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return *(_M_start + i); }
};
struct CTCCarrying {
    void GetAllCarriedThings(vector<CThing*>& things) const;   // 0x006A2220
};
struct CScriptThing {
#define VF(n) virtual void v##n() const;
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10)
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43)
    VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69) VF(70) VF(71) VF(72) VF(73) VF(74)
#undef VF
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp[2];
};

struct CGameScriptInterface {
    virtual void EntitySetAlpha(const CScriptThing& entity, float alpha, bool includeCarried) const;
};

void CGameScriptInterface::EntitySetAlpha(const CScriptThing& entity, float alpha, bool includeCarried) const
{
    if (!entity.IsAlive())
        return;
    CThing* thing = entity.GetThing();
    CTCGraphicAppearance* appearance = thing->GraphicAppearance;
    if (!appearance)
        return;
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    unsigned char alphaByte = (unsigned char)(alpha * 255.0f);
    appearance->SetAlpha(alphaByte);
    if (includeCarried && thing->HasTC(0x46)) {
        CTCCarrying* carrying = thing->GetTC<CTCCarrying>(0x46);
        vector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned long i = 0; i < carried.size(); ++i) {
            CThing* item = carried[i];
            if (item && !(item->m_flags91 & 1) && item->GraphicAppearance)
                item->GraphicAppearance->SetAlpha(alphaByte);
        }
    }
}
