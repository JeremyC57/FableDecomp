// 00c0acc0 MultiplyMaskedFactors -- descriptive name, established by the landed caller 0x00C0B130 binding ?MultiplyMaskedFactors@@YIMMPBMKK@Z; no PDB identity (Codex return 2026-10-31: NAME_SEARCH.md)
// Flags:
// Leaf: ?MultiplyMaskedFactors@@YIMMPBMKK@Z
// Fidelity: descriptive binding MultiplyMaskedFactors has no PDB identity (the landed caller already uses it).
float __fastcall MultiplyMaskedFactors(float value,const float* factors,unsigned long mask,unsigned long count)
{
    if(value != 0.0f){
        const float* cursor=factors;
        unsigned long remaining=count;
        do{
            if(mask==0 || remaining==0)break;
            if(mask&1)value*=*cursor;
            --remaining; mask>>=1; ++cursor;
        }while(value!=0.0f);
    }
    return value;
}