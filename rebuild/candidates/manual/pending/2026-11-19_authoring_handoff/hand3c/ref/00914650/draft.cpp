// CAIStateGroup_BirdStartled::IsRunnable() -- startled when the creature stands in the bird region and a bird-scaring thing is found in the rough 2.5-unit box around it, within 5 units
// Grade: DIFFER(341v340) -- landed CeilToLong retail_asm helper, the interface lookup as a CInterfaceMap member (&map held in edi) and the retail return shape; residue: frame size 0x38 v 0x30 and the order of the four box stores
// Retail-asm: 0x009146BE fadd dword ptr [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild dword ptr [ebp-4] -- the landed CeilToLong helper on the constant 2.5f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914650, length 340 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 7 CTCCreatureNavigation), the navigation region query @ 0x006CB6A0
//          (manifest "CIsHitEventBlocked::operator()" is propagated), CThing::GetPosition const @ 0x006A4D00, __ftol2 @
//          0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThingInRoughArea
//          <CThingFilter_ScaresBirds, CThingCompare_Nearest> @ 0x009147E0.
// Globals: the bird region id @ 0x0129CBA4 (3), 25.0f @ 0x0123003C.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4), modelled locally.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    __forceinline CTCBase* Find(long interfaceType)
    {
        long type = interfaceType;
        SEntry* it = LowerBound(&type);
        return ((it == last || interfaceType < it->type) ? last : it)->component;
    }
};
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x006A4D00
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        return (T*)m_interfaces.Find(interfaceType);
    }
};
struct CTCCreatureNavigation {
    long GetRegion();                           // 0x006CB6A0
};
struct CThingFilter_ScaresBirds { CThingCreatureBase* Creature; };
struct CThingCompare_Nearest { C3DVector Position; };
struct CThingSearchTools {
    bool GetPBestThingInRoughArea(const C2DBoxI* box, const CThingFilter_ScaresBirds* filter,
        const CThingCompare_Nearest* compare, CThingCreatureBase** best);   // 0x009147E0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
extern const long g_BirdRegion;                 // 0x0129CBA4
// the engine's inline-assembly ceil helper (the landed CTCShop::GetMaxStock 0x00772720 / CThingPhysical::ModifyHealth 0x006A68F0 CeilToLong, verbatim)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_BirdStartled : CAIStateGroupBase {
    bool IsRunnable();
};

bool CAIStateGroup_BirdStartled::IsRunnable()
{
    CTCCreatureNavigation* navigation;
    CThingCreatureBase* creature = PBrain->PCreature;
    if (creature->m_presentBits[0] & 0x80)
        navigation = creature->GetTC<CTCCreatureNavigation>(7);
    if (navigation->GetRegion() != g_BirdRegion)
        return false;
    long range = CeilToLong(2.5f);
    const C3DVector& pos = PBrain->PCreature->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box = { x - range, y - range, x + range, y + range };
    CThingCompare_Nearest compare = { PBrain->PCreature->GetPosition() };
    CThingFilter_ScaresBirds filter = { PBrain->PCreature };
    CThingCreatureBase* best;
    if (PeekThingSearchTools()->GetPBestThingInRoughArea(&box, &filter, &compare, &best)) {
        CThingCreatureBase* me = PBrain->PCreature;
        const C3DVector& a = me->GetPosition();
        const C3DVector& b = me->GetPosition();
        float dx = b.X - a.X, dy = b.Y - a.Y;
        if (dx * dx + dy * dy < 25.0f)
            return true;
    }
    return false;
}
