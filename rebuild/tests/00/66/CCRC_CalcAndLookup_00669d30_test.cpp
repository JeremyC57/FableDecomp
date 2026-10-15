#include <cstdio>
struct CCharStringData { const char* PData; unsigned long StringLength; };
const char g_EmptyString[] = "";
struct CCharString {
    CCharStringData* PStringData;
    unsigned long GetLength() const { if (PStringData == 0) return 0; return PStringData->StringLength; }
    const char* GetText() const { if (PStringData == 0) return g_EmptyString; return PStringData->PData; }
};
static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    unsigned long crc = seed; const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) { crc ^= p[i]; for (int k = 0; k < 8; ++k) crc = (crc >> 1) ^ (0xEDB88320UL & (0UL - (crc & 1))); }
    return crc;
}
struct SCRCMapNode { long Color; SCRCMapNode* Parent; SCRCMapNode* Left; SCRCMapNode* Right; unsigned long Key; long Value; };
struct SCRCMapIterator { SCRCMapNode* Node; };
struct CCRCMap {
    SCRCMapNode* Header; unsigned long NodeCount;
    SCRCMapIterator* Find(SCRCMapIterator* result, const unsigned long& key)
    {
        SCRCMapNode* y = Header; SCRCMapNode* x = Header->Parent;
        while (x) { if (!(x->Key < key)) { y = x; x = x->Left; } else x = x->Right; }
        result->Node = (y == Header || key < y->Key) ? Header : y;
        return result;
    }
};
struct CCRC {
    CCRCMap Map;
    bool CalcAndLookup(const CCharString& name, long* value)
    {
        unsigned long crc = Calc(0, name.GetText(), name.GetLength());
        SCRCMapIterator it;
        Map.Find(&it, crc);
        if (it.Node != Map.Header) { *value = it.Node->Value; return true; }
        return false;
    }
};
int main()
{
    CCharStringData d; d.PData = "SWORD"; d.StringLength = 5;
    CCharString s; s.PStringData = &d;
    SCRCMapNode header, node;
    node.Key = Calc(0, "SWORD", 5); node.Value = 42; node.Left = node.Right = 0; node.Parent = &header;
    header.Parent = &node; header.Left = header.Right = &node;
    CCRC c; c.Map.Header = &header; c.Map.NodeCount = 1;
    long v = -1;
    if (!c.CalcAndLookup(s, &v) || v != 42) { std::printf("00669d30_TEST FAIL\n"); return 1; }
    d.PData = "AXE"; d.StringLength = 3; v = -1;
    if (c.CalcAndLookup(s, &v) || v != -1) { std::printf("00669d30_TEST FAIL\n"); return 1; }
    CCharString empty; empty.PStringData = 0;
    if (c.CalcAndLookup(empty, &v) || v != -1) { std::printf("00669d30_TEST FAIL\n"); return 1; }
    std::printf("00669d30_TEST PASS\n");
    return 0;
}