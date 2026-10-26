// CBankFile::FindIndexByFilename(const CCharString&, long) -- find a normalized basename, optionally restricting its entry type.
// Retail: 0x009CCDF0, 242 bytes. Leaf: FindIndexByFilename
// CBankFile.h donor arrays/maps are 16 bytes versus retail12. RuntimeData+20,
// PackedDataType+58 and FilenameIndices+c4 therefore require this retail model.
// Shared fable_string.h supplies CCharString; ToLower is missing and declared in a
// method-only view of the same one-pointer object. Calls ConvertPathToFilename009CBE10,
// ToLower0099ED10, destructor0099EAE0, upper_bound009D2B50, lower_bound0042AD34,
// packed lookup00A629F0. Node color/parent/left/right/key/value at0/4/8/c/10/14.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
struct CCharStringLowerView { void* PStringData; CCharString ToLower() const; }; //0099ED10
inline CCharString ToLower(const CCharString& s) { return reinterpret_cast<const CCharStringLowerView&>(s).ToLower(); }
struct CFilenameNode {
    long Color; CFilenameNode* Parent; CFilenameNode* Left; CFilenameNode* Right;
    CCharString Key; unsigned long Value;
};
struct CFilenameMap {
    CFilenameNode* Head; unsigned long Count; unsigned char Compare;
    CFilenameNode* UpperBound(const CCharString&); //009D2B50
    CFilenameNode* LowerBound(const CCharString&); //0042AD34
};
inline CFilenameNode* NextFilenameNode(CFilenameNode* p) {
    if (p->Right) { p=p->Right; while(p->Left) p=p->Left; }
    else { CFilenameNode* parent=p->Parent; while(p==parent->Right) {p=parent;parent=parent->Parent;} if(p->Right!=parent) p=parent; }
    return p;
}
struct CRuntimeData { unsigned long DataOffset,DataSize; unsigned char DataType; bool Valid; };
template<class T> class CArray { public: T* First;T* Last;T* Limit; };
struct CPackedUIntArray {
    unsigned long* PackedInts;unsigned long Size;unsigned char Bits;unsigned long Bias;
    unsigned long operator[](unsigned long) const; //00A629F0
};
class CBankFile { public:
    void* VTable;unsigned long Size;
    CArray<CCharString> Symbols; CArray<unsigned long> Checksums;
    CArray<CRuntimeData> RuntimeData; CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset,PackedDataSize,PackedDataType,PackedValid;
    unsigned char _otherBankState[0x4c];
    CFilenameMap FilenameIndices;
    CCharString ConvertPathToFilename(const CCharString&) const; //009CBE10
    unsigned long GetDataType(unsigned long i) const { if(PackedDataType.Size) return PackedDataType[i-1];return RuntimeData.First[i].DataType; }
    unsigned long FindIndexByFilename(const CCharString&,long);
};
unsigned long CBankFile::FindIndexByFilename(const CCharString& path,long type)
{
    CCharString name=reinterpret_cast<const CCharStringLowerView&>(static_cast<const CCharString&>(ConvertPathToFilename(path))).ToLower();
    CFilenameNode* end=FilenameIndices.UpperBound(name);
    CFilenameNode* p=FilenameIndices.LowerBound(name);
    for(;p!=end;p=NextFilenameNode(p)) {
        unsigned long index=p->Value;
        if(type==-1 || GetDataType(index)==(unsigned long)type) return index;
    }
    return 0;
}