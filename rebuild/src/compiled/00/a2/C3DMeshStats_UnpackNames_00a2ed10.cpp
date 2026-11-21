// C3DMeshStats::UnpackNames(std::vector<CCharString>&, bool, const CCharString&, bool) const -- append every packed helper-point (or dummy-object) name that starts with the prefix, once per object stored under it
// Leaf: ?UnpackNames@C3DMeshStats@@QAEXAAUCCharStringVector@@_NABVCCharString@@1@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail @ 0x00A2ED10, 468 bytes (ret 10h).
// Callees: strncmp 0x00BFEAF8, CCharString(const char*, long) 0x0099EBF0 / copy ctor 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CRC Calc 0x004014A0 (__fastcall seed, data, length), CPreAllocVectorMap<ulong,C3DDummyObject>::UpperBound 0x00A317E0 /
//   LowerBound 0x00A31790, CPreAllocVectorMap<ulong,C3DHelperPoint>::UpperBound 0x00A31700 / LowerBound 0x00A316B0,
//   vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape: PackedNames starts with the ushort offset of the dummy-object names (helper names follow the word); the
// prefix length is taken once; each name is a CCharString temporary (in the dead names-argument slot) pushed
// `count` times -- 1 when every name is wanted once, else the number of map entries under the name's CRC
// (UpperBound - LowerBound over 56- or 20-byte pairs). push_back is the inline STLport form with the __false_type
// tag over the dead bool slot.
// Name fix (NF212, 2026-11-21, q6332 / q6341): the header-less __false_type stand-in and the CCharStringVector stand-in's own
//   three words, push_back and same-TU growth body move onto the shared STLport vector (stlport/fable_containers.h):
//   CCharStringVector keeps its token (no class of that name under rebuild/include; this row's self leaf uses it) and sits on
//   std::vector<CCharString> (header-less local class on a shared base). The growth body is STLport's own, visible and out of line
//   (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, with the owner row 0x00433530's FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE so its
//   pick-any copy is the owner's), so push_back's growth call binds the protected IAE vector<CCharString>::_M_insert_overflow
//   (was the stand-in ?_M_insert_overflow@CCharStringVector@@QAE...K_N, strong in this object). The PDB self spelling
//   (private const, std::vector<CCharString>&: ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@..., FableWin.pdb and Ego_r.pdb name
//   sets) is a separate family: reloc_bindings already holds three spellings at 0x00A2ED10 (QAE stand-in from 0x00A2F0F0, QBE
//   stand-in from 0x00A2F150, the PDB form from 0x00A2F170); this hand-back leaves the self leaf as it was.
// Layout: C3DMeshStats retail offsets as in the landed SaveBinary 0x00A2EAA0 / GetHelperPoint 0x00A2E8C0:
// HelperPoints +0x38, DummyObjects +0x48, PackedNames +0x74.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "stlport/fable_containers.h"
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CCharStringData { public: char* Text; long Length; };
class CCharString { public:
    CCharStringData* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* GetText() const { return Data == 0 ? "" : Data->Text; }
    long GetLength() const { return Data == 0 ? 0 : Data->Length; }
};

struct CCharStringVector : std::vector<CCharString> { };   // the landed callers' name for the names vector (STLport vector<CCharString>)

#include "engine/C3DVector.h"
class CMatrix3x4 { public: float m_row[3][4]; };
struct SHelperPointEntry { unsigned long first; C3DVector Position; long BoneIndex; };        // 20 bytes
struct SDummyObjectEntry { unsigned long first; CMatrix3x4 Transform; long BoneIndex; };      // 56 bytes
struct CHelperPointMap {                   // CPreAllocVectorMap<unsigned long, C3DHelperPoint>
    bool Compare; SHelperPointEntry* Elements; unsigned long Size; bool Dirty;
    SHelperPointEntry* LowerBound(const unsigned long& key);   // 0x00A316B0
    SHelperPointEntry* UpperBound(const unsigned long& key);   // 0x00A31700
};
struct CDummyObjectMap {                   // CPreAllocVectorMap<unsigned long, C3DDummyObject>
    bool Compare; SDummyObjectEntry* Elements; unsigned long Size; bool Dirty;
    SDummyObjectEntry* LowerBound(const unsigned long& key);   // 0x00A31790
    SDummyObjectEntry* UpperBound(const unsigned long& key);   // 0x00A317E0
};

// Same-TU stand-ins for the out-of-line bound searches (they read the key only through the reference).
#pragma auto_inline(off)
SHelperPointEntry* CHelperPointMap::LowerBound(const unsigned long& key)
{
    SHelperPointEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SHelperPointEntry* CHelperPointMap::UpperBound(const unsigned long& key)
{
    SHelperPointEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (!(key < first[half].first)) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SDummyObjectEntry* CDummyObjectMap::LowerBound(const unsigned long& key)
{
    SDummyObjectEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
SDummyObjectEntry* CDummyObjectMap::UpperBound(const unsigned long& key)
{
    SDummyObjectEntry* first = Elements;
    unsigned long count = Size;
    while (count > 0) {
        unsigned long half = count >> 1;
        if (!(key < first[half].first)) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
#pragma auto_inline(on)

class C3DMeshStats { public:
    virtual void v00();
    unsigned char _pad_0x04[0x38 - 0x04];
    CHelperPointMap HelperPoints;          // +0x38
    CDummyObjectMap DummyObjects;          // +0x48
    unsigned char _pad_0x58[0x74 - 0x58];
    char* PackedNames;                     // +0x74
    unsigned short PackedNamesSize;        // +0x78
    bool AnimatedFlag;                     // +0x7A
    FABLE_FORCEINLINE long GetNoHelperPoints(const CCharString& name)
    {
        unsigned long crc = CCRC::Calc(0, name.GetText(), name.GetLength());
        return HelperPoints.UpperBound(crc) - HelperPoints.LowerBound(crc);
    }
    FABLE_FORCEINLINE long GetNoDummyObjects(const CCharString& name)
    {
        unsigned long crc = CCRC::Calc(0, name.GetText(), name.GetLength());
        return DummyObjects.UpperBound(crc) - DummyObjects.LowerBound(crc);
    }
    void UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once);
};

void C3DMeshStats::UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once)
{
    if (!PackedNames)
        return;
    const char* text = PackedNames;
    unsigned long dummyNamesOffset = *(const unsigned short*)text;
    text += sizeof(unsigned short);
    if (dummyObjects)
        text = PackedNames + dummyNamesOffset;
    long prefixLength = prefix.GetLength();
    unsigned long length;
    while ((length = strlen(text)) != 0) {
        if (prefixLength == 0 || strncmp(text, prefix.GetText(), prefixLength) == 0) {
            CCharString name(text);
            long count = 1;
            if (!once) {
                if (dummyObjects)
                    count = GetNoDummyObjects(name);
                else
                    count = GetNoHelperPoints(name);
            }
            for (long i = 0; i < count; ++i)
                names.push_back(name);
        }
        text += length + 1;
    }
}