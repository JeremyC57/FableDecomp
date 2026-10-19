// CBankFile::CreateSymbolMap(CSymbolMap&, bool) -- add valid, non-array entry names with their type prefix and index.
// Retail: 0x009CC530, 262 bytes. Leaf: CreateSymbolMap
// CBankFile.h donor CArrays are 16 bytes, retail 12: Symbols +8, RuntimeData +20,
// PackedDataType +58, PackedValid +68. Local typed layout follows PackRuntimeData.
// Virtual slot19 at vtable0129B54C is GetBankHandle 009CBD10; its type parameter is
// ignored by the base body but passed through for derived type-specific prefixes.
// Callees: CSymbolMap Clear009E5250, AddSymbolDirty009E54E0, Finalise009E5C30;
// CCharString constructors0099EBF0/0099EC30, Find0099E5A0, destructor0099EAE0;
// CPackedUIntArray index00A629F0. Missing-name literal0129A03C is "(Unknown)".
struct CCharString {
    void* PStringData;
    CCharString(const char*, long); //0099EBF0
    CCharString(const CCharString&); //0099EC30
    ~CCharString(); //0099EAE0
    long Find(char) const; //0099E5A0
};
struct CSymbolMap {
    void Clear(); //009E5250
    void AddSymbolDirty(const CCharString&, const CCharString&, long); //009E54E0
    void Finalise(); //009E5C30
};
template<class T> struct CArray { T* First; T* Last; T* Limit; bool empty() const { return First==Last; } T& operator[](unsigned long i) { return First[i]; } };
struct CRuntimeData { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    unsigned long* PackedInts; unsigned long Size; unsigned char Bits; unsigned long Bias;
    unsigned long operator[](unsigned long) const; //00A629F0
};
struct CBankFile {
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
        return RuntimeData.First[i].Valid;
    }
    CCharString GetSymbol(unsigned long i) {
        if (Symbols.empty()) return CCharString("(Unknown)",-1);
        return Symbols[i];
    }
    unsigned long GetDataType(unsigned long i) const {
        if (PackedDataType.Size) return PackedDataType[i-1];
        return RuntimeData.First[i].DataType;
    }
    void CreateSymbolMap(CSymbolMap&, bool);
};
void CBankFile::CreateSymbolMap(CSymbolMap& map, bool append)
{
    if (!append) map.Clear();
    for (unsigned long i=0;i<Size;++i) {
        if (IsValid(i)) {
            CCharString name=GetSymbol(i);
            if (name.Find('[')==-1) {
                CCharString prefix=GetBankHandle(GetDataType(i));
                map.AddSymbolDirty(prefix,name,i);
            }
        }
    }
    map.Finalise();
}
void CBankFile::Slot00() {}
void CBankFile::Slot01() {}
void CBankFile::Slot02() {}
void CBankFile::Slot03() {}
void CBankFile::Slot04() {}
void CBankFile::Slot05() {}
void CBankFile::Slot06() {}
void CBankFile::Slot07() {}
void CBankFile::Slot08() {}
void CBankFile::Slot09() {}
void CBankFile::Slot10() {}
void CBankFile::Slot11() {}
void CBankFile::Slot12() {}
void CBankFile::Slot13() {}
void CBankFile::Slot14() {}
void CBankFile::Slot15() {}
void CBankFile::Slot16() {}
void CBankFile::Slot17() {}
void CBankFile::Slot18() {}

#include <stdio.h>
#include <string.h>
static CBankFile* active;
static CSymbolMap* activeMap;
static int liveStrings, baseline, invalid, clears, finals, adds, handles, validReads, typeReads, finds;
static unsigned long types[8];
static long values[8];
static const char* names[8];
static bool shorten;
CCharString::CCharString(const char* s,long n) { if(n!=-1) ++invalid; PStringData=(void*)s; ++liveStrings; }
CCharString::CCharString(const CCharString& other) { PStringData=other.PStringData; ++liveStrings; }
CCharString::~CCharString() { --liveStrings; }
long CCharString::Find(char c) const {
    ++finds; if(c!='[') ++invalid;
    const char* s=(const char*)PStringData; const char* p=strchr(s,c); return p ? (long)(p-s) : -1;
}
void CSymbolMap::Clear() { if(this!=activeMap || finals || adds || handles) ++invalid; ++clears; }
void CSymbolMap::Finalise() { if(this!=activeMap || liveStrings!=baseline || finals) ++invalid; ++finals; }
void CSymbolMap::AddSymbolDirty(const CCharString& prefix,const CCharString& name,long value) {
    if(this!=activeMap || finals || adds>=8 || strcmp((const char*)prefix.PStringData,"TYPE")!=0 || handles!=adds+1) ++invalid;
    values[adds]=value; names[adds]=(const char*)name.PStringData; ++adds;
    if(shorten) active->Size=2;
}
unsigned long CPackedUIntArray::operator[](unsigned long i) const {
    if(this==&active->PackedValid) ++validReads;
    else if(this==&active->PackedDataType) ++typeReads;
    else ++invalid;
    if(i>=Size) { ++invalid; return 0; }
    return PackedInts[i];
}
CCharString CBankFile::GetBankHandle(unsigned long) { ++invalid; return CCharString("BASE",-1); }
struct Bank : CBankFile {
    virtual CCharString GetBankHandle(unsigned long type) {
        if(this!=active || handles>=8 || finals) ++invalid;
        types[handles++]=type;
        return CCharString("TYPE",-1);
    }
};
static void reset(Bank& bank, CSymbolMap& map,CCharString* symbols,CRuntimeData* runtime) {
    active=&bank; activeMap=&map; baseline=liveStrings;
    invalid=clears=finals=adds=handles=validReads=typeReads=finds=0; shorten=false;
    bank.Size=5; bank.Symbols.First=symbols; bank.Symbols.Last=symbols+5; bank.Symbols.Limit=symbols+5;
    bank.RuntimeData.First=runtime; bank.RuntimeData.Last=runtime+5; bank.RuntimeData.Limit=runtime+5;
    bank.PackedValid.Size=bank.PackedDataType.Size=0;
}
int main() {
    Bank bank; CSymbolMap map;
    CCharString symbols[5]={CCharString("sentinel",-1),CCharString("apple",-1),CCharString("array[0]",-1),CCharString("unused",-1),CCharString("banana",-1)};
    CRuntimeData runtime[5];
    for(int i=0;i<5;++i) { runtime[i].Valid=i!=3; runtime[i].DataType=(unsigned char)(10+i); }
    unsigned long packedValid[4]={2,1,0,1}, packedTypes[4]={31,32,33,34};
    int ok=1;
    // Packed validity and type sources are independent switches.
    for(int mode=0;mode<4;++mode) {
        reset(bank,map,symbols,runtime);
        if(mode&1) { bank.PackedValid.Size=4; bank.PackedValid.PackedInts=packedValid; }
        if(mode&2) { bank.PackedDataType.Size=4; bank.PackedDataType.PackedInts=packedTypes; }
        bank.CreateSymbolMap(map,false);
        ok=ok && invalid==0 && clears==1 && finals==1 && adds==2 && handles==2 && finds==3
            && values[0]==1 && values[1]==4 && strcmp(names[0],"apple")==0 && strcmp(names[1],"banana")==0
            && types[0]==(unsigned long)((mode&2)?31:11) && types[1]==(unsigned long)((mode&2)?34:14)
            && validReads==((mode&1)?4:0) && typeReads==((mode&2)?2:0) && liveStrings==baseline;
    }
    reset(bank,map,symbols,runtime); bank.CreateSymbolMap(map,true);
    ok=ok && invalid==0 && clears==0 && finals==1 && adds==2;
    reset(bank,map,symbols,runtime); bank.Symbols.Last=bank.Symbols.First;
    bank.CreateSymbolMap(map,false);
    ok=ok && invalid==0 && adds==3 && values[0]==1 && values[1]==2 && values[2]==4
        && strcmp(names[0],"(Unknown)")==0 && strcmp(names[1],"(Unknown)")==0 && strcmp(names[2],"(Unknown)")==0;
    reset(bank,map,symbols,runtime); bank.Size=0; bank.CreateSymbolMap(map,false);
    ok=ok && invalid==0 && clears==1 && finals==1 && adds==0 && finds==0;
    reset(bank,map,symbols,runtime); bank.Size=1; bank.CreateSymbolMap(map,true);
    ok=ok && invalid==0 && clears==0 && finals==1 && adds==0 && validReads==0 && finds==0;
    reset(bank,map,symbols,runtime); shorten=true; bank.CreateSymbolMap(map,false);
    ok=ok && invalid==0 && adds==1 && handles==1 && finals==1;
    printf("009cc530_TEST %s\n",ok?"PASS":"FAIL"); return ok?0:1;
}