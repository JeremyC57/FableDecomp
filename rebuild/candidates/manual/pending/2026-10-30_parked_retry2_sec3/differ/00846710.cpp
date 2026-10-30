// Grade: DIFFER(1185v1192) -- 82 difflines (was 1170/144): sound literals straight to the const CCharString& parameter; the sprite text in an inner block (its dtor before the shared EmoteIconAdded store, which retail tail-merges); the GetString result bound to a const CCharString& (ToWideString through lea, not eax); the sprite id a named local read before the other SetEmoteIcon pushes. Residue: str/text slot order (+0x10/+0x14 swapped), the ToWideString temp destroyed before SetEmoteText in retail, CTCSound/deed-block register choice. Tried: inline WideFromString wrapper (139-143)
// CCreatureAction_PerformExpression::Initialise() -- ensure CTCExpression (sharing the expression def) and CTCEmoteIcon, show the sprite text or icon, play the expression sound, trigger the expression quest hook and post the opinion deed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846710, length 1192 bytes.
// Tried: scoped CCharString names for AddTC (fixed), unsigned HasTC mask test (fixed), CDefPointer operator-> (fixed), def local for the share (fixed).
// Leaf: Initialise
struct CTCBase;
struct C3DVector;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    struct CWideString ToWideString() const;            // 0x0099E480
};
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& o);
    ~CWideString();                                     // 0x0099B510
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    void AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
};
struct CThingCreatureBase : CThing {
    bool IsChild() const;                               // 0x00661F70
    const C3DVector* GetPosition() const;               // 0x006A4D00
};
struct CIntelligentPointer {
    void* m_a; void* m_b;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CDefString {
    long m_index;
    CDefString(long index);
    bool operator!=(const char* text) const;            // 0x005FA740
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CQuestManager {
    void ActivateQuestFromExpression(const CCharString& questName);   // 0x004AF3F0
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
extern bool g_UseSpriteOverrideText;                    // 0x013B86DC
struct CRefDef {
    virtual void v00(); virtual void Destroy();
    long RefCount;                                      // +0x04
};
struct CExpressionDef : CRefDef {
    virtual void d02(); virtual void d03(); virtual void d04(); virtual void d05();
    virtual void d06(); virtual void d07(); virtual void d08(); virtual void d09();
    virtual CDefString GetName() const;                 // slot 10 (+0x28)
    unsigned char _pad8[0x64 - 8];
    CDefString SpriteOverrideText;                      // +0x64
    unsigned char _pad68[0x80 - 0x68];
    long OpinionDeedType;                               // +0x80
    unsigned char _pad84[0x88 - 0x84];
    unsigned long TextTag;                              // +0x88
};
struct CDefPointer_CExpressionDef {
    CExpressionDef* m_pDef;
    CExpressionDef* operator->() const { return m_pDef; }
};
struct CTCExpression {
    unsigned char _pad0[0xc];
    CExpressionDef* PExpressionDef;                     // +0x0c
};
struct CTCEmoteIcon {
    void SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha);   // 0x0071AAB0
    void SetEmoteText(long priority, const CWideString& text, float activeTime, float scale);      // 0x0071AC70
};
struct CTCSound {
    unsigned long PlaySound(unsigned long textTag, long a, long b, long c, long d);       // 0x0066BA80
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);     // 0x0066BD00
};
struct CDeedEffectsDef : CRefDef {
    unsigned char _pad8[0x7c - 8];
    unsigned char Flags;                                // +0x7c
};
struct CDeedEffectsDefPointer {
    CDeedEffectsDef* m_pDef;
    __forceinline ~CDeedEffectsDefPointer()
    {
        if (m_pDef) {
            if (--m_pDef->RefCount == 0)
                m_pDef->Destroy();
        }
    }
    CDeedEffectsDef* operator->() const { return m_pDef; }
};
struct CTCHeroOpinionDeedLog {
    CDeedEffectsDefPointer GetDeedEffectsDef(long deed);   // 0x006C5630
    long PostDeed(long deed, const C3DVector* pos, CThing* thing, CThing* creature, long postType,
                  bool a, bool b, void* faction, bool c, long defIndex);   // 0x006C6990
};
struct CTarget { unsigned char _pad[0x10]; unsigned char Kind; };
struct CTC8 {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32();
    virtual CThing* GetTarget();                        // slot 33 (+0x84)
};
struct CExpressionGlobals { unsigned char _pad[0x128]; long ExpressionSprite; };
struct CDefinitionManagerView { unsigned char _pad[0xd0]; CExpressionGlobals* PGlobals; };
// Layout: retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4: the three STLport containers are 12 bytes, not 16).
struct CCreatureActionBase {
    virtual void v00();
    long _pad04;
    CIntelligentPointer PCreature;                      // +0x08
    long TotalTime;                                     // +0x10
    unsigned char _pad14[0xa8 - 0x14];
    void Initialise();                                  // 0x00694D10
    CDefinitionManagerView* GetPDefinitionManager();    // 0x00692910
};
struct CCreatureAction_PerformExpression : CCreatureActionBase {
    CDefPointer_CExpressionDef PExpressionDef;          // +0xa8
    CIntelligentPointer PPerformToThing;                // +0xac
    long ExpressionItemDefIndex;                        // +0xb4
    bool EmoteIconAdded;                                // +0xb8
    bool ReplacedAction;                                // +0xb9
    bool Extended;                                      // +0xba
    bool ButtonHeld;                                    // +0xbb
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual float GetAnimSeconds();
    void Initialise();
    CThingCreatureBase* Creature() const { return (CThingCreatureBase*)PCreature.GetPItem(); }
};

void CCreatureAction_PerformExpression::Initialise()
{
    CCreatureActionBase::Initialise();
    if (!Creature()->HasTC(0x8f)) {
        {
            CCharString name("CTCExpression");
            Creature()->AddTC(name, 0, 0);
        }
        CTCExpression* expression;
        Creature()->GetInterface(&expression, 0x8f);
        CExpressionDef* old = expression->PExpressionDef;
        if (old != PExpressionDef.m_pDef) {
            if (old) {
                if (--old->RefCount == 0)
                    old->Destroy();
                expression->PExpressionDef = 0;
            }
            CExpressionDef* def = PExpressionDef.m_pDef;
            expression->PExpressionDef = def;
            if (def)
                ++def->RefCount;
        }
    }
    if (!Creature()->HasTC(0x45)) {
        CCharString name("CTCEmoteIcon");
        Creature()->AddTC(name, 0, 0);
    }
    CTCEmoteIcon* emote;
    Creature()->GetInterface(&emote, 0x45);
    if (g_UseSpriteOverrideText && PExpressionDef->SpriteOverrideText != "") {
        {
            long index = PExpressionDef->SpriteOverrideText.m_index;
            const CCharString& str = g_DefStringTable.GetString(index);
            CWideString text(str.ToWideString());
            emote->SetEmoteText(7, text, -1.0f, 0.3f);
        }
        EmoteIconAdded = true;
    } else if (GetPDefinitionManager()->PGlobals->ExpressionSprite > 0) {
        float seconds = GetAnimSeconds() * TotalTime;
        if (Extended)
            seconds = -1.0f;
        long sprite = GetPDefinitionManager()->PGlobals->ExpressionSprite;
        emote->SetEmoteIcon(5, sprite, seconds, 0.5f, 255);
        EmoteIconAdded = true;
    }
    CTCSound* sound;
    if (Creature()->GetInterface(&sound, 0x10)) {
        if (PExpressionDef->TextTag) {
            if (PExpressionDef->OpinionDeedType != 0x18 || !Creature()->IsChild())
                sound->PlaySound(PExpressionDef->TextTag, 0, 0, 0, 0);
        } else if (PExpressionDef->OpinionDeedType == 0xf) {
            sound->PlaySound("EXPRESSION_SNEER", 0, 0, 0, -1);
        } else {
            sound->PlaySound("EXPRESSION_USE", 0, 0, 0, -1);
        }
    }
    if (Creature()->HasTC(0x29)) {
        g_pQuestManager->ActivateQuestFromExpression(g_DefStringTable.GetString(PExpressionDef->GetName().m_index));
        CTCHeroOpinionDeedLog* log;
        if (Creature()->GetInterface(&log, 0x51)) {
            bool special = PExpressionDef->OpinionDeedType != 0 && (log->GetDeedEffectsDef(PExpressionDef->OpinionDeedType)->Flags & 2);
            if (special) {
                CTC8* tc8;
                Creature()->GetInterface(&tc8, 8);
                CThing* target = tc8->GetTarget();
                if (target && ((CTarget*)target)->Kind == 1)
                    log->PostDeed(PExpressionDef->OpinionDeedType, Creature()->GetPosition(), target, target, 1, false, false, 0, false, 0);
            } else {
                log->PostDeed(PExpressionDef->OpinionDeedType, Creature()->GetPosition(), 0, 0, 2, false, false, 0, false, 0);
            }
        }
    }
}
