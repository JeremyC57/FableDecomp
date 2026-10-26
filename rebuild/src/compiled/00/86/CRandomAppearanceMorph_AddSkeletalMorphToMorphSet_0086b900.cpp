// CRandomAppearanceMorph::AddSkeletalMorphToMorphSet(unsigned long&, const CSkeletalMorph&, CMorphSet&) const -- append the skeletal morph to the morph set by name, with a random strength in [0, 1) when the morph allows variation, else full strength
// Leaf: ?AddSkeletalMorphToMorphSet@CRandomAppearanceMorph@@IBEXAAKABVCSkeletalMorph@1@AAVCMorphSet@1@@Z
// Retail 0x0086B900, 0xC8 bytes (+ int3 padding to 0xD0), thiscall `ret 0Ch` (`this` unused). FableWin/Ego_r name:
// ?AddSkeletalMorphToMorphSet@CRandomAppearanceMorph@@IBEXAAKABVCSkeletalMorph@1@AAVCMorphSet@1@@Z (protected const);
// callers 0x0086BA7E / 0x0086BB1A are GetRandomSkeletalMorph's two picks (seed, &pair.second, morph set).
// Callees: CCharString() @ 0x0099E4B0, CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828
// (by-value result built in the dead `morph` argument slot), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0
// (temporary, then the entry), GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall: seed in ecx, 1.0f pushed),
// CCharString(const CCharString&) @ 0x0099EC30 (the push_back placement copy), vector<CMorphSetSkeletalMorph>::
// _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x0086D370.
// Layout (Ego_r, retail-shifted): CSkeletalMorph {long GroupID +0, CDefString Name +4, bool AllowVariation +8};
// CMorphSet {CArray<CMeshTextureSet> BodyParts +0, CArray<CMorphSetSkeletalMorph> SkeletalMorphs +0x0C (retail vectors are
// 12 bytes), CArray<CMorphSetTextureMorph> GeneralTextureMorphs +0x18}; CMorphSetSkeletalMorph {CCharString Name +0, float
// Strength +4} (implicit default constructor: Name only, Strength left unset).
// Shape: the looked-up name is a block-scoped named local assigned to the entry (`lea edx,[esp+18h] ; push edx` before
// operator=, destroyed before the strength test; assigning the GetString temporary directly pushes the returned eax);
// the strength is `AllowVariation ? GFFloatRandom(1.0f, &seed) : 1.0f` as an if/else; then the inline STLport push_back.
// Manifest label "CreateRandomMorphEntry" is a descriptive auto label.

class CCharStringData;

class CCharString
{
public:
    CCharStringData* PStringData;                    // +0x00
    CCharString();                                   // 0x0099E4B0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};

class CDefString
{
public:
    long TablePos;                                   // +0x00
};

class CDefStringTable
{
public:
    CCharString GetString(long tablePos);            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;             // 0x013CA828

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {
struct __false_type { __false_type() { } };     // user constructor: the tag temporary is not zero-stored

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class vector                    // STLport vector<_Tp, allocator<_Tp> >, 12 bytes
{
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _Tp* _M_end_of_storage;                          // +0x08

    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class T> class CArray : public std::vector<T> { };

class CRandomAppearanceMorph
{
public:
    class CSkeletalMorph
    {
    public:
        long       GroupID;                          // +0x00
        CDefString Name;                             // +0x04
        bool       AllowVariation;                   // +0x08
    };

    class CMorphSet
    {
    public:
        class CMeshTextureSet;
        class CMorphSetTextureMorph;
        class CMorphSetSkeletalMorph
        {
        public:
            CCharString Name;                        // +0x00
            float       Strength;                    // +0x04
        };

        CArray<CMeshTextureSet>        BodyParts;               // +0x00
        CArray<CMorphSetSkeletalMorph> SkeletalMorphs;          // +0x0C
        CArray<CMorphSetTextureMorph>  GeneralTextureMorphs;    // +0x18
    };

protected:
    void AddSkeletalMorphToMorphSet(unsigned long& seed, const CSkeletalMorph& morph, CMorphSet& morphSet) const;
};

void CRandomAppearanceMorph::AddSkeletalMorphToMorphSet(unsigned long& seed, const CSkeletalMorph& morph, CMorphSet& morphSet) const
{
    CMorphSet::CMorphSetSkeletalMorph entry;
    {
        CCharString name = g_DefStringTable.GetString(morph.Name.TablePos);
        entry.Name = name;
    }
    if (morph.AllowVariation)
        entry.Strength = GFFloatRandom(1.0f, &seed);
    else
        entry.Strength = 1.0f;
    morphSet.SkeletalMorphs.push_back(entry);
}