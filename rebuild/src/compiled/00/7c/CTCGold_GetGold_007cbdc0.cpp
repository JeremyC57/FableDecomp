// CTCGold::GetGold() const -- the gold amount: the component's own value when set, else the amount in the owner thing def's CGoldDef sub-def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CBDC0, length 166 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?GetGold@CTCGold@@QBEJXZ`.
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const
//          @ 0x0042AF3C (the thing's def; T taken as CThingBaseDef, the instantiation shared by 119 callers including CThing),
//          CDefClassBase::GetSubDef<CGoldDef>(CDefPointer<const T>*) const @ 0x006AE320, the defs' virtual slot 1.
// Shape: the two out slots are block-scoped and share the one push-ecx word, each zeroed after its address is pushed and
//        read once into a local released by hand (the def's own Release()); on the hit path the gold def's release has no null test (it was just
//        dereferenced) and the amount is returned straight, else the member is re-read.
// Layout: Ego_r.pdb CTCGold: Gold +0x0C; retail CThing DefGlobalIndex (unsigned short) +0x8C; retail CGoldDef amount +0x28.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x006AE320 for CGoldDef
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CGoldDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long Gold;                                      // +0x28
};
class CThingBaseDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { };

class CThing {
public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;                  // +0x8C
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCGold : public CTCBase {
public:
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    long Gold;                                      // +0x0C
    long GetGold() const;
};

long CTCGold::GetGold() const
{
    if (Gold == 0) {
        const CThingBaseDef* thingDef;
        bool found;
        {
            CThing* parent = ParentThing;
            CDefPointer<const CThingBaseDef> out;
            found = GetDefinitionManager().GetDef(parent->DefGlobalIndex, &out);
            thingDef = out.Object;
        }
        if (found) {
            const CGoldDef* goldDef;
            {
                CDefPointer<const CGoldDef> out;
                found = thingDef->GetSubDef(&out);
                goldDef = out.Object;
            }
            if (found) {
                long gold = goldDef->Gold;
                goldDef->Release();
                if (thingDef)
                    thingDef->Release();
                return gold;
            }
            if (goldDef)
                goldDef->Release();
        }
        if (thingDef)
            thingDef->Release();
    }
    return Gold;
}