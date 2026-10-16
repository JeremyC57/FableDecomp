// CHeroMorphDef::operator=(const CHeroMorphDef&) -- def copy: CDefClassBase part, then the CHeroMorphDef fields memberwise in declaration order
// Leaf: ??4CHeroMorphDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6F0B, 105 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E6F06 in slot 19 of vtable 0x01242BCC = CHeroMorphDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, the morph vectors' operator= @ 0x004E6F74 / 0x004E72C8 / 0x004E711E (twice).
// Layout (engine/CHeroMorphDef.h, PDB 20-byte maps are 16 in retail: PDB +0x28/+0x3c/+0x50/+0x64): TextureMorphs +0x28, SkeletalMorphs +0x38, ParticleMorphs +0x48, IdleParticleMorphs +0x58.
// Shape: (the CVectorMap and morph-class assignments are written as `__forceinline` copies of the implicit ones:
//   left implicit, VC7.1 emits them out of line here) the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// Each morph class wraps one CVectorMap whose implicit assignment inlines: vector operator=, nothing for the empty comparator, the sorted byte at +0x0d.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CKeyPairCompareLess {};          // empty comparator (copies nothing)
struct CTextureMorphEntryVector {       // retail std::vector<pair<EHeroMorphType,CTextureMorphEntry>> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CTextureMorphEntryVector& operator=(const CTextureMorphEntryVector& other);   // 0x004E6F74
};
struct CSkeletalMorphEntryVector {      // retail std::vector<pair<unsigned long,CSkeletalMorphs::CEntry>>
    void* m_first; void* m_last; void* m_end;
    CSkeletalMorphEntryVector& operator=(const CSkeletalMorphEntryVector& other); // 0x004E72C8
};
struct CParticleMorphEntryVector {      // retail std::vector<pair<EHeroMorphType,CParticleMorphs::CEntry>>
    void* m_first; void* m_last; void* m_end;
    CParticleMorphEntryVector& operator=(const CParticleMorphEntryVector& other); // 0x004E711E
};
template<class V> struct CVectorMap {   // retail 16 bytes (PDB 20): vector, empty comparator +0x0c, sorted flag +0x0d
    V m_vector;
    CKeyPairCompareLess m_compare;
    bool m_sorted;
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};
struct CTextureMorphs {
    CVectorMap<CTextureMorphEntryVector> Morphs;
    __forceinline CTextureMorphs& operator=(const CTextureMorphs& other) { Morphs = other.Morphs; return *this; }
};
struct CSkeletalMorphs {
    CVectorMap<CSkeletalMorphEntryVector> Morphs;
    __forceinline CSkeletalMorphs& operator=(const CSkeletalMorphs& other) { Morphs = other.Morphs; return *this; }
};
struct CParticleMorphs {
    CVectorMap<CParticleMorphEntryVector> Morphs;
    __forceinline CParticleMorphs& operator=(const CParticleMorphs& other) { Morphs = other.Morphs; return *this; }
};

struct CHeroMorphDef : CDefClassBase {
    CTextureMorphs           TextureMorphs;               // +0x28
    CSkeletalMorphs          SkeletalMorphs;              // +0x38
    CParticleMorphs          ParticleMorphs;              // +0x48
    CParticleMorphs          IdleParticleMorphs;          // +0x58
    CHeroMorphDef& operator=(const CHeroMorphDef& other);
};

CHeroMorphDef& CHeroMorphDef::operator=(const CHeroMorphDef& other)
{
    CDefClassBase::operator=(other);
    TextureMorphs = other.TextureMorphs;
    SkeletalMorphs = other.SkeletalMorphs;
    ParticleMorphs = other.ParticleMorphs;
    IdleParticleMorphs = other.IdleParticleMorphs;
    return *this;
}
