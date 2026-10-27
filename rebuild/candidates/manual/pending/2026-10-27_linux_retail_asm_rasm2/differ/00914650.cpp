// CAIStateGroup_BirdStartled::IsRunnable() -- startled when the creature stands in the bird region and a bird-scaring thing is found in the rough 2.5-unit box around it, within 5 units
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914650, length 340 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 7 CTCCreatureNavigation), the navigation region query @ 0x006CB6A0
//          (manifest "CIsHitEventBlocked::operator()" is propagated), CThing::GetPosition const @ 0x006A4D00, __ftol2 @
//          0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThingInRoughArea
//          <CThingFilter_ScaresBirds, CThingCompare_Nearest> @ 0x009147E0.
// Globals: the bird region id @ 0x0129CBA4 (3), 25.0f @ 0x0123003C.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4), modelled locally.
#pragma optimize("y", off)
// Retail-asm: 0x009146BB..0x009146D5 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-4]` (the 2.5f argument stored into the by-value copy) -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
void* FindRef_CInterfaceMap_LowerBound_0();   // stand-in callee of the stand-in body below
// Same-TU stand-in of the CVectorMap LowerBound instantiation (the real body is 0x0040F020): it only reads the key through
// the pointer, so VC7.1 knows the caller's address-taken type word does not escape and reuses its frame slot.
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    void* p = FindRef_CInterfaceMap_LowerBound_0();
    if (!p)
        return 0;
    *(void**)type = p;
    return (SEntry*)p;
}
#pragma auto_inline(on)
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x006A4D00
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
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
    C2DBoxI box;
    CThingCompare_Nearest compare;
    CThingFilter_ScaresBirds filter;
    CThingCreatureBase* best;
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
    box.X0 = x - range; box.Y0 = y - range; box.X1 = x + range; box.Y1 = y + range;
    compare.Position = PBrain->PCreature->GetPosition();
    filter.Creature = PBrain->PCreature;
    if (!PeekThingSearchTools()->GetPBestThingInRoughArea(&box, &filter, &compare, &best))
        return false;
    CThingCreatureBase* me = PBrain->PCreature;
    const C3DVector& a = me->GetPosition();
    const C3DVector& b = me->GetPosition();
    float dx = b.X - a.X, dy = b.Y - a.Y;
    if (dx * dx + dy * dy < 25.0f)
        return true;
    return false;
}
