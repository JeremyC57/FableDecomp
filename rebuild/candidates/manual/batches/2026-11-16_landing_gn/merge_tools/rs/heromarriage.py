def R(k,o,b,t):
    assert o==['#ifdef FABLE_RETAIL_CHEROMARRIAGEINFO_CTOR','    EMarriageState   State;                                  // +0x10','#else']
    return ['#if defined(FABLE_RETAIL_CHEROMARRIAGEINFO_CTOR) || defined(FABLE_RETAIL_CHEROMARRIAGEINFO_STATE)']+t[1:]
