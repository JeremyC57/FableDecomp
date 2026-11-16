def R(k,o,b,t):
    assert o[0]=='#ifdef FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT' and o[5]=='#ifdef FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT'
    assert t[0]=='#ifdef FABLE_RETAIL_CDISPLAYENGINE_SETGAMMARAMP' and t[6].startswith('#if defined(FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT) && defined(FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT)')
    alpha=t[7:-1]; plain=o[6:-1]
    return (o[:5]+t[:6]+
      ['#if defined(FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT) && defined(FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT) && defined(FABLE_RETAIL_CFADEINFADEOUTBASE_FADE)']+
      alpha+
      ['    // Two texts are on record for IsScreenFullyFadedOut (both descriptive, from row bytes): this one through GetScreenFadeOutAlpha',
       '    // (wfA2 rows 0x00439070 / 0x00435530, which define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE) and the member test below (wfA1 rows',
       '    // 0x006D0830 / 0x006D1E30, FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN). The FADE opt-in that GetFadeInOutAlpha needs selects this one.',
       '#elif defined(FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT)']+
      plain+['#endif'])
