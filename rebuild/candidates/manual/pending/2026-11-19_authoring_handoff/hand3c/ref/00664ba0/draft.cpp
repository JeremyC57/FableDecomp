// CThingCreatureBase::GetCurrentMeshRadius() const -- the creature def's radius spread interpolated by its hero-stat radius weight for this thing's hero experience; 1 when the thing has no experience component or no creature def
// Grade: DIFFER(207v184) -- the failure-path null-tested release becomes the long `mov ecx,eax ; add eax,4 ; dec ; test ; mov [eax],..` form (retail `dec [ecx+4] ; jne`, the retry-8 lane gap), the key and out pointer get separate frame words (retail shares one), and the no-TC exit is laid out inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664BA0, length 184 bytes, `ret`, float in st0.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x68 = CTCHeroExperience), CDefClassBase::GetSubDef<CCreatureDef>
//   @ 0x00485815 (byte-twin label GetSubDef<CAbilityDef>), CHeroAbilityStatWeight::GetParameter(const CTCHeroExperience&)
//   const @ 0x00851BD0, the def's virtual Destroy (slot 1).
// Float pool: 0x0122DED8 = 1.0f.
// Layout: retail CThing interface bitset +0x20 (TC 0x68 = word +0x2c bit 8), interface map +0x44; retail CThingCreatureBase
//   PDef (CDefPointer<CThingCreatureDef>) +0xe0 (PDB +0xfc); retail CCreatureDef RadiusWeight +0xd0 (PDB +0xe0, its map is
//   12 bytes in retail), RadiusSpread +0xdc (CFloatRange: HighValue +0, LowValue +4; PDB +0xf0).
// Shape: HasTC/GetTC inline (the key and the sub-def out pointer share the `push ecx` frame word); the sub-def out
//   pointer is released by hand through the def's inline Release(): null-tested on the failure path, and on the
//   success path through the def copied into a local (no test; the count store sinks among the x87 code); the spread
//   is the inline `(High - Low) * t + Low`.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

class CTCHeroExperience;
struct CHeroAbilityStatWeight {
    unsigned char StatWeights[0xc];
    float GetParameter(const CTCHeroExperience& experience) const;   // 0x00851BD0
};
struct CFloatRange {
    float HighValue;                            // +0x00
    float LowValue;                             // +0x04
    float Interpolate(float t) const { return (HighValue - LowValue) * t + LowValue; }
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0xd0 - 0x08];
    CHeroAbilityStatWeight RadiusWeight;        // +0xd0
    CFloatRange RadiusSpread;                   // +0xdc
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out) const;     // 0x00485815 GetSubDef<CCreatureDef>
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad4c[0xe0 - 0x4c];
    CDefClassBase* PDef;                        // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    float GetCurrentMeshRadius() const;
};

float CThingCreatureBase::GetCurrentMeshRadius() const
{
    if (!HasTC(0x68))
        return 1.0f;
    CTCHeroExperience* experience = GetTC<CTCHeroExperience>(0x68);
    CDefPointer<CCreatureDef> creatureDef;
    if (!PDef->GetSubDef(&creatureDef)) {
        CCreatureDef* failed = creatureDef.Object;
        if (failed) {
            if (--failed->RefCount == 0)
                failed->Destroy();
        }
        return 1.0f;
    }
    CCreatureDef* def = creatureDef.Object;
    float radius = def->RadiusSpread.Interpolate(def->RadiusWeight.GetParameter(*experience));
    def->Release();
    return radius;
}
