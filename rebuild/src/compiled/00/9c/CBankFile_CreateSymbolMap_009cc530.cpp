// CBankFile::CreateSymbolMap(CSymbolMap&, bool) -- add valid, non-array entry names with their type prefix and index.
// Plan phase 5 (2026-11-05), CArray wave: the local CArray stand-in replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>). Also: the local CArray's pointer members First/Last are the header CArray's (public STLport vector<T>) begin() / end() (STLport vector start +0 / finish +4).
// Retail: 0x009CC530, 262 bytes. Leaf: CreateSymbolMap
// Naming wave 27 (family dm, chunk 695): 0x009E5C30 is CSymbolMap::UpdateDirty() const (Ego_r.pdb + FableWin.pdb); was
//   an invented ?Finalise@CSymbolMap (no Finalise in either PDB).
// CBankFile.h donor CArrays are 16 bytes, retail 12: Symbols +8, RuntimeData +20,
// PackedDataType +58, PackedValid +68. Local typed layout follows PackRuntimeData.
// Virtual slot19 at vtable0129B54C is GetBankHandle 009CBD10; its type parameter is
// ignored by the base body but passed through for derived type-specific prefixes.
// Callees: CSymbolMap Clear009E5250, AddSymbolDirty009E54E0, Finalise009E5C30;
// CCharString constructors0099EBF0/0099EC30, Find0099E5A0, destructor0099EAE0;
// CPackedUIntArray index00A629F0. Missing-name literal0129A03C is "(Unknown)".
class CCharString { public:
    void* PStringData;
    CCharString(const char*, long); //0099EBF0
    CCharString(const CCharString&); //0099EC30
    ~CCharString(); //0099EAE0
public:
    long FindCharPos(char) const;   // 0x0099E5A0
};
class CSymbolMap { public:
    void Clear(); //009E5250
    void AddSymbolDirty(const CCharString&, const CCharString&, long); //009E54E0
    void UpdateDirty() const;   // 0x009E5C30 (Ego_r.pdb / FableWin.pdb ?UpdateDirty@CSymbolMap@@QBEXXZ)
};
#include "engine/CArray.h"
struct CRuntimeData { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    unsigned long* PackedInts; unsigned long Size; unsigned char Bits; unsigned long Bias;
    unsigned long operator[](unsigned long) const; //00A629F0
};
class CBankFile { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual void Slot04(); virtual void Slot05(); virtual void Slot06(); virtual void Slot07();
    virtual void Slot08(); virtual void Slot09(); virtual void Slot10(); virtual void Slot11();
    virtual void Slot12(); virtual void Slot13(); virtual void Slot14(); virtual void Slot15();
    virtual void Slot16(); virtual void Slot17(); virtual void Slot18();
    virtual CCharString GetBankHandle(unsigned long type); //009CBD10 slot19
    unsigned long Size;
    CArray<CCharString> Symbols;
    CArray<unsigned long> Checksums;
    CArray<CRuntimeData> RuntimeData;
    CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    bool IsValid(unsigned long i) const {
        if (i==0 || i>=Size) return false;
        if (PackedValid.Size) return PackedValid[i-1]!=0;
        return RuntimeData.begin()[i].Valid;
    }
    CCharString GetSymbol(unsigned long i) {
        if (Symbols.empty()) return CCharString("(Unknown)",-1);
        return Symbols[i];
    }
    unsigned long GetDataType(unsigned long i) const {
        if (PackedDataType.Size) return PackedDataType[i-1];
        return RuntimeData.begin()[i].DataType;
    }
    void CreateSymbolMap(CSymbolMap&, bool);
};
void CBankFile::CreateSymbolMap(CSymbolMap& map, bool append)
{
    if (!append) map.Clear();
    for (unsigned long i=0;i<Size;++i) {
        if (IsValid(i)) {
            CCharString name=GetSymbol(i);
            if (name.FindCharPos('[')==-1) {
                CCharString prefix=GetBankHandle(GetDataType(i));
                map.AddSymbolDirty(prefix,name,i);
            }
        }
    }
    map.UpdateDirty();
}