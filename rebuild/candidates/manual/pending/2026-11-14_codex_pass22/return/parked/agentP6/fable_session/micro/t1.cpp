#ifndef OPSTAR
#define OPSTAR T& operator*() const { return *Data; }
#endif
// Leaf: ?Go@CDef@@QAEXAAUCPersistContext@@@Z
#define TRAITS_CTOR
#define TRAITS_CTOR2
struct CMemoryDataInputStream; struct CDataOutputStream;
struct CWM { void TransferBinaryIn(CMemoryDataInputStream&); void TransferBinaryOut(CDataOutputStream&) const; int a,b,c,d; };
struct CEX { void TransferBinaryIn(CMemoryDataInputStream&); void TransferBinaryOut(CDataOutputStream&) const; int a,b,c,d; };
struct CRM { int x; };
enum EMode { MODE_TEXT=0, MODE_X=1, MODE_LOAD_BINARY=2, MODE_SAVE_BINARY=3 };
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CWM> { CWM DefaultVal; TRAITS_CTOR
  void TransferBinaryIn(CMemoryDataInputStream& is,CWM& value) { value.TransferBinaryIn(is); }
  void TransferBinaryOut(CDataOutputStream& os,const CWM& value) { value.TransferBinaryOut(os); } };
template<> struct CPersistTraits<CEX> { CEX DefaultVal; TRAITS_CTOR2
  void TransferBinaryIn(CMemoryDataInputStream& is,CEX& value) { value.TransferBinaryIn(is); }
  void TransferBinaryOut(CDataOutputStream& os,const CEX& value) { value.TransferBinaryOut(os); } };
struct CPersistContext {
  void CheckCRC(const char*);
  int pad[6]; EMode Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
  template<class T> void Transfer(T& value);
  void Transfer(CRM&);
};
template<> void CPersistContext::Transfer<CWM>(CWM& value) {
    CheckCRC(""); CPersistTraits<CWM> traits;
    switch (Mode) { case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
                    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break; }
}
template<> void CPersistContext::Transfer<CEX>(CEX& value) {
    CheckCRC(""); CPersistTraits<CEX> traits;
    switch (Mode) { case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
                    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break; }
}
template<class T> struct CP { T* Data; void* Info; OPSTAR };
struct CDef { void Go(CPersistContext& p); CWM* PW; int z; CP<CRM> PR; CEX* PE; };
void CDef::Go(CPersistContext& p) { p.Transfer(*PW); p.Transfer(*PR); p.Transfer(*PE); }
