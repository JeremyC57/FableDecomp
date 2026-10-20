// CTCInventoryClothing::SetUpSelectedClothes() -- rebuilds the worn-clothing map: every selected item that is not a whole suit and whose appearance modifier names a suit part is recorded as that part's item
// Retail 0x005B4795, 269 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: map<EClothingSuitPart, long>::clear 0x004DB795 (manifest "CTCCarryable::OnKill" is propagated),
// CTCInventoryBase::GetAllSelectedItems(vector<long>*) 0x005BEAF5, CTCBase::GetDefinitionManager 0x006869D0,
// CDefinitionManager::GetDef<T>(long, CDefPointer*) 0x0042AF3C, CDefClassBase::GetSubDef<CHeroSuitDef> 0x005935C4 and
// GetSubDef<CAppearanceModifierDef> 0x0057FF92, map<EClothingSuitPart, long>::operator[](const key&) 0x005BBBF3, free
// 0x00BFEA14, the defs' slot 1 on release.
// Shape: the selected item vector is a zeroed local freed inline at the end; each positive item fetches its def, then
// (unless it is a suit) its appearance modifier into the def's now-dead out slot; a non-zero suit part maps to the item
// (the element address is formed before the map call); the modifier is released without a null test, the suit and the
// def with one (the shared zero register `esi`).
// Layout (retail): CurrentClothingSelected map +0x140 (as the landed ctor 0x005B7049); CAppearanceModifierDef suit part
// +0x4C.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
};
struct CHeroSuitDef : CDefPointeeBase {};
struct CAppearanceModifierDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x4c - 0x08];
    long SuitPart;                                   // +0x4c EClothingSuitPart
};
template <class T> struct CDefPointerSlot {         // the out-parameter view of a CDefPointer (released by hand)
    T* Object;
};
struct CThingDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CHeroSuitDef>* out);            // 0x005935C4
    bool GetSubDef(CDefPointerSlot<CAppearanceModifierDef>* out);   // 0x0057FF92
};
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<CHeroSuitDef>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
void* FindDef_s7r_1();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_1 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_1 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_1()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointerSlot<CAppearanceModifierDef>* out)
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long index, CDefPointerSlot<CThingDef>* out);   // 0x0042AF3C
};
void* FindDef_s7r_2();
long TmplSink_s7_2(const void* self, long seen);   // stand-in: the work FindDef_s7r_2 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_2 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_2()
{
    long r = TmplSink_s7_2(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointerSlot<CThingDef>* out)
{
    void* def = FindDef_s7r_2();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CLongVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    void clear();                                    // 0x004DB795
    long& operator[](const long& part);              // 0x005BBBF3
};

struct CTCInventoryClothing {
    unsigned char _pad_0x000[0x140];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void GetAllSelectedItems(CLongVector* items);    // 0x005BEAF5 (CTCInventoryBase)
    void SetUpSelectedClothes();
};

void CTCInventoryClothing::SetUpSelectedClothes()
{
    CurrentClothingSelected.clear();
    CLongVector selected;
    GetAllSelectedItems(&selected);
    for (unsigned long i = 0; i < selected.size(); ++i) {
        if (selected[i] > 0) {
            CDefPointerSlot<CThingDef> def;
            def.Object = 0;
            GetDefinitionManager()->GetDef(selected[i], &def);
            CThingDef* thingDef = def.Object;
            {
                CDefPointer<CHeroSuitDef> suit;
                if (!thingDef->GetSubDef(&suit)) {
                    CDefPointerSlot<CAppearanceModifierDef> modifierSlot;
                    modifierSlot.Object = 0;
                    thingDef->GetSubDef(&modifierSlot);
                    CAppearanceModifierDef* modifier = modifierSlot.Object;
                    if (modifier->SuitPart != 0)
                        CurrentClothingSelected[modifier->SuitPart] = selected[i];
                    modifier->Release();
                }
            }
            if (thingDef)
                thingDef->Release();
        }
    }
}