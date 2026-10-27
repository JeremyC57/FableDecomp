// CPersistContext::TransferVector<NInventory::CItem>(const char* name, std::vector<NInventory::CItem>& value) -- persist one vector member: CRC-check the key, then by mode text load, text save, binary load, binary save, or empty it
// Leaf: ??$TransferVector@VCItem@NInventory@@@CPersistContext@@QAEXPBDAAV?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C2B8B, 135 bytes, thiscall, `ret 8`, ebp frame.
// Identity (proven): the text arms store vtable 0x01253CA4, whose RTTI is .?AV?$CPersistContext_TransferableVector@VCItem@NInventory@@@@;
//   Ego_r.pdb ??$TransferVector@VCItem@NInventory@@@CPersistContext@@QAEXPBDAAV?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@@Z; called from CTCInventoryBase::OnSerialise 0x005BFD5E (four calls). The manifest's
//   "TransferVector<unsigned_long>" label is propagated across byte-identical bodies.
// Callees: CPersistContext::CheckCRC 0x00404500, vector<NInventory::CItem>::erase(first, last) 0x005C1ECC (out of line),
//   GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8 and GFSerialiseVectorBinaryOut<NInventory::CItem> 0x005C12A1 (both `__fastcall` stream,
//   vector), CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText 0x009BAC80.
// Shape (family M143, representative 0x005815D6): the frame-mode sibling of the landed TransferVector<ECategory>
//   0x00619890 -- here the mode switch keeps a real default (STLport clear() = `erase(begin(), end())`, the erase out of
//   line), so VC7.1 emits the sub/dec compare chain instead of a table; `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDataOutputStream;
class CMemoryDataInputStream;
namespace NInventory {
class CItem {                                             // 0x14 bytes in retail
public:
    unsigned long Members[5];
};
}
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last);                // out of line
    void clear() { erase(begin(), end()); }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& in, std::vector<T>& value);
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& out, std::vector<T>& value);
class CPersistContext;
class CStringParser;
class CCharString;
class CPersistContext_TransferableVectorGenericType {
public:
    virtual long Size() = 0;
    virtual void Resize(long count) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;
};
template <class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T>& v) : TransferableVector(&v) { }
private:
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
    std::vector<T>* TransferableVector;                   // +0x4
};
class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    Mode;                         // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28
    void CheckCRC(const char* name);                      // 0x00404500
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BAC80
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BB110
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
template <class T>
void CPersistContext::TransferVector(const char* name, std::vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 0: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorLoadText(name, item);
        break;
    }
    case 1: {
        CPersistContext_TransferableVector<T> item(value);
        TransferVectorSaveText(name, item);
        break;
    }
    case 2:
        GFSerialiseVectorBinaryIn(*PLoadStream, value);
        break;
    case 3:
        GFSerialiseVectorBinaryOut(*PSaveStream, value);
        break;
    default:
        value.clear();
        break;
    }
}
template void CPersistContext::TransferVector<NInventory::CItem>(const char* name, std::vector<NInventory::CItem>& value);