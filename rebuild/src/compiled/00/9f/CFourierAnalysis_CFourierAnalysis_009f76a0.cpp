// CFourierAnalysis::CFourierAnalysis() -- empty default constructor, compiled without optimisation
// Retail: Fable.exe 0x009F76A0, 14 bytes: `push ebp ; mov ebp,esp ; push ecx ; mov [ebp-4],ecx ; mov eax,[ebp-4] ;
// mov esp,ebp ; pop ebp ; ret` -- the /Od frame of a constructor that only returns `this`.
// Identity: catalog name; Ego_r.pdb ??0CFourierAnalysis@@QAE@XZ (the class has no data members in the PDB layouts and
// only member functions over CArray<CComplexNumber>). The TU is unoptimised: #pragma optimize("", off).
// Leaf: ??0CFourierAnalysis@@QAE@XZ
#pragma optimize("", off)

class CFourierAnalysis {
public:
    CFourierAnalysis();
};

CFourierAnalysis::CFourierAnalysis()
{
}