// CTCInventoryBase::GetCategoryNameWithIndex(long index, CWideString& name) -- copies the display name of category `index` into `name`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDB68, length 44 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CCategory::GetCategoryName() const @ 0x005BDAC5 (returns CWideString by value through a hidden pointer),
// CWideString::operator= @ 0x0099B7D0, CWideString::~CWideString @ 0x0099B510.
// Retail CTCInventoryBase: no engine header; Categories (0x2C-byte CCategory elements) at +0x20.
// Shape: the by-value temporary is homed in the dead `index` argument slot (`lea ecx,[ebp+8]`) once the index has
// been consumed; Categories.first is loaded into eax BEFORE the hidden-pointer push, which needs it in a named local
// (`Categories[index]` through the accessor re-derives it as `add ecx,[eax+20h]` after the push).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                    // 0x0099B510
};
struct CCategory_Retail {
    unsigned char _body[0x2C];
    CWideString GetCategoryName() const;               // 0x005BDAC5
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                        // +0x20
    void GetCategoryNameWithIndex(long index, CWideString& name);
};
void CTCInventoryBase_Retail::GetCategoryNameWithIndex(long index, CWideString& name)
{
    CCategory_Retail* first = Categories.first;
    name = first[index].GetCategoryName();
}
