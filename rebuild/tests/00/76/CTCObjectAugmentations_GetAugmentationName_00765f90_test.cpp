// Behaviour test for 0x00765F90 CTCObjectAugmentations::GetAugmentationName: def index -> thing def -> augmentation
// sub-def; a non-zero Name is looked up in the text bank (entry text returned), a zero Name gives the missing-name
// string; every counted reference taken is released (def, sub-def, group entry).
#include <cstdio>

static int WideEqual(const wchar_t* a, const wchar_t* b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}

class CWideString { public:
    const wchar_t* Text;
    explicit CWideString(const wchar_t* t) : Text(t) { }
    CWideString(const CWideString& o) : Text(o.Text) { }
};
static CWideString gMissing(L"Missing augmentation name!");

struct CCounted {
    long RefCount;
    int Releases;
    CCounted() : RefCount(1), Releases(0) { }
    void ReleaseReference() { if (--RefCount == 0) ++Releases; }
};
class CAugmentationDef : public CCounted { public: unsigned long Name; };
class CThingBaseDef : public CCounted { public:
    CAugmentationDef* Sub;
    bool GetSubDef(CAugmentationDef** out) const { ++Sub->RefCount; *out = Sub; return true; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
};

static CThingBaseDef* gDefs[4];
bool GetDef(long index, CDefPointer<CThingBaseDef>* out) { ++gDefs[index]->RefCount; out->Object = gDefs[index]; return true; }

struct CGroupEntry : CCounted {
    const wchar_t* Text;
    CWideString GetText() const { return CWideString(Text); }
};
static CGroupEntry gEntries[8];
static unsigned long gLastLookup = 0;
struct CEntryPtr {                               // returned as a temporary: VC7.1 has no NRVO
    CGroupEntry* Object;
    explicit CEntryPtr(CGroupEntry* o) : Object(o) { }
    ~CEntryPtr() { if (Object) { Object->ReleaseReference(); Object = 0; } }
    CGroupEntry* operator->() const { return Object; }
};
CEntryPtr GetPGroupEntry(unsigned long index, bool) { gLastLookup = index; ++gEntries[index].RefCount; return CEntryPtr(&gEntries[index]); }

CWideString GetAugmentationName(long defIndex)
{
    CDefPointer<CThingBaseDef> def;
    GetDef(defIndex, &def);
    CDefPointer<CAugmentationDef> augmentation;
    def.Object->GetSubDef(&augmentation.Object);
    if (augmentation.Object->Name > 0)
        return GetPGroupEntry(augmentation.Object->Name, true)->GetText();
    return gMissing;
}

int main()
{
    CAugmentationDef fire; fire.Name = 5;
    CAugmentationDef blank; blank.Name = 0;
    CThingBaseDef d1; d1.Sub = &fire;
    CThingBaseDef d2; d2.Sub = &blank;
    gDefs[1] = &d1; gDefs[2] = &d2;
    gEntries[5].Text = L"Fire Augmentation";
    int ok = 1;
    CWideString a = GetAugmentationName(1);
    if (!WideEqual(a.Text, L"Fire Augmentation") || gLastLookup != 5) ok = 0;
    if (d1.RefCount != 1 || fire.RefCount != 1 || gEntries[5].RefCount != 1) ok = 0;   // all references returned
    CWideString b = GetAugmentationName(2);
    if (!WideEqual(b.Text, L"Missing augmentation name!") || gLastLookup != 5) ok = 0;
    if (d2.RefCount != 1 || blank.RefCount != 1) ok = 0;
    if (!ok) { std::printf("00765f90_TEST FAIL\n"); return 1; }
    std::printf("00765f90_TEST PASS\n");
    return 0;
}