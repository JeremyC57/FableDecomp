// CRampedParam::Update(float dt) -- advance a ramped audio parameter: while the ramp runs, evaluate the value and its rate from the Taylor coefficients at the new time; at the end snap to the target (first time) or re-anchor the target on the value
// Grade: DIFFER(175v167) -- statement-sequenced sums (`rate = c3*h; rate += c2*t; ...`) stop VC7.1's reassociation: the rate block and the value's term order (q, t, c, h, then c0) now match retail instruction for instruction; left: at `value += c3*c` the lane copies c (`fld st0 ; fmul [ecx+28h] ; faddp st3`) and pops it at the end (`fstp st0`) where retail multiplies it in place (`fmul [ecx+28h] ; faddp st2`); tried c as a repeated expression and `c * m_coef[3]`; the one-expression value reorders to (q, c, h, t) (171 bytes, the parked form)
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B7B0, 167 bytes (`ret 4`). Descriptive names: the manifest's
//   "AnimationCurve_UpdateWithDelta" is propagated. Caller: the parameter block's Update 0x00C0AF20 (dt = 1000 / rate).
// Constants: 0.5 @ 0x0122F59C, 1/3 @ 0x0125A4DC, 1/6 @ 0x0125D640.
// Tried: bool r = (a < b) (branches both ways; `bool r = false; if (a < b) r = true;` gives retail's xor dl,dl ;
//   mov dl,1), `if (m_time < m_duration) {...} else` (inverted layout; the early-return `>=` form matches), a local t
//   (h*t loads h; using m_time CSE'd on the stack gives retail's `fld st1 ; fmul st1`), three term orders (no change).
// Shape: value(t) = c0 + c1 t + c2 t^2/2 + c3 t^3/6 + c4 t^4/24 and rate(t) its derivative, with h = t*t*0.5,
//   c = t*h/3 and q = h*h/6 kept on the x87 stack.
struct CRampedParam {                           // 0x34 bytes
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_targetRate;                         // +0x08 rate on arrival
    float m_rate;                               // +0x0c current rate
    float m_10;                                 // +0x10
    float m_time;                               // +0x14 elapsed ramp time
    float m_duration;                           // +0x18 ramp duration
    float m_coef[5];                            // +0x1c Taylor coefficients of the ramp (value, rate, 2nd..4th derivative)
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void Update(float dt);
};

void CRampedParam::Update(float dt)
{
    bool wasRunning = false;
    if (m_time < m_duration)
        wasRunning = true;
    m_time += dt;
    if (m_time >= m_duration) {
        if (wasRunning) {
            m_rate = m_targetRate;
            m_value = m_target;
            m_10 = 0.0f;
            m_time = m_duration;
        } else {
            m_target = m_value;
            m_time = m_duration;
        }
        return;
    }
    float h = m_time * m_time * 0.5f;
    float c = m_time * h * 0.333333343f;
    float q = h * h * 0.166666672f;
    float rate = m_coef[3] * h;
    rate += m_coef[2] * m_time;
    rate += m_coef[4] * c;
    rate += m_coef[1];
    m_rate = rate;
    float value = m_coef[4] * q;
    value += m_coef[1] * m_time;
    value += m_coef[3] * c;
    value += m_coef[2] * h;
    value += m_coef[0];
    m_value = value;
}
