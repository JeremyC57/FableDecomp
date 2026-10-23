// CTCInventory::UpdateContextSensitiveItems() -- every 8th frame (or when flagged) drop exhausted candidates, count the carried items' opinion expressions, rank the context-sensitive candidates and fade each new winner into its button slot, clearing unclaimed slots
// Grade: DIFFER(729v735) -- logic modelled (frame-8 gate, exhausted-candidate erase, item scan for context-sensitive subdefs and opinion-expression counts, global candidates, sort, fade-in per button slot, clear unclaimed slots); residue is the escape-analysis lane gap: retail treats the candidate vector as non-escaping (its end cached in edi across the sort, counted pointer loop) yet reloads the item vector's start every pass and orders items below candidates in the frame -- any one stand-in set reproduces only half (items-escaping flips the frame order, items-non-escaping strength-reduces loop 1); also ParentThing loaded before the IsValidOverride pushes and the fade 'this' scheduled after the argument slots; 14 grades
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);

struct CThing;
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};
struct CFadeInFadeOutBase {                          // 0x1c bytes
    bool       Active;                               // +0x00
    float      FadeInTime;                           // +0x04
    float      FadeOutTime;                          // +0x08
    bool       Closing;                              // +0x0c
    bool       Opening;                              // +0x0d
    float      OpenTimer;                            // +0x10
    float      CloseTimer;                           // +0x14
    CRGBColour ToColour;                             // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour toColour);   // 0x00434840
};

struct CTCInventory;
struct CContextSensitiveItemDef {
    virtual void d00();
    virtual void DeleteThis();                       // slot 1 (+4)
    long          RefCount;                          // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long          Kind;                              // +0x28 (6 = opinion expression)
    unsigned char _pad2c[0x30 - 0x2c];
    long          ButtonSlot;                        // +0x30
    long          Expression;                        // +0x34
    bool IsValidOverride(const CThing& thing, long type, const CTCInventory* inventory) const;   // 0x00850A80
};
struct CCSDefPointer {                               // CDefPointer<const CContextSensitiveItemDef>
    CContextSensitiveItemDef* m_p;
    CCSDefPointer() : m_p(0) {}
    __forceinline ~CCSDefPointer() { if (m_p && --m_p->RefCount == 0) m_p->DeleteThis(); }
    CContextSensitiveItemDef* operator->() const { return m_p; }
};
struct CThingDef {
    virtual void d00();
    virtual void DeleteThis();                       // slot 1 (+4)
    long RefCount;                                   // +0x04
    bool GetSubDef(CCSDefPointer* out) const;        // 0x005933C8 GetSubDef<CContextSensitiveItemDef>
};
struct CThingDefPointer {                            // CDefPointer<const CThingDef>
    CThingDef* m_p;
    CThingDefPointer() : m_p(0) {}
    __forceinline ~CThingDefPointer() { if (m_p && --m_p->RefCount == 0) m_p->DeleteThis(); }
    CThingDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    bool GetDef(long index, CThingDefPointer* out) const;   // 0x0042AF3C GetDef<...>
};
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CCSDefPointer* out) const { out->m_p = 0; return false; }
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CThingDefPointer* out) const { out->m_p = 0; return index != 0; }
#pragma auto_inline(on)

struct SItemCount { long first; long second; };      // pair<long, long>
struct CItemCountVector {                            // STLport vector<pair<long, long> >
    SItemCount* _M_start;
    SItemCount* _M_finish;
    SItemCount* _M_end_of_storage;
    CItemCountVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemCountVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    SItemCount& operator[](unsigned long n) { return _M_start[n]; }
};
struct SCandidate {                                  // pair<long, CDefPointer<const CContextSensitiveItemDef> >
    long          first;
    CCSDefPointer second;
    SCandidate(const long& type, const CCSDefPointer& def);   // 0x00592962
    ~SCandidate();                                   // 0x005930F3
};
struct CCandidateVector {
    SCandidate* _M_start;
    SCandidate* _M_finish;
    SCandidate* _M_end_of_storage;
    CCandidateVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCandidateVector();                             // 0x00594BE8
    void push_back(const SCandidate& value);         // 0x00594E15
    unsigned long size() const { return _M_finish - _M_start; }
    SCandidate& operator[](unsigned long n) { return _M_start[n]; }
};
struct CCandidateOrder { CCandidateOrder() {} bool operator()(const SCandidate& a, const SCandidate& b) const; };
void __fastcall SortCandidates(SCandidate* first, SCandidate* last, CCandidateOrder order);   // 0x00594ED4 (std::sort)

struct SItemTypeEntry { long Type; long _pad[2]; };  // 12 bytes
#pragma auto_inline(off)
__declspec(noinline) SCandidate::SCandidate(const long& type, const CCSDefPointer& def) { first = type; second.m_p = def.m_p; }
__declspec(noinline) SCandidate::~SCandidate() {}
__declspec(noinline) CCandidateVector::~CCandidateVector() {}
__declspec(noinline) void CCandidateVector::push_back(const SCandidate& value) { _M_finish = _M_end_of_storage; }
#pragma auto_inline(on)

struct CItemTypeVector {
    SItemTypeEntry* _M_start;
    SItemTypeEntry* _M_finish;
    SItemTypeEntry* _M_end_of_storage;
    SItemTypeEntry* erase(SItemTypeEntry* position);   // 0x0059310D
    unsigned long size() const { return _M_finish - _M_start; }
    SItemTypeEntry& operator[](unsigned long n) { return _M_start[n]; }
};
extern CItemTypeVector g_ContextSensitiveItemTypes;  // 0x013B8B40

struct CWorld {
    static long __fastcall GetFrameCount();          // 0x0049D870
};

template<class T> struct CVector {                   // retail STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCInventory {
    void*                          __vftable;        // +0x000
    CThing*                        ParentThing;      // +0x004
    unsigned char                  _pad_0x008[0x154 - 0x008];
    CVector<long>                  ContextSensitiveItems;        // +0x154 (PDB +0x178)
    CVector<CFadeInFadeOutBase>    ContextSensitiveFadeTimes;    // +0x160 (PDB +0x188)
    unsigned char                  _pad_0x16c[0x1d4 - 0x16c];
    bool                           ContextSensitiveUpdate;       // +0x1d4 (PDB +0x20c)
    unsigned char                  _pad_0x1d5[3];
    CVector<long>                  NumberOfCSOpinionExpressions; // +0x1d8 (PDB +0x210)
    const CWorld& PeekWorld() const;                 // 0x00686A80 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long type) const;   // 0x005BDF08
    void GetAllItems(CItemCountVector& items) const; // 0x005BF482
    long GetContextSensitiveButtonSlot(long button); // 0x005819F2
    static long __fastcall GetContextSensitivePreferredItemButtonIndex(long type);   // 0x00581BFE
    void UpdateContextSensitiveItems();
};


void CTCInventory::UpdateContextSensitiveItems()
{
    if (!ContextSensitiveUpdate && PeekWorld().GetFrameCount() % 8 != 0)
        return;
    for (SItemTypeEntry* it = g_ContextSensitiveItemTypes._M_start; it != g_ContextSensitiveItemTypes._M_finish; ) {
        if (GetNumberOfItemsOfTypeInInventory(it->Type) == 0)
            it = g_ContextSensitiveItemTypes.erase(it);
        else
            ++it;
    }
    CItemCountVector items;
    GetAllItems(items);
    CCandidateVector candidates;
    for (long expression = 0; expression < 6; ++expression)
        NumberOfCSOpinionExpressions[expression] = 0;
    {
        long count = items.size();
        for (long i = 0; i < count; ++i) {
            CThingDefPointer def;
            GetDefinitionManager()->GetDef(items[i].first, &def);
            CCSDefPointer csDef;
            if (def->GetSubDef(&csDef)) {
                candidates.push_back(SCandidate(items[i].first, csDef));
                if (csDef->Kind == 6 && csDef->Expression >= 0 && csDef->Expression < 6)
                    ++NumberOfCSOpinionExpressions[csDef->Expression];
            }
        }
    }
    for (unsigned long k = 0; k < g_ContextSensitiveItemTypes.size(); ++k)
        candidates.push_back(SCandidate(g_ContextSensitiveItemTypes[k].Type, CCSDefPointer()));
    SortCandidates(candidates._M_start, candidates._M_finish, CCandidateOrder());
    bool used[4];
    used[0] = false;
    used[1] = false;
    used[2] = false;
    used[3] = false;
    for (unsigned long c = 0; c < candidates.size(); ++c) {
        long type = candidates[c].first;
        long slot;
        long button = GetContextSensitivePreferredItemButtonIndex(type);
        if (button != -1)
            slot = GetContextSensitiveButtonSlot(button);
        else
            slot = candidates[c].second->ButtonSlot;
        if (slot >= 0) {
            if (candidates[c].second.m_p == 0 || candidates[c].second->IsValidOverride(*ParentThing, type, this)) {
                if (!ContextSensitiveFadeTimes[slot].Opening && ContextSensitiveItems[slot] != type && !used[slot]) {
                    ContextSensitiveItems[slot] = type;
                    ContextSensitiveFadeTimes[slot].Start(1.0f, 0.0f, CRGBColour(0, 0, 0, 0xff));
                }
                used[slot] = true;
            }
        }
    }
    for (unsigned long j = 0; j < ContextSensitiveItems.size(); ++j)
        if (!used[j])
            ContextSensitiveItems[j] = 0;
    ContextSensitiveUpdate = false;
}
