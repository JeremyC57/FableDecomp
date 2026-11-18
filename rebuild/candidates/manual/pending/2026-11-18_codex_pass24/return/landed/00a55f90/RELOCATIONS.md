# Relocation audit

The VC7.1 leaf is `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z`, 259 bytes. Its only relocation is the absolute 32-bit load operand at leaf offset +0x0D, in `fmul` at 0x00A55F9B. Retail targets 0x0124F2B8, the established `?TWO_PI@@3NB` value. The COFF symbol is exactly `?TWO_PI@@3NB` and the retail double equals 6.2831854820251465, the float two-pi value promoted to double.
