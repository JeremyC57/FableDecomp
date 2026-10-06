// MMX / SSE / SSE2 lifting (included inside class Emitter).
// Operands are staged through Xmm temporaries; MMX registers use the low 8
// bytes. Returns false for mnemonics this file does not handle.

static bool isXmm(const ZydisDecodedOperand& o) {
    return o.type == ZYDIS_OPERAND_TYPE_REGISTER && o.reg.value >= ZYDIS_REGISTER_XMM0 && o.reg.value <= ZYDIS_REGISTER_XMM7;
}
static bool isMm(const ZydisDecodedOperand& o) {
    return o.type == ZYDIS_OPERAND_TYPE_REGISTER && o.reg.value >= ZYDIS_REGISTER_MM0 && o.reg.value <= ZYDIS_REGISTER_MM7;
}
static bool isGpr32(const ZydisDecodedOperand& o) {
    return o.type == ZYDIS_OPERAND_TYPE_REGISTER && ZydisRegisterGetClass(o.reg.value) == ZYDIS_REGCLASS_GPR32;
}

// Emit code loading operand `o` into Xmm temporary `t` (zero-filling).
std::string vload(const std::string& t, const ZydisDecodedOperand& o) {
    if (isXmm(o)) return t + " = c->xmm[" + std::to_string(o.reg.value - ZYDIS_REGISTER_XMM0) + "];";
    if (isMm(o)) return "memset(&" + t + ", 0, 16); " + t + ".q[0] = c->mm[" + std::to_string(o.reg.value - ZYDIS_REGISTER_MM0) + "];";
    if (o.type == ZYDIS_OPERAND_TYPE_REGISTER) return "memset(&" + t + ", 0, 16); " + t + ".d[0] = " + readReg(o.reg.value) + ";";
    if (o.type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
        return "memset(&" + t + ", 0, 16); " + t + ".q[0] = " + hexu(o.imm.value.u & 0xFF) + ";";
    const std::string a = addr(o);
    if (o.size >= 128) return "rdxmm(&" + t + ", " + a + ");";
    if (o.size == 64) return "memset(&" + t + ", 0, 16); " + t + ".q[0] = rd64(" + a + ");";
    if (o.size == 16) return "memset(&" + t + ", 0, 16); " + t + ".w[0] = rd16(" + a + ");";
    return "memset(&" + t + ", 0, 16); " + t + ".d[0] = rd32(" + a + ");";
}
// Emit code storing Xmm temporary `t` to operand `o` (memory width = o.size).
std::string vstore(const ZydisDecodedOperand& o, const std::string& t) {
    if (isXmm(o)) return "c->xmm[" + std::to_string(o.reg.value - ZYDIS_REGISTER_XMM0) + "] = " + t + ";";
    if (isMm(o)) return "c->mm[" + std::to_string(o.reg.value - ZYDIS_REGISTER_MM0) + "] = " + t + ".q[0];";
    if (o.type == ZYDIS_OPERAND_TYPE_REGISTER) return writeReg(o.reg.value, t + ".d[0]");
    const std::string a = addr(o);
    if (o.size >= 128) return "wrxmm(" + a + ", &" + t + ");";
    if (o.size == 64) return "wr64(" + a + ", " + t + ".q[0]);";
    return "wr32(" + a + ", " + t + ".d[0]);";
}

// dst = f(A = dst, B = src) lane-wise; `lanes` counts elements of `field`.
bool binop(const Insn& ins, const char* field, int laneBytes, const std::string& expr) {
    const auto* o = ins.op;
    const int bytesTotal = isMm(o[0]) ? 8 : 16;
    const int lanes = bytesTotal / laneBytes;
    line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; for (int i = 0; i < " + std::to_string(lanes) +
         "; ++i) R." + field + "[i] = " + expr + "; " + vstore(o[0], "R") + " }");
    return true;
}
// Scalar float op on lane 0 only.
bool scalar(const Insn& ins, const std::string& expr) {
    const auto* o = ins.op;
    line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; R.f[0] = " + expr + "; " + vstore(o[0], "R") + " }");
    return true;
}

bool simd(const Insn& ins) {
    const auto* o = ins.op;
    const auto m = ins.in.mnemonic;
    switch (m) {
    case ZYDIS_MNEMONIC_EMMS: return true;
    case ZYDIS_MNEMONIC_MOVAPS: case ZYDIS_MNEMONIC_MOVUPS: case ZYDIS_MNEMONIC_MOVDQA: case ZYDIS_MNEMONIC_MOVDQU:
    case ZYDIS_MNEMONIC_MOVAPD: case ZYDIS_MNEMONIC_MOVUPD:
    case ZYDIS_MNEMONIC_MOVNTPS: case ZYDIS_MNEMONIC_MOVNTPD: case ZYDIS_MNEMONIC_MOVNTDQ:  // non-temporal hint only
        line("{ Xmm T; " + vload("T", o[1]) + " " + vstore(o[0], "T") + " }");
        return true;
    case ZYDIS_MNEMONIC_MOVNTQ:
        line("{ Xmm T; " + vload("T", o[1]) + " wr64(" + addr(o[0]) + ", T.q[0]); }");
        return true;
    case ZYDIS_MNEMONIC_PAVGB: return binop(ins, "b", 1, "(uint8_t)(((uint32_t)A.b[i] + B.b[i] + 1u) >> 1)");
    case ZYDIS_MNEMONIC_PAVGW: return binop(ins, "w", 2, "(uint16_t)(((uint32_t)A.w[i] + B.w[i] + 1u) >> 1)");
    case ZYDIS_MNEMONIC_MOVQ:
        if (isXmm(o[0]) || isMm(o[0])) line("{ Xmm T; " + vload("T", o[1]) + " T.q[1] = 0; " + vstore(o[0], "T") + " }");
        else line("{ Xmm T; " + vload("T", o[1]) + " wr64(" + addr(o[0]) + ", T.q[0]); }");
        return true;
    case ZYDIS_MNEMONIC_MOVD:
        if (isXmm(o[0]) || isMm(o[0])) line("{ Xmm T; " + vload("T", o[1]) + " T.d[1] = 0; T.q[1] = 0; " + vstore(o[0], "T") + " }");
        else line("{ Xmm T; " + vload("T", o[1]) + " " + (o[0].type == ZYDIS_OPERAND_TYPE_MEMORY ? "wr32(" + addr(o[0]) + ", T.d[0]);" : writeReg(o[0].reg.value, "T.d[0]")) + " }");
        return true;
    case ZYDIS_MNEMONIC_MOVSS:
        if (isXmm(o[0]) && isXmm(o[1])) line("{ Xmm A, B; " + vload("A", o[0]) + " " + vload("B", o[1]) + " A.f[0] = B.f[0]; " + vstore(o[0], "A") + " }");
        else if (isXmm(o[0])) line("{ Xmm T; " + vload("T", o[1]) + " " + vstore(o[0], "T") +
                                   (nanCheck() ? " if (isnan(T.f[0])) recomp_nanload(c, " + hex(ins.addr) + "u, " + addr(o[1]) + ");" : "") + " }");
        else line("{ Xmm T; " + vload("T", o[1]) + " wr32(" + addr(o[0]) + ", T.d[0]);" +
                  (nanCheck() ? " if (isnan(T.f[0])) recomp_nan(c, " + hex(ins.addr) + "u);" : "") + " }");
        return true;
    case ZYDIS_MNEMONIC_MOVLPS: case ZYDIS_MNEMONIC_MOVHPS: {
        const std::string half = m == ZYDIS_MNEMONIC_MOVLPS ? "0" : "1";
        if (isXmm(o[0])) line("{ Xmm A; " + vload("A", o[0]) + " A.q[" + half + "] = rd64(" + addr(o[1]) + "); " + vstore(o[0], "A") + " }");
        else line("{ Xmm A; " + vload("A", o[1]) + " wr64(" + addr(o[0]) + ", A.q[" + half + "]); }");
        return true;
    }
    case ZYDIS_MNEMONIC_MOVHLPS:
        line("{ Xmm A, B; " + vload("A", o[0]) + " " + vload("B", o[1]) + " A.q[0] = B.q[1]; " + vstore(o[0], "A") + " }");
        return true;
    case ZYDIS_MNEMONIC_MOVLHPS:
        line("{ Xmm A, B; " + vload("A", o[0]) + " " + vload("B", o[1]) + " A.q[1] = B.q[0]; " + vstore(o[0], "A") + " }");
        return true;
    case ZYDIS_MNEMONIC_ADDPS: return binop(ins, "f", 4, "A.f[i] + B.f[i]");
    case ZYDIS_MNEMONIC_SUBPS: return binop(ins, "f", 4, "A.f[i] - B.f[i]");
    case ZYDIS_MNEMONIC_MULPS: return binop(ins, "f", 4, "A.f[i] * B.f[i]");
    case ZYDIS_MNEMONIC_DIVPS: return binop(ins, "f", 4, "A.f[i] / B.f[i]");
    case ZYDIS_MNEMONIC_MINPS: return binop(ins, "f", 4, "(A.f[i] < B.f[i]) ? A.f[i] : B.f[i]");
    case ZYDIS_MNEMONIC_MAXPS: return binop(ins, "f", 4, "(A.f[i] > B.f[i]) ? A.f[i] : B.f[i]");
    case ZYDIS_MNEMONIC_SQRTPS: return binop(ins, "f", 4, "sqrtf(B.f[i])");
    case ZYDIS_MNEMONIC_RSQRTPS: return binop(ins, "f", 4, "1.0f / sqrtf(B.f[i])");
    case ZYDIS_MNEMONIC_RCPPS: return binop(ins, "f", 4, "1.0f / B.f[i]");
    case ZYDIS_MNEMONIC_ANDPS: case ZYDIS_MNEMONIC_PAND: return binop(ins, "d", 4, "A.d[i] & B.d[i]");
    case ZYDIS_MNEMONIC_ANDNPS: case ZYDIS_MNEMONIC_PANDN: return binop(ins, "d", 4, "~A.d[i] & B.d[i]");
    case ZYDIS_MNEMONIC_ORPS: case ZYDIS_MNEMONIC_POR: return binop(ins, "d", 4, "A.d[i] | B.d[i]");
    case ZYDIS_MNEMONIC_XORPS: case ZYDIS_MNEMONIC_PXOR: return binop(ins, "d", 4, "A.d[i] ^ B.d[i]");
    case ZYDIS_MNEMONIC_ADDSS: return scalar(ins, "A.f[0] + B.f[0]");
    case ZYDIS_MNEMONIC_SUBSS: return scalar(ins, "A.f[0] - B.f[0]");
    case ZYDIS_MNEMONIC_MULSS: return scalar(ins, "A.f[0] * B.f[0]");
    case ZYDIS_MNEMONIC_DIVSS: return scalar(ins, "A.f[0] / B.f[0]");
    case ZYDIS_MNEMONIC_MINSS: return scalar(ins, "(A.f[0] < B.f[0]) ? A.f[0] : B.f[0]");
    case ZYDIS_MNEMONIC_MAXSS: return scalar(ins, "(A.f[0] > B.f[0]) ? A.f[0] : B.f[0]");
    case ZYDIS_MNEMONIC_SQRTSS: return scalar(ins, "sqrtf(B.f[0])");
    case ZYDIS_MNEMONIC_RSQRTSS: return scalar(ins, "1.0f / sqrtf(B.f[0])");
    case ZYDIS_MNEMONIC_RCPSS: return scalar(ins, "1.0f / B.f[0]");
    case ZYDIS_MNEMONIC_SHUFPS: {
        const unsigned k = static_cast<unsigned>(o[2].imm.value.u);
        line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R.f[0] = A.f[" + std::to_string(k & 3) + "]; R.f[1] = A.f[" +
             std::to_string((k >> 2) & 3) + "]; R.f[2] = B.f[" + std::to_string((k >> 4) & 3) + "]; R.f[3] = B.f[" + std::to_string((k >> 6) & 3) +
             "]; " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_PSHUFD: {
        const unsigned k = static_cast<unsigned>(o[2].imm.value.u);
        line("{ Xmm B, R; " + vload("B", o[1]) + " R.d[0] = B.d[" + std::to_string(k & 3) + "]; R.d[1] = B.d[" + std::to_string((k >> 2) & 3) +
             "]; R.d[2] = B.d[" + std::to_string((k >> 4) & 3) + "]; R.d[3] = B.d[" + std::to_string((k >> 6) & 3) + "]; " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_PSHUFW: {
        const unsigned k = static_cast<unsigned>(o[2].imm.value.u);
        line("{ Xmm B, R; " + vload("B", o[1]) + " memset(&R, 0, 16); for (int i = 0; i < 4; ++i) R.w[i] = B.w[(" + std::to_string(k) +
             "u >> (2 * i)) & 3u]; " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_UNPCKLPS:
        line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R.f[0] = A.f[0]; R.f[1] = B.f[0]; R.f[2] = A.f[1]; R.f[3] = B.f[1]; " + vstore(o[0], "R") + " }");
        return true;
    case ZYDIS_MNEMONIC_UNPCKHPS:
        line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R.f[0] = A.f[2]; R.f[1] = B.f[2]; R.f[2] = A.f[3]; R.f[3] = B.f[3]; " + vstore(o[0], "R") + " }");
        return true;
    case ZYDIS_MNEMONIC_CMPPS: case ZYDIS_MNEMONIC_CMPSS: {
        static const char* preds[] = {"a == b", "a < b", "a <= b", "isnan(a) || isnan(b)", "!(a == b)", "!(a < b)", "!(a <= b)", "!isnan(a) && !isnan(b)"};
        const std::string pr = preds[o[2].imm.value.u & 7];
        const std::string lanes = m == ZYDIS_MNEMONIC_CMPPS ? "4" : "1";
        line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; for (int i = 0; i < " + lanes +
             "; ++i) { float a = A.f[i], b = B.f[i]; R.d[i] = (" + pr + ") ? 0xFFFFFFFFu : 0u; } " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_COMISS: case ZYDIS_MNEMONIC_UCOMISS:
        line("{ Xmm A, B; " + vload("A", o[0]) + " " + vload("B", o[1]) + " fr = fcmpi(A.f[0], B.f[0]); fop = FOP(FK_EXPLICIT, 4); }");
        return true;
    case ZYDIS_MNEMONIC_MOVMSKPS:
        line("{ Xmm A; " + vload("A", o[1]) + " " + writeReg(o[0].reg.value, "(A.d[0] >> 31) | ((A.d[1] >> 31) << 1) | ((A.d[2] >> 31) << 2) | ((A.d[3] >> 31) << 3)") + " }");
        return true;
    case ZYDIS_MNEMONIC_CVTDQ2PS: return binop(ins, "f", 4, "(float)(int32_t)B.d[i]");
    case ZYDIS_MNEMONIC_CVTPS2DQ:
        return binop(ins, "d", 4, "(uint32_t)((B.f[i] >= -2147483648.0f && B.f[i] < 2147483648.0f) ? (int32_t)nearbyintf(B.f[i]) : (int32_t)0x80000000u)");
    case ZYDIS_MNEMONIC_CVTTPS2DQ:
        return binop(ins, "d", 4, "(uint32_t)((B.f[i] >= -2147483648.0f && B.f[i] < 2147483648.0f) ? (int32_t)B.f[i] : (int32_t)0x80000000u)");
    case ZYDIS_MNEMONIC_CVTPI2PS:
        line("{ Xmm A, B; " + vload("A", o[0]) + " " + vload("B", o[1]) + " A.f[0] = (float)(int32_t)B.d[0]; A.f[1] = (float)(int32_t)B.d[1]; " + vstore(o[0], "A") + " }");
        return true;
    case ZYDIS_MNEMONIC_CVTPS2PI: case ZYDIS_MNEMONIC_CVTTPS2PI: {
        const std::string f = m == ZYDIS_MNEMONIC_CVTPS2PI ? "nearbyintf" : "truncf";
        line("{ Xmm B, R; " + vload("B", o[1]) + " memset(&R, 0, 16); for (int i = 0; i < 2; ++i) R.d[i] = (B.f[i] >= -2147483648.0f && B.f[i] < 2147483648.0f) ? (uint32_t)(int32_t)" +
             f + "(B.f[i]) : 0x80000000u; " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_CVTSI2SS:
        line("{ Xmm A; " + vload("A", o[0]) + " A.f[0] = (float)(int32_t)" + rd(o[1], 32) + "; " + vstore(o[0], "A") + " }");
        return true;
    case ZYDIS_MNEMONIC_CVTSS2SI: case ZYDIS_MNEMONIC_CVTTSS2SI: {
        const std::string f = m == ZYDIS_MNEMONIC_CVTSS2SI ? "nearbyintf" : "truncf";
        line("{ Xmm B; " + vload("B", o[1]) + " float v = B.f[0]; " +
             wr(o[0], 32, "(v >= -2147483648.0f && v < 2147483648.0f) ? (uint32_t)(int32_t)" + f + "(v) : 0x80000000u") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_LDMXCSR: line("c->mxcsr = rd32(" + addr(o[0]) + ");"); return true;
    case ZYDIS_MNEMONIC_STMXCSR: line("wr32(" + addr(o[0]) + ", c->mxcsr);"); return true;
    // ---- integer SIMD ----
    case ZYDIS_MNEMONIC_PADDB: return binop(ins, "b", 1, "(uint8_t)(A.b[i] + B.b[i])");
    case ZYDIS_MNEMONIC_PADDW: return binop(ins, "w", 2, "(uint16_t)(A.w[i] + B.w[i])");
    case ZYDIS_MNEMONIC_PADDD: return binop(ins, "d", 4, "A.d[i] + B.d[i]");
    case ZYDIS_MNEMONIC_PADDQ: return binop(ins, "q", 8, "A.q[i] + B.q[i]");
    case ZYDIS_MNEMONIC_PSUBB: return binop(ins, "b", 1, "(uint8_t)(A.b[i] - B.b[i])");
    case ZYDIS_MNEMONIC_PSUBW: return binop(ins, "w", 2, "(uint16_t)(A.w[i] - B.w[i])");
    case ZYDIS_MNEMONIC_PSUBD: return binop(ins, "d", 4, "A.d[i] - B.d[i]");
    case ZYDIS_MNEMONIC_PSUBQ: return binop(ins, "q", 8, "A.q[i] - B.q[i]");
    case ZYDIS_MNEMONIC_PADDSW: return binop(ins, "w", 2, "(uint16_t)sat16((int16_t)A.w[i] + (int16_t)B.w[i])");
    case ZYDIS_MNEMONIC_PSUBSW: return binop(ins, "w", 2, "(uint16_t)sat16((int16_t)A.w[i] - (int16_t)B.w[i])");
    case ZYDIS_MNEMONIC_PADDUSB: return binop(ins, "b", 1, "satu8((int32_t)A.b[i] + B.b[i])");
    case ZYDIS_MNEMONIC_PSUBUSB: return binop(ins, "b", 1, "satu8((int32_t)A.b[i] - B.b[i])");
    case ZYDIS_MNEMONIC_PADDUSW: return binop(ins, "w", 2, "(uint16_t)((uint32_t)A.w[i] + B.w[i] > 0xFFFFu ? 0xFFFFu : A.w[i] + B.w[i])");
    case ZYDIS_MNEMONIC_PSUBUSW: return binop(ins, "w", 2, "(uint16_t)(A.w[i] > B.w[i] ? A.w[i] - B.w[i] : 0)");
    case ZYDIS_MNEMONIC_PMULLW: return binop(ins, "w", 2, "(uint16_t)((int32_t)(int16_t)A.w[i] * (int16_t)B.w[i])");
    case ZYDIS_MNEMONIC_PMULHW: return binop(ins, "w", 2, "(uint16_t)(((int32_t)(int16_t)A.w[i] * (int16_t)B.w[i]) >> 16)");
    case ZYDIS_MNEMONIC_PMULHUW: return binop(ins, "w", 2, "(uint16_t)(((uint32_t)A.w[i] * B.w[i]) >> 16)");
    case ZYDIS_MNEMONIC_PMADDWD:
        return binop(ins, "d", 4, "(uint32_t)((int32_t)(int16_t)A.w[2*i] * (int16_t)B.w[2*i] + (int32_t)(int16_t)A.w[2*i+1] * (int16_t)B.w[2*i+1])");
    case ZYDIS_MNEMONIC_PCMPEQB: return binop(ins, "b", 1, "(uint8_t)(A.b[i] == B.b[i] ? 0xFF : 0)");
    case ZYDIS_MNEMONIC_PCMPEQW: return binop(ins, "w", 2, "(uint16_t)(A.w[i] == B.w[i] ? 0xFFFF : 0)");
    case ZYDIS_MNEMONIC_PCMPEQD: return binop(ins, "d", 4, "A.d[i] == B.d[i] ? 0xFFFFFFFFu : 0u");
    case ZYDIS_MNEMONIC_PCMPGTB: return binop(ins, "b", 1, "(uint8_t)((int8_t)A.b[i] > (int8_t)B.b[i] ? 0xFF : 0)");
    case ZYDIS_MNEMONIC_PCMPGTW: return binop(ins, "w", 2, "(uint16_t)((int16_t)A.w[i] > (int16_t)B.w[i] ? 0xFFFF : 0)");
    case ZYDIS_MNEMONIC_PCMPGTD: return binop(ins, "d", 4, "(int32_t)A.d[i] > (int32_t)B.d[i] ? 0xFFFFFFFFu : 0u");
    case ZYDIS_MNEMONIC_PSLLW: case ZYDIS_MNEMONIC_PSLLD: case ZYDIS_MNEMONIC_PSLLQ:
    case ZYDIS_MNEMONIC_PSRLW: case ZYDIS_MNEMONIC_PSRLD: case ZYDIS_MNEMONIC_PSRLQ:
    case ZYDIS_MNEMONIC_PSRAW: case ZYDIS_MNEMONIC_PSRAD: {
        const bool w16 = m == ZYDIS_MNEMONIC_PSLLW || m == ZYDIS_MNEMONIC_PSRLW || m == ZYDIS_MNEMONIC_PSRAW;
        const bool q64 = m == ZYDIS_MNEMONIC_PSLLQ || m == ZYDIS_MNEMONIC_PSRLQ;
        const bool left = m == ZYDIS_MNEMONIC_PSLLW || m == ZYDIS_MNEMONIC_PSLLD || m == ZYDIS_MNEMONIC_PSLLQ;
        const bool arith = m == ZYDIS_MNEMONIC_PSRAW || m == ZYDIS_MNEMONIC_PSRAD;
        const int bits = w16 ? 16 : q64 ? 64 : 32;
        const std::string field = w16 ? "w" : q64 ? "q" : "d";
        const int total = isMm(o[0]) ? 8 : 16;
        const std::string lanes = std::to_string(total * 8 / bits);
        const std::string cnt = o[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE ? "(uint64_t)" + hexu(o[1].imm.value.u & 0xFF) : "K.q[0]";
        std::string e;
        if (arith) e = w16 ? "(uint16_t)((int16_t)A.w[i] >> (n > 15 ? 15 : n))" : "(uint32_t)((int32_t)A.d[i] >> (n > 31 ? 31 : n))";
        else e = std::string("n >= ") + std::to_string(bits) + " ? 0 : (A." + field + "[i] " + (left ? "<<" : ">>") + " n)";
        line("{ Xmm A, K, R; " + vload("A", o[0]) + " " + (o[1].type == ZYDIS_OPERAND_TYPE_IMMEDIATE ? "" : vload("K", o[1])) +
             " uint64_t n = " + cnt + "; R = A; for (int i = 0; i < " + lanes + "; ++i) R." + field + "[i] = " + e + "; " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_PUNPCKLBW: case ZYDIS_MNEMONIC_PUNPCKHBW: case ZYDIS_MNEMONIC_PUNPCKLWD: case ZYDIS_MNEMONIC_PUNPCKHWD:
    case ZYDIS_MNEMONIC_PUNPCKLDQ: case ZYDIS_MNEMONIC_PUNPCKHDQ: case ZYDIS_MNEMONIC_PUNPCKLQDQ: case ZYDIS_MNEMONIC_PUNPCKHQDQ: {
        const bool high = m == ZYDIS_MNEMONIC_PUNPCKHBW || m == ZYDIS_MNEMONIC_PUNPCKHWD || m == ZYDIS_MNEMONIC_PUNPCKHDQ || m == ZYDIS_MNEMONIC_PUNPCKHQDQ;
        const std::string field = (m == ZYDIS_MNEMONIC_PUNPCKLBW || m == ZYDIS_MNEMONIC_PUNPCKHBW) ? "b"
                                  : (m == ZYDIS_MNEMONIC_PUNPCKLWD || m == ZYDIS_MNEMONIC_PUNPCKHWD) ? "w"
                                  : (m == ZYDIS_MNEMONIC_PUNPCKLDQ || m == ZYDIS_MNEMONIC_PUNPCKHDQ) ? "d" : "q";
        const int esz = field == "b" ? 1 : field == "w" ? 2 : field == "d" ? 4 : 8;
        const int total = isMm(o[0]) ? 8 : 16;
        const int half = total / esz / 2;
        line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; for (int i = 0; i < " + std::to_string(half) + "; ++i) { R." +
             field + "[2*i] = A." + field + "[" + (high ? std::to_string(half) + " + " : "") + "i]; R." + field + "[2*i+1] = B." + field + "[" +
             (high ? std::to_string(half) + " + " : "") + "i]; } " + vstore(o[0], "R") + " }");
        return true;
    }
    case ZYDIS_MNEMONIC_PACKSSDW: case ZYDIS_MNEMONIC_PACKSSWB: case ZYDIS_MNEMONIC_PACKUSWB: {
        const int total = isMm(o[0]) ? 8 : 16;
        if (m == ZYDIS_MNEMONIC_PACKSSDW) {
            const int n = total / 4;
            line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; for (int i = 0; i < " + std::to_string(n) +
                 "; ++i) { R.w[i] = (uint16_t)sat16((int32_t)A.d[i]); R.w[" + std::to_string(n) + " + i] = (uint16_t)sat16((int32_t)B.d[i]); } " + vstore(o[0], "R") + " }");
        } else {
            const int n = total / 2;
            const std::string sat = m == ZYDIS_MNEMONIC_PACKUSWB ? "satu8((int16_t)" : "(uint8_t)(int8_t)sat8((int16_t)";
            line("{ Xmm A, B, R; " + vload("A", o[0]) + " " + vload("B", o[1]) + " R = A; for (int i = 0; i < " + std::to_string(n) + "; ++i) { R.b[i] = " +
                 sat + "A.w[i]); R.b[" + std::to_string(n) + " + i] = " + sat + "B.w[i]); } " + vstore(o[0], "R") + " }");
        }
        return true;
    }
    case ZYDIS_MNEMONIC_PEXTRW:
        line("{ Xmm B; " + vload("B", o[1]) + " " + writeReg(o[0].reg.value, "B.w[" + std::to_string(o[2].imm.value.u & (isMm(o[1]) ? 3 : 7)) + "]") + " }");
        return true;
    case ZYDIS_MNEMONIC_PINSRW:
        line("{ Xmm A; " + vload("A", o[0]) + " A.w[" + std::to_string(o[2].imm.value.u & (isMm(o[0]) ? 3 : 7)) + "] = (uint16_t)" + rd(o[1], 16) + "; " + vstore(o[0], "A") + " }");
        return true;
    default:
        return false;
    }
}
