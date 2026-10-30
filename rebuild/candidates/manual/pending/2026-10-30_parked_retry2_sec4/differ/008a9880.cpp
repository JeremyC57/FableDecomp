// CGameScriptInterface::ScreenFilterFadeTo(float time, float a, float b, float c, float d, const CRGBFloatColour& colour, const std::vector<CScreenFilterSThingByPass>& bypass) const -- convert the script-thing bypass list into intelligent-pointer bypass records and start a screen-filter fade (priority 1) on the environment spell-effect interpolator; returns its handle
// Leaf: ?ScreenFilterFadeTo@CGameScriptInterface@@UBEPAXMMMMMABVCRGBFloatColour@@ABV?$vector@UCScreenFilterSThingByPass@@V?$allocator@UCScreenFilterSThingByPass@@@std@@@std@@@Z
// Grade: DIFFER(ours v retail) -- 347v363, nd 45->31 (retry wave 2 sec4): the record is a named local (a 4-aligned frame
//   slot at esp+18h as retail; the push_back temporary landed 1-aligned at esp+1Bh). Left: the zero constant lives in
//   edi (retail ebx, re-zeroing edi for the element offset) and retail aligns the loop head (jmp over lea esp,[esp]);
//   tried index-only, while, named thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9880, length 361 bytes (`ret 1Ch`).
// Callees: the script thing's slot 11 (+0x2C GetThing), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 /
//   0x00A01B50 / 0x00A01B90 / 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), the record vector's
//   _M_insert_overflow @ 0x006A3950, CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo(float, ScreenEffectPriority,
//   float, float, float, float, const CRGBFloatColour&, const vector<...>&) @ 0x006B85D0, free @ 0x00BFEA14.
// Layout: CScreenFilterSThingByPass = CScriptThing (12 bytes) + bool (+0xC), 16-byte stride; the engine-side record is
//   {CIntelligentPointer<CThing>, bool} with a 9-byte stride (packed, the `add esi,9` walks); the interpolator is reached
//   through World+0x18 -> +0x1964.
// Shape: `for (long i = 0; i < (long)bypass.size(); ++i)` (signed `test ecx,0FFFFFFF0h ; jle` entry test, the start
//   pointer re-read each pass); push_back inlined (null-tested placement copy, finish += 9, else the overflow with the
//   __false_type tag in the dead bypass slot); the record vector's destructor walks the elements then frees.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +4
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
#pragma pack(push, 1)
struct SThingByPassRecord {                             // 9 bytes: retail walks these with `add esi,9`
    CIntelligentPointer Thing;                          // +0
    bool                Flag;                           // +8
    SThingByPassRecord(CThing* thing, bool flag) : Thing(thing), Flag(flag) {}
};
#pragma pack(pop)
struct __false_type { __false_type() {} };
inline void _Destroy(SThingByPassRecord* first, SThingByPassRecord* last)
{
    for (; first != last; ++first)
        first->~SThingByPassRecord();
}
struct CByPassRecordVector {                            // STLport vector<SThingByPassRecord> (malloc allocator)
    SThingByPassRecord* _M_start;
    SThingByPassRecord* _M_finish;
    SThingByPassRecord* _M_end_of_storage;
    CByPassRecordVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CByPassRecordVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    void _M_insert_overflow(SThingByPassRecord* position, const SThingByPassRecord& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x006A3950
    void push_back(const SThingByPassRecord& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SThingByPassRecord(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    void* PImp_Data;
    void* PImp_Info;
};
struct CScreenFilterSThingByPass {                      // 16 bytes
    CScriptThing Thing;                                 // +0x00
    bool         Flag;                                  // +0x0C
};
struct CScreenFilterByPassVector {
    CScreenFilterSThingByPass* _M_start;
    CScreenFilterSThingByPass* _M_finish;
    CScreenFilterSThingByPass* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CScreenFilterSThingByPass& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CRGBFloatColour;
enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_SCRIPT = 1 };
struct CEnvironmentSpellEffectInterpolator {
    void* ScreenFilterFadeTo(float time, ScreenEffectPriority priority, float a, float b, float c, float d,
                             const CRGBFloatColour& colour, const CByPassRecordVector& bypass);   // 0x006B85D0
};
struct CDisplayEffects_Retail {
    unsigned char                        _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffects;  // +0x1964
};
struct CWorld_Retail {
    unsigned char           _pad00[0x18];
    CDisplayEffects_Retail* Effects;                    // +0x18
};
struct CGameScriptInterface {
    virtual void* ScreenFilterFadeTo(float time, float a, float b, float c, float d, const CRGBFloatColour& colour,
                                     const CScreenFilterByPassVector& bypass) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

void* CGameScriptInterface::ScreenFilterFadeTo(float time, float a, float b, float c, float d, const CRGBFloatColour& colour,
                                               const CScreenFilterByPassVector& bypass) const
{
    CByPassRecordVector records;
    for (long i = 0; i < (long)bypass.size(); ++i) {
        const CScreenFilterSThingByPass& entry = bypass[i];
        SThingByPassRecord record(entry.Thing.GetThing(), entry.Flag);
        records.push_back(record);
    }
    CEnvironmentSpellEffectInterpolator* effects = World->Effects->SpellEffects;
    return effects->ScreenFilterFadeTo(time, SCREEN_EFFECT_PRIORITY_SCRIPT, a, b, c, d, colour, records);
}
