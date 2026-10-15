// Grade: DIFFER(83v81) -- the component pointer lands in eax and is copied to ecx for the call (retail loads it straight into ecx: `mov ecx,[eax+4] ; mov al,[ecx+18h]`); block order matches
// CThing::IsUnderScriptControl() -- a thing with a script-control component (interface 0x1f) is under script control when the component's control flag is set or its script task is no longer running
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8C90, 81 bytes. Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface lookup); the component method @ 0x007122C0 (manifest "CScriptTask_IsRunning").
// Shape: the landed GetInterface out-parameter template; interface 0x1f is bit 31 of bitset word 0
// (`mov eax,[ecx+20h] ; test eax,eax ; jns`). The missing-component exit is a plain `return false`
// (`xor al,al`); the rest is one `||` expression (full-register 0/1). The component's flag is the byte at +0x18.
struct CTCBase;

struct CTCScriptControl {
    unsigned char _pad00[0x18];
    bool          m_underControl;                    // +0x18
    bool IsTaskRunning();                            // 0x007122C0
    bool IsUnderControl() { return m_underControl || !IsTaskRunning(); }
};

struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool IsUnderScriptControl();
};

bool CThing::IsUnderScriptControl()
{
    CTCScriptControl* control;
    if (GetInterface(&control, 0x1f))
        return control->IsUnderControl();
    return false;
}
