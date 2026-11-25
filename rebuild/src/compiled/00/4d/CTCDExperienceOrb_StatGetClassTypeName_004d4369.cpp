class CCharString { public:
    char* p;
    /* real thiscall member at 0x99ebf0: this=ecx, (const char*, int) on stack */
    void Init(const char* s, int n);
};

// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x0123BB40; it is now the literal spelled from retail's bytes
//   at 0x0123BB40 (narrow "CTCDExperienceOrb", NUL-terminated).
/* value-returning method: ecx = hidden return-object pointer (esi in retail) */
CCharString* __fastcall FableStatGetClassTypeName_5(CCharString* retbuf)
{
    retbuf->Init("CTCDExperienceOrb", -1);
    return retbuf;
}