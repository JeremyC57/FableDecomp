// Grade: DIFFER(190v190) -- register swap only: retail keeps the loop index in ebp and the 12-byte entry offset in edi (`xor ebp,ebp` before `push edi`, `mov eax,[esi+0Ch] ; mov edx,[edi+eax]`), ours the reverse (edi index, ebp offset); the in-loop release reads the vtable into eax (ours edx). Every other instruction matches.
// CTCCarrying::IsPrimarySlotFree() const -- true when the thing has a primary carry slot and every primary slot is empty
// Leaf: ?IsPrimarySlotFree@CTCCarrying@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1270, length 190 bytes (+ int3 padding), thiscall, plain `ret`.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0 (static), CDefinitionManager::GetDef<T>(long, CDefPointer<T>*)
//   @ 0x006A3060 (landed: assigns the out pointer with add-ref / release), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, the definition's virtual slot 1 on the last release.
// Identity: Ego_r.pdb `?IsPrimarySlotFree@CTCCarrying@@QBE_NXZ` (name set only); retail order matches the donor's
//   (GetCarriedThing 0x006A1120 .. IsCarrySlotFree 0x006A1230, IsPrimarySlotFree, IsSlotPrimarySlot 0x006A1330).
//   The catalog's "CThing_Array_GetFirstPhysicalSwitch" is a descriptive auto label.
// Layout: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc = the retail sorted vector of
//   12-byte {slot, pointer} pairs (the landed CTCCarrying models); the slot def's IsPrimary flag at +0x30 and its
//   reference count at +4 (the landed IsSlotPrimarySlot 0x006A1330).
// Shape: index loop with size() = (last - first) / 12 recomputed every pass and compared unsigned; a
//   CDefPointer local per pass (null-initialised after the argument pushes), released with `dec [ebx+4] ; jne`
//   (`if (--RefCount == 0)` on a direct member); a full primary slot releases and returns false at once, an
//   empty one sets the frame byte `found`.
class CThing;

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    unsigned int size() const { return m_pLast - m_pFirst; }
    const SCarrySlotEntry& operator[](unsigned int n) const { return *(m_pFirst + n); }
};

struct CCarrySlotDef {
    virtual void v00();
    virtual void DeleteThis();                      // slot 1
    long          RefCount;                         // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;                        // +0x30
};

template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (--m_p->RefCount == 0) m_p->DeleteThis(); }
    T* operator->() const { return m_p; }
};

struct CDefinitionManager {
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_006a1270();                           // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail, landed at 0x006A3060): it writes the out slot only, so
// VC7.1 knows the def pointer's address does not escape the call (the landed 0x006A1330 / 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_006a1270();
    if (!def)
        return;
    *(void**)out = def;
}
#pragma auto_inline(on)

struct CGameDefinitionManager {
    static CDefinitionManager* Get();               // 0x0044C6B0
};

struct CTCCarrying {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    CCarrySlotMap CarrySlots;                       // +0x0c
    bool IsPrimarySlotFree() const;
};

bool CTCCarrying::IsPrimarySlotFree() const
{
    bool found = false;
    for (unsigned int i = 0; i < CarrySlots.size(); ++i) {
        CDefPointer<CCarrySlotDef> def;
        CGameDefinitionManager::Get()->GetDef(CarrySlots[i].first, &def);
        if (def->IsPrimary) {
            if (CarrySlots[i].second.GetPItem())
                return false;
            found = true;
        }
    }
    return found;
}
