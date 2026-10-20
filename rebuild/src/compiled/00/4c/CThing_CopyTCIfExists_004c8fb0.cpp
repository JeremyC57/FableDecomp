// CThing::CopyTCIfExists(const CTCBase&) -- when this thing has a component of the source's interface type, copy the source into it
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8FB0, 114 bytes (+14 int3). __fastcall, `ret 4`.
// Callees: source CTCBase virtual slot 21 (+0x54, the interface-type getter); STLport bitset<274>::test range check with
//          exceptions compiled out: puts("bitset") 0x00BFEAF2 + abort() 0x00BFEAEC; CVectorMap<ETCInterfaceType,CTCBase*>::
//          LowerBound 0x0040F020; the found component's virtual slot 12 (+0x30) with the source (descriptive name CopyFrom).
// Shape: the type is a runtime value, so the inline bitset test keeps STLport's `pos >= 274` guard and the shift/word
//        split (`and ecx,1Fh ; shl edx,cl ; shr eax,5 ; test [edi+eax*4+20h],edx`); the map lookup key lives in the dead
//        argument slot and the `type < it->type` test is `cmp esi,[eax] ; jge`.
// Layout: retail CThing bitset +0x20 (bitset<274>, 9 words), interface map +0x44.
extern "C" int __cdecl puts(const char* text);   // 0x00BFEAF2
extern "C" void __cdecl abort();                 // 0x00BFEAEC
class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyFrom(const CTCBase& source);            // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual long GetInterfaceType() const;                    // slot 21 (+0x54)
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CComponentBits {                         // STLport bitset<274>
    unsigned long _M_w[9];
    bool test(unsigned long pos) const
    {
        if (pos >= 274) {
            puts("bitset");
            abort();
        }
        return (_M_w[pos >> 5] & (1UL << (pos & 31))) != 0;
    }
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    CComponentBits UsingComponents;             // +0x20
    CInterfaceMap Components;                   // +0x44
    void CopyTCIfExists(const CTCBase& source);
};
__forceinline CTCBase* GetTC(CThing* thing, long type)
{
    long key = type;
    CInterfaceMap* map = &thing->Components;
    CInterfaceMap::SEntry* it = map->LowerBound(&key);
    return ((it == map->last || type < it->type) ? map->last : it)->component;
}
void CThing::CopyTCIfExists(const CTCBase& source)
{
    long type = source.GetInterfaceType();
    if (UsingComponents.test(type))
        GetTC(this, type)->CopyFrom(source);
}