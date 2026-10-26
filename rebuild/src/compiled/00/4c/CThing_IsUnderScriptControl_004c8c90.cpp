// CThing::IsUnderScriptControl() -- a thing with a script-control component (interface 0x1f) is under script control when the component's control flag is set or its script task is no longer running
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model (landed 0x00584157 idiom) instead of the GetInterface out-pointer: the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8C90, 81 bytes. Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface lookup); the component method @ 0x007122C0 (manifest "CScriptTask_IsRunning").
// Shape: the landed GetInterface out-parameter template; interface 0x1f is bit 31 of bitset word 0
// (`mov eax,[ecx+20h] ; test eax,eax ; jns`). The missing-component exit is a plain `return false`
// (`xor al,al`); the rest is one `||` expression (full-register 0/1). The component's flag is the byte at +0x18.
class CTCBase;

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
    SEntry* LowerBound(const long* type) const;            // 0x0040F020
};

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        long type = wanted;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    bool IsUnderScriptControl();
};

bool CThing::IsUnderScriptControl()
{
    if (HasTC(0x1f)) {
        CTCScriptControl* control = GetTC<CTCScriptControl>(0x1f);
        return control->IsUnderControl();
    }
    return false;
}