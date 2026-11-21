// C3DMeshStats::UnpackNames(std::vector<CCharString>&, bool, const CCharString&, bool) const -- append every packed helper-point (or dummy-object) name that starts with the prefix, once per object stored under it
// Leaf: ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_NABVCCharString@@1@Z
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
// Name fix (NF212, 2026-11-21, q6332 / q6341): the header-less __false_type stand-in and the stand-in vector's own push_back and
//   same-TU growth body moved onto the shared STLport vector (stlport/fable_containers.h). The growth body is STLport's own,
//   visible and out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, with the owner row 0x00433530's
//   FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE so its pick-any copy is the owner's), so push_back's growth call binds the
//   protected IAE vector<CCharString>::_M_insert_overflow.
// Name fix (NF214, 2026-11-21, q12343): the self leaf is the PDB spelling, private with a const `this` over
//   std::vector<CCharString>& (FableWin.pdb 0x031E4CD0 / Ego_r.pdb 0x00B8E770
//   ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_NABVCCharString@@1@Z; was the
//   stand-in ?UnpackNames@C3DMeshStats@@QAEXAAUCCharStringVector@@_NABVCCharString@@1@Z), with its callers 0x00A2F0F0 / 0x00A2F120 /
//   0x00A2F150 on engine/C3DMeshStats.h (0x00A2F170 already bound it). The transitional CCharStringVector wrapper is gone; the local
//   CCharString / CCRC / CMatrix3x4 models moved onto fable_string.h (FABLE_RETAIL_CCHARSTRING: GetAsCharArray now binds
//   ?NullChar@CCharString@@1DB 0x0129AAF4 where the local model bound an in-object "" literal), engine/CCRC.h and engine/CMatrix3x4.h.
//   Still local (no shared header): the C3DMeshStats model (engine/C3DMeshStats.h types the two maps only under
//   FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA, whose engine/CPreAllocVectorMap.h / engine/C3DHelperPoint.h exist only in
//   candidates/manual/pending/2026-11-08_linux_hand868/hdr) and the CHelperPointMap / CDummyObjectMap stand-ins with their same-TU
//   bound searches. The two map members are `mutable` (stand-in device): the const UnpackNames keeps the HEAD non-const stand-in
//   bound-search symbols, which landed siblings bind, unchanged.
// Layout: C3DMeshStats retail offsets as in the landed SaveBinary 0x00A2EAA0 / GetHelperPoint 0x00A2E8C0:
// HelperPoints +0x38, DummyObjects +0x48, PackedNames +0x74.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "stlport/fable_containers.h"
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

#include "engine/CCRC.h"

#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"

#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
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
    mutable CHelperPointMap HelperPoints;  // +0x38 (stand-in device: the header-less map stand-ins keep their non-const bound searches under the const UnpackNames)
    mutable CDummyObjectMap DummyObjects;  // +0x48
    unsigned char _pad_0x58[0x74 - 0x58];
    char* PackedNames;                     // +0x74
    unsigned short PackedNamesSize;        // +0x78
    bool AnimatedFlag;                     // +0x7A
    FABLE_FORCEINLINE long GetNoHelperPoints(const CCharString& name) const
    {
        unsigned long crc = CCRC::Calc(0, name.GetAsCharArray(), name.GetLength());
        return HelperPoints.UpperBound(crc) - HelperPoints.LowerBound(crc);
    }
    FABLE_FORCEINLINE long GetNoDummyObjects(const CCharString& name) const
    {
        unsigned long crc = CCRC::Calc(0, name.GetAsCharArray(), name.GetLength());
        return DummyObjects.UpperBound(crc) - DummyObjects.LowerBound(crc);
    }
private:
    void UnpackNames(std::vector<CCharString>& names, bool dummyObjects, const CCharString& prefix, bool once) const;
};

void C3DMeshStats::UnpackNames(std::vector<CCharString>& names, bool dummyObjects, const CCharString& prefix, bool once) const
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
        if (prefixLength == 0 || strncmp(text, prefix.GetAsCharArray(), prefixLength) == 0) {
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