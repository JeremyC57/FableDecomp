def R(k,o,b,t):
    assert t[0]=='#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPARETEST' and t[3]=='protected:' and t[5]=='public:'
    return o+t[:4]+['#if !defined(FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_OCCLUSIONTEST) && !defined(FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST)',
                    '    // (those opt-ins declare it)', t[4], '#endif']+t[5:]
