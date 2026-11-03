#ifndef FABLE_ENGINE_CPACKEDUINTARRAY_RENDER2D_H
#define FABLE_ENGINE_CPACKEDUINTARRAY_RENDER2D_H
#include "rebuild_abi.h"

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CPackedUIntArray {
private: unsigned long * PackedInts;
private: unsigned long Size;
private: unsigned char Bits;
private: unsigned long Bias;
private: static unsigned long BitMasks[33];
public: CPackedUIntArray();
public: ~CPackedUIntArray();
public: void Clear();
public: void Initialise(const void *,unsigned long,long,unsigned long);
public: unsigned long operator[](unsigned long) const;
public: unsigned long GetSize() const;
public: unsigned long GetBits() const;
public: unsigned long Sizeof() const;
private: unsigned long CalculateBias(const void *,unsigned long,long,unsigned long) const;
private: unsigned long CountBits(const void *,unsigned long,long,unsigned long) const;
private: void PackArray(const void *,unsigned long,long,unsigned long);
private: static void ReadNBytes(unsigned long &,const unsigned char *,unsigned long);
private: static unsigned long MakeBitMask(unsigned long);
};
#endif
