def R(k,o,b,t):
    assert o==['    void Clear();','    CThing* GetPTargetedThing() const;'] and t[0]=='#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWHIGHLIGHTINGTARGETINGCURSOR_CLEAR' and t[-1]=='#endif'
    return t[:-1]+['#else']+o+['#endif']
