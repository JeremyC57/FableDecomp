// Grade: DIFFER(1136v1016) -- retail pushes ebp with ebx up front and sends the early false returns to one shared tail; its sight cases share the two squares sums only as CSE'd squares (VC7.1 here CSEs the whole sum, flipping the compare to test ah,0x41/jp) and computes cos(FOV) before the heading dot product; the sight-case tails are cross-jumped in retail
// CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& sensed, const CScriptThing& senser, ESenseType type) const -- whether senser can smell/hear/see the sensed thing: the cached flags when it is the current perception target, else a radius test (plus a field-of-view cone for sight)
// Leaf: CanThingBe_Sensed_ByOtherThing
extern "C" int __cdecl puts(const char* text);
extern "C" __declspec(noreturn) void __cdecl abort();
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[9];                    // +0x20: which component kinds the thing holds (bitset<274>)
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x3c];
    unsigned short m_id;                           // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void SetInLimbo(bool limbo);                   // 0x004C8CF0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
    __forceinline bool TestComponent(unsigned long position) const    // STLport bitset<274>::test with its range check
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_tcMask[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};

extern unsigned long g_InvSqrtMantissaTable[128];  // 0x013CE558
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
static const float TWO_PI = 6.28318548f;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLengthSquared() const { return Z * Z + Y * Y + X * X; }
    __forceinline float operator*(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CThingPos {
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CHeadObject {
    virtual void h00(); virtual void h01(); virtual void h02(); virtual void h03(); virtual void h04(); virtual void h05(); virtual void h06(); virtual void h07();
    virtual void h08(); virtual void h09(); virtual void h10(); virtual void h11(); virtual void h12(); virtual void h13(); virtual void h14(); virtual void h15();
    virtual void h16(); virtual void h17(); virtual void h18(); virtual void h19(); virtual void h20(); virtual void h21(); virtual void h22(); virtual void h23();
    virtual void h24(); virtual void h25(); virtual void h26(); virtual void h27(); virtual void h28(); virtual void h29(); virtual void h30(); virtual void h31();
    virtual void h32(); virtual void h33(); virtual void h34(); virtual void h35(); virtual void h36(); virtual void h37(); virtual void h38(); virtual void h39();
    virtual void h40(); virtual void h41(); virtual void h42(); virtual void h43(); virtual void h44(); virtual void h45(); virtual void h46(); virtual void h47();
    virtual void h48(); virtual void h49(); virtual void h50(); virtual void h51(); virtual void h52(); virtual void h53(); virtual void h54(); virtual void h55();
    virtual void h56(); virtual void h57(); virtual void h58(); virtual void h59(); virtual void h60(); virtual void h61(); virtual void h62(); virtual void h63();
    virtual void h64(); virtual void h65(); virtual void h66(); virtual void h67(); virtual void h68(); virtual void h69();
    virtual C3DVector GetHeadingDirection() const; // slot 70 (+0x118)
};
struct CThingSense {
    unsigned char _pad[0x24];
    unsigned long Flags24;                         // +0x24
    unsigned char _pad28[0x38];
    CHeadObject*  Head;                            // +0x60
};
struct CTCPerceiveThing {
    unsigned char _pad[0x2d];
    bool CanSmell;                                 // +0x2d
    bool CanHear;                                  // +0x2e
    unsigned char _pad2f[2];
    bool CanSee;                                   // +0x31
    bool CanSeeExtended;                           // +0x32
    CThing_Retail* GetCurrentTarget();             // 0x007446A0
    bool IsAwareOfThing();                         // 0x00643B3B
    float GetSmellRadius();                        // 0x00744440
    float GetHearingRadius();                      // 0x00744450
    float GetSightRadius();                        // 0x00744950
    float GetExtendedSightRadius();                // 0x007448F0
    float GetFieldOfView();                        // 0x007445D0
};
enum ESenseType { SENSE_NONE };
struct CGameScriptInterface {
    virtual void v000();
    virtual bool CanThingBe_Sensed_ByOtherThing(const CScriptThing& sensed, const CScriptThing& senser, ESenseType type) const;
};
#define POS(t) ((const CThingPos*)(t))->GetPosition()

bool CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& sensed, const CScriptThing& senser, ESenseType type) const
{
    if (!sensed.IsValid() || !senser.IsValid())
        return false;
    CThing_Retail* perceiver = senser.GetThing();
    CThing_Retail* target = sensed.GetThing();
    if (!(((CThingSense*)perceiver)->Flags24 & 0x100000))
        return false;
    CTCPerceiveThing* perceive = (CTCPerceiveThing*)GetTC(perceiver, 0x34);
    CThing_Retail* current = perceive->GetCurrentTarget();
    if (current && current == target) {
        switch (type) {
        case 1: return perceive->CanSmell;
        case 2: return perceive->CanHear;
        case 3: return perceive->CanSee;
        case 4: return perceive->CanSeeExtended;
        case 5: return perceive->IsAwareOfThing();
        }
        return false;
    }
    switch (type) {
    case 5: {
        float radius = Max(perceive->GetSmellRadius(), perceive->GetHearingRadius());
        C3DVector d = POS(target) - POS(perceiver);
        if (d.GetLengthSquared() <= radius * radius)
            return true;
    }
    case 2: {
        float radius = perceive->GetSightRadius();
        C3DVector d = POS(target) - POS(perceiver);
        if (d.X * d.X + d.Y * d.Y + d.Z * d.Z <= radius * radius) {
            d.Normalise();
            return d * ((CThingSense*)perceiver)->Head->GetHeadingDirection() > cos(perceive->GetFieldOfView() * (double)TWO_PI);
        }
        return false;
    }
    case 1: {
        float radius = perceive->GetExtendedSightRadius();
        C3DVector d = POS(target) - POS(perceiver);
        if (d.X * d.X + d.Y * d.Y + d.Z * d.Z <= radius * radius) {
            d.Normalise();
            return d * ((CThingSense*)perceiver)->Head->GetHeadingDirection() > cos(perceive->GetFieldOfView() * (double)TWO_PI);
        }
        return false;
    }
    case 3: {
        float radius = perceive->GetHearingRadius();
        C3DVector d = POS(target) - POS(perceiver);
        return d.GetLengthSquared() <= radius * radius;
    }
    case 4: {
        float radius = perceive->GetSmellRadius();
        C3DVector d = POS(target) - POS(perceiver);
        return d.GetLengthSquared() <= radius * radius;
    }
    }
    return false;
}
