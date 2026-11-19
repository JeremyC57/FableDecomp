B0='''    if (m_changed & 0x20000) {
        m_front.Normalise();
        m_top.Normalise();
        float dot = m_front.Dot(m_top);
        if (dot > 0.05f || dot < -0.05f)
            m_top.SetCross(m_front, m_right);
        else
            m_right.SetCrossProduct(m_front, m_top);
    }'''
def rep(new):
    def f(s):
        assert B0 in s; return s.replace(B0,new)
    return f
def ptrs(order, cross='top->SetCross(*front, *right);'):
    decl={'f':'        C3DVector* front = &m_front;\n','t':'        C3DVector* top = &m_top;\n','r':'        C3DVector* right = &m_right;\n'}
    return rep('''    if (m_changed & 0x20000) {
'''+''.join(decl[c] for c in order)+'''        front->Normalise();
        top->Normalise();
        float dot = front->Dot(*top);
        if (dot > 0.05f || dot < -0.05f)
            '''+cross+'''
        else
            right->SetCrossProduct(*front, *top);
    }''')
INL='''        if (dot > 0.05f || dot < -0.05f) {
            m_top.X = m_front.Y * m_right.Z - m_front.Z * m_right.Y;
            m_top.Y = m_right.X * m_front.Z - m_front.X * m_right.Z;
            m_top.Z = m_front.X * m_right.Y - m_right.X * m_front.Y;
        } else'''
VARIANTS=[
 ('p_ftr', ptrs('ftr')), ('p_rft', ptrs('rft')), ('p_frt', ptrs('frt')), ('p_trf', ptrs('trf')), ('p_ft', ptrs('ft')+0 if False else ptrs('ft', 'top->SetCross(*front, m_right);')),
 ('p_rf', ptrs('rf', 'm_top.SetCross(*front, *right);')),
 ('p_ftr_inl', ptrs('ftr', '{ top->X = front->Y * right->Z - front->Z * right->Y; top->Y = right->X * front->Z - front->X * right->Z; top->Z = front->X * right->Y - right->X * front->Y; }')),
 ('inline_members', rep(B0.replace('''        if (dot > 0.05f || dot < -0.05f)
            m_top.SetCross(m_front, m_right);
        else''', INL))),
]
