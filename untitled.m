

J = 6e-5; % momento de inércia do rotor [kg.m²]
c = 0.01; % fricção de viscosidade do motor [N.m.s/raid]
% % Ke = 0.04;
% % Kt = 0.04;
K = 0.04; % constante de proporcionalidade
Ra = 0.6; % resistência elétrica do motor [Ohm]
La = 0.002; % indutância elétrica do motor [H]


k = K/(Ra*J);

alpha = (c+K*K/Ra)/J;