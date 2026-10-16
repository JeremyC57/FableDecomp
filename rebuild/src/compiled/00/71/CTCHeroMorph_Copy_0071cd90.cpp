// CTCHeroMorph::Copy(const CTCBase& other) -- copy another hero-morph component: the shared mesh-morph set, the tattoo defs, the tattoo frame and the morph weights / teenager flag, then mark the morph changed
// Leaf: ?Copy@CTCHeroMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071CD90, length 150 bytes, `ret 4`.
// Ego_r.pdb: ?Copy@CTCHeroMorph@@UAEXABVCTCBase@@@Z (virtual)
// Callees: operator delete @ 0x00BFE9BC (the counted pointer's info block), the block's __fastcall DeleteFunc, the out-of-line
// STLport vector<CDefPointer<const CThingBaseDef>>::operator= @ 0x00721150 (manifest "Vector_CopyOrMove_CountedPtr").
// Shape: CCountedPointer::operator= loads the source Data/Info first, skips on the same info block, releases the old block (Info
// re-read after the decrement), stores Data then Info and AddRefs the new block; the plain members are copied in declaration
// order (FrameHeroBecameIdle and IdleParticlesAdded are not copied) and `Changed = true` is last.
// Layout: retail PMeshMorphSet +0x0c (Data, Info), ParticleEmitters +0x14, IdleParticleEmitters +0x18, Tattoos +0x1c,
// CustomToBankIndex +0x28, FrameTattoosChanged +0x34, FrameHeroBecameIdle +0x38, IdleParticlesAdded +0x3c, Changed +0x3d,
// Strength +0x40, Berserk +0x44, Will +0x48, Skill +0x4c, Age +0x50, Morality +0x54, Fatness +0x58, Tan +0x5c, Teenager +0x60.
void __cdecl operator delete(void* block);      // 0x00BFE9BC

struct CCPPointerInfo {                         // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* data = o.Data;
        CCPPointerInfo* info = o.Info;
        if (Info != info) {
            ReleaseInfo();
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
};
struct CDefPointeeBase;
template <class T> struct CDefPointer { CDefPointeeBase* Object; };
struct CThingBaseDef;
struct CDefPointerVector {                      // STLport vector<CDefPointer<const CThingBaseDef>>
    CDefPointer<const CThingBaseDef>* _M_start;
    CDefPointer<const CThingBaseDef>* _M_finish;
    CDefPointer<const CThingBaseDef>* _M_end_of_storage;
    CDefPointerVector& operator=(const CDefPointerVector& o);   // 0x00721150
};
struct CMeshMorphSet;
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    virtual void Copy(const CTCBase& other);
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroMorph : public CTCBase {
public:
    CCountedPointer<CMeshMorphSet> PMeshMorphSet;   // +0x0c
    void*             ParticleEmitters;         // +0x14
    void*             IdleParticleEmitters;     // +0x18
    CDefPointerVector Tattoos;                  // +0x1c
    void*             CustomToBankIndex[3];     // +0x28
    long              FrameTattoosChanged;      // +0x34
    long              FrameHeroBecameIdle;      // +0x38
    bool              IdleParticlesAdded;       // +0x3c
    bool              Changed;                  // +0x3d
    float             Strength;                 // +0x40
    float             Berserk;                  // +0x44
    float             Will;                     // +0x48
    float             Skill;                    // +0x4c
    float             Age;                      // +0x50
    float             Morality;                 // +0x54
    float             Fatness;                  // +0x58
    float             Tan;                      // +0x5c
    bool              Teenager;                 // +0x60
    virtual void Copy(const CTCBase& other);
};

void CTCHeroMorph::Copy(const CTCBase& other)
{
    const CTCHeroMorph& o = (const CTCHeroMorph&)other;
    PMeshMorphSet = o.PMeshMorphSet;
    Tattoos = o.Tattoos;
    FrameTattoosChanged = o.FrameTattoosChanged;
    Strength = o.Strength;
    Berserk = o.Berserk;
    Will = o.Will;
    Skill = o.Skill;
    Age = o.Age;
    Morality = o.Morality;
    Fatness = o.Fatness;
    Tan = o.Tan;
    Teenager = o.Teenager;
    Changed = true;
}