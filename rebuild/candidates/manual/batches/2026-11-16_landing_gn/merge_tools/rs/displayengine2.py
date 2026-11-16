def R(k,o,b,t):
    i=t.index('    bool IsScreenFullyFadedOut() const;                                 // 0x004A9210')
    assert t[i+1].startswith('    void EndScreenFadeOut();')
    t=t[:i]+['    // (IsScreenFullyFadedOut / EndScreenFadeOut are the FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT / _ENDSCREENFADEOUT members',
             '    // when those opt-ins are also set.)',
             '#ifndef FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT',
             t[i], '#endif', '#ifndef FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT', t[i+1], '#endif']+t[i+2:]
    return o+t
