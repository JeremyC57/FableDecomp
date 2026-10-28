// CInputProcessBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- forward the input to the main hero's betting component (component kind 0x102) while betting input is enabled
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the map Find written as the ternary entry pick (`((it == last || kind < it->kind) ? last : it)->tc`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691560, length 105 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (player manager at CMainGameComponent +0x1c, the component global
// @ 0x013B86A0), CPlayer::GetPControlledCreature @ 0x00487DC0, the component map's LowerBound @ 0x0040F020; the component's
// virtual slot 28 (+0x70) takes (event, processed, this). The enable flag is the byte @ 0x013BAE1D.
// Shape: the presence test is the masked bitset word (`test byte ptr [eax+40h],4` for kind 0x102), the map `find` is
// inlined with its key in the `push ecx` frame word; the flag test sits in an inline member of the component
// (the component lands straight in ecx and is looked up before the flag is read).
class CInputEvent;
class CProcessedInput;
class CInputProcessBetting;
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void ProcessBettingInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessBetting* process);   // slot 28 (+0x70)
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
extern bool g_bBettingInputEnabled;                // 0x013BAE1D
class CTCBetting : public CTCBase { public:
    void ProcessInputIfEnabled(const CInputEvent& event, CProcessedInput& processed, CInputProcessBetting* process)
    {
        if (g_bBettingInputEnabled)
            ProcessBettingInput(event, processed, process);
    }
};

class CInputProcessBetting {                       // engine/CInputProcessBetting.h: vptr + 0x24-byte base
public:
    virtual void v00();
    unsigned char _base04[0x24];
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero->HasTC(0x102)) {
        static_cast<CTCBetting*>(hero->GetTC(0x102))->ProcessInputIfEnabled(event, processed, this);
    }
}