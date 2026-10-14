// CIsFreeHopscotchBoard::operator()(CThing* thing) -- true for a thing whose virtual slot 48 flags carry bit 15 and whose +0x24 flags carry bit 13, when its user-action-point component (kind 0x2d, the component map at +0x44) reports the point free (0x007FEBF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941BE0, length 96 bytes.
// Shape: `shr eax,0Fh ; test al,1`, `test ah,20h`, the key temporary in the dead argument slot, the lower-bound tail with the end entry read, `ret 4`.
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long           m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                      // 0x007FEBF0
};
struct SStateFlags_00941be0 {                      // the slot-48 flags word
    unsigned long m_low : 15;
    unsigned long m_isBoard : 1;                   // bit 15 (`shr eax,0Fh ; test al,1`)
    unsigned long m_high : 16;
};
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetStateFlags();         // slot 48 (+0xc0)
    unsigned char  _pad04[0x20];
    unsigned long  m_flags24;                      // +0x24
    unsigned char  _pad28[0x1c];
    CTCMap_Retail  m_tcs;                          // +0x44
};
struct CIsFreeHopscotchBoard {
    bool operator()(CThing_Retail* thing);
};

bool CIsFreeHopscotchBoard::operator()(CThing_Retail* thing)
{
    if ((unsigned char)(thing->GetStateFlags() >> 15) & 1) {   // `shr eax,0Fh ; test al,1`
        if (thing->m_flags24 & 0x2000) {
            if (((CTCUserActionPointBase*)thing->m_tcs.Find(0x2d))->IsActionPointFree())
                return true;
        }
    }
    return false;
}
