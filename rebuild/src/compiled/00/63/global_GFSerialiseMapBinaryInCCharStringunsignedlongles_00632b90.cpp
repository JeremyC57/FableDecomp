// GFSerialiseMapBinaryIn<CCharString,unsigned long,less<unsigned long> >(CMemoryDataInputStream& is, std::map<unsigned long, CCharString>& m) -- binary load of an id-to-string map: clear it, read the entry count, then per entry read the key and the null-terminated string and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCCharString@@KU?$less@K@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@KVCCharString@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00632B90, 253 bytes (+3 int3), __fastcall (stream in ecx, map in edx), plain `ret`; speed TU, `and esp,-8` frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ??$GFSerialiseMapBinaryIn@VCCharString@@KU?$less@K@std@@@@... (this exact spelling); the only
//   caller CPersistContext::TransferMapBinary<unsigned long,CCharString> 0x00632AF0 (load arm) passes CUIDef::ActionMap; the key is read by
//   CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0 and the insert is _Rb_tree<unsigned long,pair<const unsigned long,CCharString>,...>
//   ::insert_unique 0x00632C90 (both naming wave 1 decisions). The catalog label <CCharString,unsigned long,less<unsigned long> > is right.
// Callees: CCharString::CCharString() 0x0099E4B0, _Rb_tree<...>::clear() 0x00630FE0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0, CDataInputStream::ReadNullTerminatedString (virtual slot 6),
//   CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, CCharString::CCharString(const CCharString&) 0x0099EC30,
//   _Rb_tree<...>::insert_unique(const value_type&) 0x00632C90.
// Shape: the shared engine/CPersistTraits.h body (FABLE_RETAIL_GFSERIALISEMAPBINARYIN) with the visible generic traits body
//   (FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY: the key read stays a call but cannot capture the empty key traits, which shares the
//   value traits' frame word) and the inline CCharString traits read (FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CCharString, unsigned long, std::less<unsigned long> >(CMemoryDataInputStream& is, std::map<unsigned long, CCharString, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CCharString> > >& m);