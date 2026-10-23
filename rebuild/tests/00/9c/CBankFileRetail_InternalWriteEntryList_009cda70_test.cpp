// 009cda70 behaviour test: CBankFile::InternalWriteEntryList (match.cpp body: CArray size() as end() - begin(),
// operator[] as *(begin() + index)), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CDataOutputStream {
    std::vector<std::string> Log;
    unsigned long Pos;
    unsigned long GetPosition() { return Pos; }
    void Write(const void* data, long size) { Log.push_back("raw:" + std::string((const char*)data, size)); Pos += size; }
    void WriteULong(unsigned long value) { char b[24]; sprintf(b, "%lu", value); Log.push_back(b); Pos += 4; }
    void WritePresizedString(const CCharString& s) { Log.push_back("s:" + s.S); Pos += 4 + (unsigned long)s.S.size(); }
};
struct CByteVector {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CByteVector(unsigned long n, char value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (char*)malloc(n) : 0;
        _M_end_of_storage = _M_start + n;
        char* first = _M_start;
        if (n) memset(first, value, n);
        _M_finish = first + n;
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    long GetCount() const { return _M_finish - _M_start; }
    char* GetData() { return _M_finish - _M_start != 0 ? _M_start : (char*)this; }
};
struct CBankFileEntryUpdateData {
    unsigned long StateBlockCRC;
    unsigned long InfoSize;
    char*         InfoBuffer;
    CCharString*  Filenames_Start;
    signed char   Filenames_Size;
    bool          RequiresUpdate;
    void ReadInfo(unsigned long start, void* data, unsigned long size) const
    {
        i64 end = (i64)(start + size);
        if (end >= 0 && end <= 0x7fffffff)
            memcpy(data, InfoBuffer + start, size);
    }
};
struct CRuntimeData {
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool          Valid;
};
struct CPackedUIntArray {
    unsigned long* m_pData;
    unsigned long operator[](unsigned long index) const { return m_pData[index]; }
    bool IsPacked() const { return m_pData != 0; }
};
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* begin() const { return m_pFirst; }
    T* end() const { return m_pLast; }
    unsigned long size() const { return end() - begin(); }
    bool empty() const { return m_pFirst == m_pLast; }
    T& operator[](unsigned long index) const { return *(begin() + index); }
    void set(std::vector<T>& v) { m_pFirst = v.empty() ? 0 : &v[0]; m_pLast = m_pFirst + v.size(); }
};
struct SULongPair { unsigned long first; unsigned long second; };
struct SULongNode {
    SULongNode*   parent;
    SULongNode*   left;
    SULongNode*   right;
    SULongPair    value;
};
// map model: a right-leaning chain in key order under the STLport header (header->left = first, header->right = last)
struct CULongMap {
    SULongNode*   _M_header;
    unsigned long _M_node_count;
    CULongMap() : _M_node_count(0)
    {
        _M_header = new SULongNode;
        _M_header->parent = 0; _M_header->left = _M_header; _M_header->right = _M_header;
    }
    ~CULongMap()
    {
        SULongNode* x = _M_header->parent;
        while (x) { SULongNode* n = x->right; delete x; x = n; }
        delete _M_header;
    }
    void rebuild(std::vector<SULongNode*>& v)
    {
        for (size_t k = 0; k < v.size(); ++k) {
            v[k]->left = 0;
            v[k]->right = k + 1 < v.size() ? v[k + 1] : 0;
            v[k]->parent = k ? v[k - 1] : _M_header;
        }
        _M_header->parent = v[0]; _M_header->left = v[0]; _M_header->right = v.back();
    }
    void insert(const SULongPair& value)
    {
        std::vector<SULongNode*> v;
        for (SULongNode* x = _M_header->parent; x; x = x->right) { if (x->value.first == value.first) return; v.push_back(x); }
        SULongNode* n = new SULongNode; n->value = value;
        size_t k = 0; while (k < v.size() && v[k]->value.first < value.first) ++k;
        v.insert(v.begin() + k, n); ++_M_node_count; rebuild(v);
    }
    unsigned long& operator[](const unsigned long& key)
    {
        for (SULongNode* x = _M_header->parent; x; x = x->right) if (x->value.first == key) return x->value.second;
        SULongPair p = { key, 0 }; insert(p); return (*this)[key];
    }
    static SULongNode* _M_increment(SULongNode* x)
    {
        if (x->right != 0) {
            x = x->right;
            while (x->left != 0)
                x = x->left;
        } else {
            SULongNode* y = x->parent;
            while (x == y->right) {
                x = y;
                y = y->parent;
            }
            if (x->right != y)
                x = y;
        }
        return x;
    }
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CBankFile_Retail {
    CArray<CCharString>       Symbols;
    CArray<unsigned long>     Checksums;
    CArray<CRuntimeData>      RuntimeData;
    CArray<CBankFileEntryUpdateData*> UpdateData;
    CPackedUIntArray          PackedDataOffset;
    CPackedUIntArray          PackedDataSize;
    CPackedUIntArray          PackedDataType;
    unsigned long             FileInformationSize;
    unsigned long             FileSize;
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    unsigned long GetEntryDataSize(unsigned long index) const
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData[index].DataType;
    }
    CCharString GetEntrySymbol(unsigned long index) const
    {
        if (Symbols.empty())
            return CCharString("(Unknown)", -1);
        return Symbols[index];
    }
    unsigned long InternalWriteEntryList(CDataOutputStream& stream);
};

unsigned long CBankFile_Retail::InternalWriteEntryList(CDataOutputStream& stream)
{
    unsigned long dataEnd = stream.GetPosition();
    unsigned long count = 0;
    unsigned long maxIndex = 0;
    CULongMap typeCounts;
    for (unsigned long i = 0; i < UpdateData.size(); ++i) {
        if (RuntimeData[i].Valid && RuntimeData[i].DataOffset != 0 && !UpdateData[i]->RequiresUpdate) {
            ++count;
            unsigned long type = GetEntryDataType(i);
            SULongPair value;
            value.first = type;
            value.second = 0;
            typeCounts.insert(value);
            ++typeCounts[type];
            unsigned long end = GetEntryDataOffset(i) + GetEntryDataSize(i);
            dataEnd = Max(dataEnd, end);
            if (maxIndex < i)
                maxIndex = i;
        }
    }
    stream.WriteULong(typeCounts._M_node_count);
    for (SULongNode* it = typeCounts._M_header->left; it != typeCounts._M_header; it = CULongMap::_M_increment(it)) {
        stream.WriteULong(it->value.first);
        stream.WriteULong(it->value.second);
    }
    for (unsigned long i = 0; i < UpdateData.size(); ++i) {
        if (RuntimeData[i].Valid && RuntimeData[i].DataOffset != 0) {
            CBankFileEntryUpdateData* update = UpdateData[i];
            if (!update->RequiresUpdate) {
                stream.WriteULong(42);
                stream.WriteULong(i);
                stream.WriteULong(GetEntryDataType(i));
                stream.WriteULong(GetEntryDataSize(i));
                stream.WriteULong(GetEntryDataOffset(i));
                stream.WriteULong(update->StateBlockCRC);
                stream.WritePresizedString(GetEntrySymbol(i));
                stream.WriteULong(Checksums[i]);
                stream.WriteULong(update->Filenames_Size);
                for (unsigned long j = 0; j < (unsigned long)update->Filenames_Size; ++j)
                    stream.WritePresizedString(update->Filenames_Start[j]);
                unsigned long infoSize = update->InfoSize;
                CByteVector info(infoSize, 0);
                update->ReadInfo(0, info.GetData(), infoSize);
                stream.WriteULong(infoSize);
                stream.Write(info.GetData(), infoSize);
            }
        }
    }
    FileInformationSize = stream.GetPosition() - dataEnd;
    FileSize = Max(FileSize, stream.GetPosition());
    return count;
}

static std::string join(const std::vector<std::string>& v, size_t from, size_t n)
{
    std::string s;
    for (size_t k = from; k < from + n && k < v.size(); ++k) { if (k > from) s += ","; s += v[k]; }
    return s;
}

int main()
{
    // four entries: 0 written (type 2), 1 invalid, 2 needs update, 3 written (type 1); a fifth written (type 2)
    CRuntimeData rd[5] = { { 100, 20, 2, true }, { 200, 5, 1, false }, { 300, 7, 2, true }, { 150, 30, 1, true }, { 40, 10, 2, true } };
    std::vector<CRuntimeData> runtime(rd, rd + 5);
    CCharString names[2] = { CCharString("a.wav"), CCharString("b.wav") };
    char info0[] = "XY";
    CBankFileEntryUpdateData u0 = { 0xAA, 2, info0, names, 2, false };
    CBankFileEntryUpdateData u1 = { 0, 0, 0, 0, 0, false };
    CBankFileEntryUpdateData u2 = { 0, 0, 0, 0, 0, true };
    CBankFileEntryUpdateData u3 = { 0xBB, 0, 0, 0, 0, false };
    CBankFileEntryUpdateData u4 = { 0xCC, 0, 0, names, 1, false };
    std::vector<CBankFileEntryUpdateData*> upd; upd.push_back(&u0); upd.push_back(&u1); upd.push_back(&u2); upd.push_back(&u3); upd.push_back(&u4);
    std::vector<CCharString> syms; for (int k = 0; k < 5; ++k) { char b[8]; sprintf(b, "SYM%d", k); syms.push_back(CCharString(b)); }
    std::vector<unsigned long> sums; for (int k = 0; k < 5; ++k) sums.push_back(1000 + k);

    CBankFile_Retail bank;
    bank.RuntimeData.set(runtime); bank.UpdateData.set(upd); bank.Symbols.set(syms); bank.Checksums.set(sums);
    bank.PackedDataOffset.m_pData = 0; bank.PackedDataSize.m_pData = 0; bank.PackedDataType.m_pData = 0;
    bank.FileSize = 0; bank.FileInformationSize = 0;

    CDataOutputStream s; s.Pos = 100;
    unsigned long n = bank.InternalWriteEntryList(s);
    CHECK(n == 3);
    // type table: 2 types, sorted: type 1 x1, type 2 x2
    CHECK(join(s.Log, 0, 5) == "2,1,1,2,2");
    // entry 0
    CHECK(join(s.Log, 5, 14) == "42,0,2,20,100,170,s:SYM0,1000,2,s:a.wav,s:b.wav,2,raw:XY,42");
    // entry 3 then entry 4 (entries 1 and 2 skipped)
    CHECK(join(s.Log, 18, 10) == "42,3,1,30,150,187,s:SYM3,1003,0,0");
    CHECK(s.Log[28] == "raw:");
    CHECK(join(s.Log, 29, 11) == "42,4,2,10,40,204,s:SYM4,1004,1,s:a.wav,0");
    // dataEnd = max(start pos 100, 100+20, 150+30, 40+10) = 180
    CHECK(bank.FileInformationSize == s.Pos - 180 && bank.FileSize == s.Pos);

    // FileSize keeps a larger previous value; no symbols -> "(Unknown)"; packed arrays read index - 1
    unsigned long pType[5] = { 7, 7, 7, 7, 7 }, pOff[5] = { 1, 2, 3, 4, 5 }, pSize[5] = { 9, 9, 9, 9, 9 };
    bank.PackedDataType.m_pData = pType; bank.PackedDataOffset.m_pData = pOff; bank.PackedDataSize.m_pData = pSize;
    std::vector<CCharString> none; bank.Symbols.set(none);
    bank.FileSize = 100000;
    runtime[0].Valid = false;                // packed arrays are 1-based: keep entry 0 out
    CDataOutputStream t; t.Pos = 0;
    CHECK(bank.InternalWriteEntryList(t) == 2);
    CHECK(join(t.Log, 0, 3) == "1,7,2");
    CHECK(join(t.Log, 3, 7) == "42,3,7,9,3,187,s:(Unknown)");
    CHECK(join(t.Log, 14, 6) == "42,4,7,9,4,204");
    // dataEnd = max(0, 3+9, 4+9) = 13
    CHECK(bank.FileInformationSize == t.Pos - 13 && bank.FileSize == 100000);

    if (!g_fail) printf("009cda70_TEST PASS\n");
    return g_fail;
}