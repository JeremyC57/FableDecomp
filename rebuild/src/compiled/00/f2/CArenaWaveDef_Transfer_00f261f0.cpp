// CArenaWaveDef::Transfer(CPersistContext& context) -- persist the arena wave def: NumWaveCreatures (default 0), the Creatures sub-def vector, ShortWave (default false)
// Leaf: ?Transfer@CArenaWaveDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F261F0, 86 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 (Transfer) of the CArenaWaveDef vtable 0x012FB2F0, stored by the landed ctor
//   CArenaWaveDef::CArenaWaveDef 0x00F25C60; the slots 1..17 / 21..24 are the shared CDefClassBase/CResource members, and
//   the three persisted names "NumWaveCreatures" / "Creatures" / "ShortWave" are exactly the PDB members of CArenaWaveDef
//   (Ego_r.pdb: NumWaveCreatures +0x28, vector<CArenaCreatureDef> Creatures +0x2c, ShortWave +0x3c).
//   ?Transfer@CArenaWaveDef@@UAEXAAVCPersistContext@@@Z is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f261f0" is a gap-scan placeholder.
// Callees: CPersistContext::Transfer<long> 0x00410BE0, CPersistContext::TransferVectorOfSubComponents<CArenaCreatureDef>
//   0x00F26250, CPersistContext::Transfer<bool> 0x004045C0 (PDB member-template spellings).
// Layout: CDefClassBase is the retail 0x28-byte class of the shared FABLE_RETAIL_DEF_CLASS_BASE headers; NumWaveCreatures +0x28, the 12-byte STLport vector Creatures +0x2c, ShortWave +0x38
//   (Ego_r.pdb +0x3c: its Dinkumware vector is 16 bytes).
// Shape: one Transfer per member in declaration order; the defaults are compiler temporaries (the long one takes the frame
//   word, the bool one the dead context-argument slot).
class CArenaCreatureDef;
namespace std {
template <class T> class allocator { public: };
template <class T, class A = allocator<T> > class vector {     // STLport 4.6 vector: 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // long 0x00410BE0, bool 0x004045C0
    template <class T> void TransferVectorOfSubComponents(const char* name, std::vector<T>& value);   // 0x00F26250
};

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"

class CArenaWaveDef : public CDefClassBase {             // vtable 0x012FB2F0 (no RTTI); slot 0 0x00F25CB0
public:
    long                              NumWaveCreatures;   // +0x28
    std::vector<CArenaCreatureDef>    Creatures;          // +0x2c
    bool                              ShortWave;          // +0x38
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F261F0
    virtual void Copy(const CDefClassBase* other);      // slot 19 0x00F25F50
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F25CA0
};

void CArenaWaveDef::Transfer(CPersistContext& context)
{
    context.Transfer("NumWaveCreatures", NumWaveCreatures, 0L);
    context.TransferVectorOfSubComponents("Creatures", Creatures);
    context.Transfer("ShortWave", ShortWave, false);
}