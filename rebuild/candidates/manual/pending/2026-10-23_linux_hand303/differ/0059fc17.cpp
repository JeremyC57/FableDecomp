// CTCInventoryTrade::AlreadyHaveSpecial(long defIndex) -- true when the hero already has the hair style / tattoo / ability the special item gives
// Grade: DIFFER(715v716) -- retail holds the trade-entry pointer in edi and keeps the vector size in a frame word (sar [ebp-0Ch],2); ours holds it in ecx with a spill to [ebp-10h]
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> struct CDefBasePointer {
    T* Object;
    __forceinline ~CDefBasePointer()
    {
        if (Object) {
            T* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CStringData { bool Equals(const CStringData* other) const; };   // 0x004A93C0
struct CCharString {
    CStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return PStringData->Equals(other.PStringData);
        return false;
    }
};
struct CHairCardDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long HairType;                                      // +0x28
};
struct CTattooDef : CDefBase {
    unsigned char _pad08[0x34 - 0x08];
    long Area;                                          // +0x34
    unsigned char _pad38[0x3c - 0x38];
    bool Permanent;                                     // +0x3c
    bool Exclusive;                                     // +0x3d
    unsigned char _pad3e[0x4c - 0x3e];
    CCharString Name;                                   // +0x4c
    bool IsConflicting(const CTattooDef* other) const;  // 0x0059B679
};
CDefBasePointer<CHairCardDef> __fastcall GetHairCardDef(long defIndex);      // 0x005D9A10
CDefBasePointer<CTattooDef> __fastcall GetTattooCardDef(long defIndex);      // 0x005D9AA0
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                          // 0x00581353
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int i) { return _M_start[i]; }
};
struct CTCHeroMorph {
    void GetTattoos(vector<CTattooDef*>& out) const;    // 0x0071DD50
    long HasAPermanentTattooCoveringArea() const;       // 0x0071C2D0
};
struct CTCAppearance {
    bool IsWearingAppearanceModifier(long type, bool b) const;   // 0x00706000
};
struct CTCHeroStats {
    unsigned char _pad00[0x14];
    long SpecialAbility;                                // +0x14
};
struct CAbilityDef : CDefBase { };
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out);      // 0x005B1E5A
};
void* FindDef_a();
void* FindDef_b();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out)
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CAbilityDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_b();
    if (!def)
        return false;
    out->Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPresenceBits { bool test(long kind) const; };   // 0x00410DE0
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInventoryTrade {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    bool AlreadyHaveSpecial(long defIndex);
};
bool CTCInventoryTrade::AlreadyHaveSpecial(long defIndex)
{
    CDefBasePointer<CHairCardDef> hair = GetHairCardDef(defIndex);
    if (hair.Object) {
        CTCAppearance* appearance;
        if (ParentThing->GetInterface(&appearance, 0x5e)) {
            if (appearance->IsWearingAppearanceModifier(hair->HairType, false))
                return true;
        }
    }
    CDefBasePointer<CTattooDef> tattoo = GetTattooCardDef(defIndex);
    if (tattoo.Object) {
        CTCHeroMorph* morph;
        if (ParentThing->GetInterface(&morph, 3))
            ;
        if (morph) {
            vector<CTattooDef*> tattoos;
            morph->GetTattoos(tattoos);
            if (tattoos.empty()) {
                if (tattoo->Permanent)
                    return true;
                return false;
            }
            CTattooDef* def = tattoo.Object;
            if (def->Permanent) {
                if (tattoos.size() == 1 && morph->HasAPermanentTattooCoveringArea())
                    return true;
                return false;
            }
            unsigned int count = tattoos.size();
            for (unsigned int i = 0; i < count; ++i) {
                CTattooDef* t = tattoos[i];
                if (t->Area == def->Area) {
                    if (t->Area != -1 || t->Area != def->Area)
                        return true;
                    if (t->Name == def->Name)
                        return true;
                } else if (t->Exclusive && t->IsConflicting(def))
                    return true;
            }
        }
    }
    CDefPointer<CDefClassBase> def;
    CDefPointer<CAbilityDef> ability;
    if (GetDefinitionManager()->GetDef(defIndex, &def)) {
        if (def->GetSubDef(&ability)) {
            CTCHeroStats* stats;
            ParentThing->GetInterface(&stats, 0x29);
            if (stats && stats->SpecialAbility == defIndex)
                return true;
        }
    }
    return false;
}
