// CRampedParam::Update(float dt) -- advance a ramped audio parameter: while the ramp runs, evaluate the value and its rate from the Taylor coefficients at the new time; at the end snap to the target (first time) or re-anchor the target on the value
// Grade: DIFFER(171v167) -- x87 sum canonicalisation: retail sums the rate as (h*c3 + t*c2) + c*c4 + c1 and the value by accumulating into the q slot (fxch st3 ; faddp st3: q*c4 + t*c1, c*c3, h*c2); VC7.1 here emits t*c2 first and folds the value top-down (c, h, t) whatever the source term order (3 orders tried); the finished-path temporaries also swap eax/edx
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
    m_rate = m_coef[1] + m_coef[2] * m_time + m_coef[3] * h + m_coef[4] * c;
    m_value = m_coef[0] + m_coef[1] * m_time + m_coef[2] * h + m_coef[3] * c + m_coef[4] * q;
}
