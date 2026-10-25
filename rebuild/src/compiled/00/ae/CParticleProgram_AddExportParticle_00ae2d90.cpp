// CParticleProgram::AddExportParticle(const CCharString& name, unsigned long particle) -- register a named export particle: append the {name, particle index} pair to the ExportParticles vector map and mark it dirty (sorted on the next lookup)
// Leaf: ?AddExportParticle@CParticleProgram@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE2D90, 137 bytes, __fastcall `ret 8`. No direct caller (the particle-script parser
//   0x00AE8xxx family inlines it; this is the out-of-line copy kept in the image).
// Identity: Ego_r.pdb `?AddExportParticle@CParticleProgram@@QAEXABVCCharString@@K@Z`; ExportParticles is
//   CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > at +0x88 (the landed
//   WriteToFile 0x00AE2230 reads it there: 8-byte {name, index} entries, Dirty at +0x95).
// Callees: CCharString copy ctor 0x0099EC30 (x2: the pair temporary, then the placement copy), ~CCharString 0x0099EAE0,
//   vector<pair<CCharString, unsigned long> >::_M_insert_overflow 0x009B71B0 (`(_M_finish, x, __false_type(), 1, true)`).
// Layout: the generated engine/CParticleProgram.h is the Ego_r.pdb view with opaque 16-byte CArrays and an opaque map, so
//   the program is modelled locally up to the map (+0x88 is the same in both layouts); retail vector map = STLport vector
//   (12 bytes) + empty Compare +0xc + Dirty +0xd.
// Label: catalog "CParticleMorphs_InsertEntry" is propagated.
// Shape: the CVectorMap::Add family (landed 0x00759570 / 0x00AF3A40): `m_vector.push_back(value_type(key, value)); Dirty
//   = true;` with STLport push_back inline (placement copy with its null test, else _M_insert_overflow with the empty tag
//   homed in the dead name slot); the temporary's string dies before the Dirty store on both paths. VC7.1 keeps a plain
//   inline Add out of line here (`add ecx,88h ; call`, 24 bytes): the map's Add is `__forceinline`.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                                      // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct _STLP_alloc_proxy {             // the end-of-storage pointer over an empty allocator
    _Tp* _M_data;
};

template <class _Tp> class vector {                        // STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp> _M_end_of_storage;               // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);          // 0x009B71B0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> m_vector;                      // +0x00
    C    Compare;                                           // +0x0c (empty)
    bool Dirty;                                             // +0x0d
    __forceinline void Add(const K& key, const V& value)
    {
        m_vector.push_back(value_type(key, value));
        Dirty = true;
    }
};

class CParticleProgram {
public:
    void*         __vftable;                                // +0x00 (CMovableResource)
    unsigned char _base_0x04[0x4];                          // +0x04
    unsigned char InitialSimulation[0x80];                  // +0x08 CParticleSimulation
    CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > ExportParticles;   // +0x88
    void AddExportParticle(const CCharString& name, unsigned long particle);
};

void CParticleProgram::AddExportParticle(const CCharString& name, unsigned long particle)
{
    ExportParticles.Add(name, particle);
}