// Behaviour test for 00cba3e0: placement-copy pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>> entries, add-ref each non-null count block
#include <cstdio>
#include <new>

struct CActiveEntityScriptBase { int Id; };
struct SCountBlock { long RefCount; };

struct CCountedPointer_CActiveEntityScriptBase {
    CActiveEntityScriptBase* Data;
    SCountBlock* Info;
    CCountedPointer_CActiveEntityScriptBase() : Data(0), Info(0) { }
    CCountedPointer_CActiveEntityScriptBase(const CCountedPointer_CActiveEntityScriptBase& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct SEntityScriptEntry {
    unsigned int KeyLow;
    unsigned int KeyHigh;
    CCountedPointer_CActiveEntityScriptBase Script;
    SEntityScriptEntry() : KeyLow(0), KeyHigh(0) { }
    SEntityScriptEntry(const SEntityScriptEntry& other)
        : KeyLow(other.KeyLow), KeyHigh(other.KeyHigh), Script(other.Script) { }
};

struct FalseType { FalseType() { } };

SEntityScriptEntry* UninitCopy(const SEntityScriptEntry* first, const SEntityScriptEntry* last, SEntityScriptEntry* result, const FalseType&)
{
    for (; first != last; ++first, ++result)
        new (result) SEntityScriptEntry(*first);
    return result;
}

int main()
{
    CActiveEntityScriptBase scripts[3] = { { 1 }, { 2 }, { 3 } };
    SCountBlock counts[3] = { { 1 }, { 5 }, { 9 } };
    SEntityScriptEntry src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].KeyLow = 0x1000u + i;
        src[i].KeyHigh = 0x2000u + i;
        src[i].Script.Data = &scripts[i];
        src[i].Script.Info = (i == 1) ? 0 : &counts[i];
    }
    void* raw = ::operator new(3 * sizeof(SEntityScriptEntry));
    SEntityScriptEntry* dst = static_cast<SEntityScriptEntry*>(raw);
    SEntityScriptEntry* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3;
    for (int i = 0; i < 3; ++i) {
        ok = ok && dst[i].KeyLow == 0x1000u + i && dst[i].KeyHigh == 0x2000u + i;
        ok = ok && dst[i].Script.Data == &scripts[i] && dst[i].Script.Info == src[i].Script.Info;
    }
    ok = ok && counts[0].RefCount == 2 && counts[1].RefCount == 5 && counts[2].RefCount == 10;
    ok = ok && UninitCopy(src, src, dst, FalseType()) == dst;
    ::operator delete(raw);
    if (!ok) { std::printf("00cba3e0_TEST FAIL\n"); return 1; }
    std::printf("00cba3e0_TEST PASS\n");
    return 0;
}