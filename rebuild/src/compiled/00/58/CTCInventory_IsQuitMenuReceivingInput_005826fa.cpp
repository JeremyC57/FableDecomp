#pragma optimize("s", on)
// CTCInventory::IsQuitMenuReceivingInput() -- true when any of the inventory's UI components owns a "PC_MENU_ENTRY_QUIT" entry whose input receiver accepts input
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005826FA, length 90 bytes; size-optimised TU ("s").
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (literal 0x0124FF80 "PC_MENU_ENTRY_QUIT",
//          length -1); per component virtual slot 3 (`call [ebp+0Ch]`, the name by value) returning an entry,
//          then on that entry's second base (+4) virtual slot 2 (`call [eax+8]`, argument 2) returning bool.
// Ego_r PDB: ?IsQuitMenuReceivingInput@CTCInventory@@QAE_NXZ.
// Retail layout: the component pointer vector is the 12-byte {first,last,end} vector at +0x0c (PDB
// CTCInventory.h has 16-byte vectors), so `this` is modelled locally; the entry/receiver classes are named
// by use.
// Shape: CCharString has a user copy constructor, so the by-value argument is built in its stack slot
// (`push ecx ; mov ecx,esp`) and destroyed by the callee; the component's vptr is loaded before the string is
// built; the receiver is reached by the implicit derived-to-base adjustment (`lea ecx,[eax+4]`).
struct CCharString {
    void* m_pData;                                   // +0x00
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

struct CInputReceiver {
    virtual void v00(); virtual void v01();
    virtual bool IsReceivingInput(long inputLayer);  // slot 2
};

struct CMenuEntryOwner {
    virtual void v00();
};

struct CMenuEntry : CMenuEntryOwner, CInputReceiver {   // the receiver base sits at +4
};

struct CUIComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CMenuEntry* FindEntry(CCharString name); // slot 3
};

struct CComponentVector {
    CUIComponent** first;                            // +0x00
    CUIComponent** last;                             // +0x04
    CUIComponent** end_of_storage;                   // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    CUIComponent*& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char    _pad_0x00[0x0c];
    CComponentVector Components;                     // +0x0c
    bool IsQuitMenuReceivingInput();
};

bool CTCInventory::IsQuitMenuReceivingInput()
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CMenuEntry* entry = Components[i]->FindEntry(CCharString("PC_MENU_ENTRY_QUIT"));
        if (entry && entry->IsReceivingInput(2))
            return true;
    }
    return false;
}